#include <server.h>
#include <socket.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string>
#include <iostream>
#include <atomic>
#include <cstring>
#include <fstream>

Server::Server(){
};

Server::~Server(){
  //If socket is open, close.
  if(connectionSocket.getFileDescriptor() >= 0){
    connectionSocket.closeSocket();
  }
}

//Setters
void Server::setConnectionSocket(Socket socket){
  connectionSocket = socket;
};

void Server::setAddress(sockaddr_in addr){
  address = addr;
}

//Getters
sockaddr_in Server::getAddress(){
  return address;
}

Socket Server::getConnectionSocket(){
  return connectionSocket;
};

std::list<sockaddr_in> Server::getClients(){
  return clients;
}

char* Server::getBuffer(){
  return buffer;
};

//Functional Methods

//Calls bind socket
int Server::bindSocket(void){
  int bindResult = connectionSocket.bindSocket(&address, sizeof(address));

  //If operation wasn't successful
  if(bindResult < 0){
      throw std::runtime_error(
        std::string("Couldn't bind socket: ") +
        std::to_string(bindResult) +  + " Error Number: " +
        std::to_string(errno)
      );
  }
  return bindResult;
}

//Listens and makes the socket accept connections
int Server::listen(std::atomic<bool> &stopServerFlag){
  int listenStatus;
  listenStatus = connectionSocket.openSocket();

  //If couldn't open socket
  if(listenStatus < 0){
    throw std::runtime_error(
      std::string("Couldn't make the socket listen correctly: ") +
      std::to_string(listenStatus) + " Error Number: " +
      std::to_string(errno)
    );
  }

  //While stopServer is false
  while(!stopServerFlag){
    //Create an empty client
    sockaddr_in client;

    //Create a client's socket file descriptor
    int clientConnectionFd;

    //Pass the empty client, as a sockadrr*, which is what "::listen()" requires
    //Accept requests as long as there's no stop serverflag, the client file descriptor is less than 0
    //Or the errno is EINTR
    clientConnectionFd = connectionSocket.acceptRequests((sockaddr*)&client);

    //If server already stopped, break;
    if(stopServerFlag)
      break;

    //If couldn't accept the request, 
    //means there's an error on system, regardless of what client sent
    if(clientConnectionFd < 0){
      throw std::runtime_error(
        std::string("Socket isn't able to accept requests: ") +
        std::to_string(clientConnectionFd) + " Error Number: " +
        std::to_string(errno)
      );
    }

    //If client has invalid address, but accept operation was successfull, dont push to client list
    if(client.sin_addr.s_addr == 0){
      std::cerr << "Warning: Client address is 0. Request accepted, but client is invalid. Skipping." << std::endl;
    } else {
      //If all is good, push client to clients list
      clients.push_back(client);

      //Setting buffer to 0, just for next read
      memset(buffer, 0, sizeof(buffer));

      //Reading HTTP request from client
      ssize_t readResult = read(clientConnectionFd, buffer, BUFFER_SIZE + 1);

      if (readResult < 0) {
        throw std::runtime_error("Couldn't read client's buffer: " + std::to_string(readResult) + " Error Number: " + std::to_string(errno));
      } else {
        //NULL-terminate buffer
        buffer[readResult] = '\0';

        //Print buffer
        std::cout << buffer << std::endl;

        //Print information
        std::cout << "Bytes Read: " << readResult << std::endl;
        } 

        //Set buffer to 0 for write
        memset(buffer, 0, sizeof(buffer));

        //Send information
        FILE* htmlFilePtr = ::fopen("html/index.html", "r");
        
        if(htmlFilePtr != NULL){
          size_t htmlReadResult = fread(buffer, 1, BUFFER_SIZE - 1, htmlFilePtr);

          //If couldn't read anything
          if (htmlReadResult == 0 && ferror(htmlFilePtr)) {
            throw std::runtime_error("Couldn't read index.html buffer: " + std::to_string(htmlReadResult) + " Error Number: " + std::to_string(errno));
          } else {
            //NULL-terminate buffer
            buffer[htmlReadResult] = '\0';

            //Print buffer
            std::cout << buffer << std::endl;

            //Print information
            std::cout << "Bytes Read: " << htmlReadResult << std::endl;

            //Write buffer to client 
            std::string responseHeader = "HTTP/1.1 200 OK\r\n" "Content-Type: text/html\r\n" "Content-Length: " + 
            std::to_string(htmlReadResult) + 
            "\r\n" "Connection: close\r\n" "\r\n";

            //Write headers to client
            ssize_t headerWritten = ::write(clientConnectionFd, responseHeader.c_str(), responseHeader.size());
            if (headerWritten < 0) {
              throw std::runtime_error("Couldn't write HTTP headers; Error Number: " + std::to_string(errno));
            }

            //Write HTML to client
            ssize_t sent;

            //Total bytes sent so far
            size_t totalSent = 0;                 

            //Bytes remaining to send
            size_t remainingBytes = htmlReadResult; 

            //Pointer to current position in buffer
            char* currentPtr = buffer;

            //While totalSent is less then the size of htlmReadResult (in bytes)...        
            while (totalSent < htmlReadResult) {
              // Attempt to write remaining bytes
              sent = ::write(clientConnectionFd, currentPtr, remainingBytes);

              if (sent < 0) {
                throw std::runtime_error(
                  "Couldn't write HTML; bytes sent: " + std::to_string(sent) +
                  " Error Number: " + std::to_string(errno)
                );
              }

              //Update totalSent
              totalSent += sent;

              //Move pointer forward by the amount sent
              currentPtr += sent;

              //Reduce remaining bytes
              remainingBytes -= sent;
            }

            std::cout << "Written bytes: " << sent << std::endl;

            fclose(htmlFilePtr);

            if(sent < 0){
              throw std::runtime_error("Couldn't write HTML" + std::to_string (sent) + "; Error Number: " + std::to_string(errno));
            }
          }  
        } else {
          throw std::runtime_error("HTML File pointer is NULL; Error Number: " + std::to_string(errno));
        }
      
      //Close client connection
      ::close(clientConnectionFd);

      //Log client info
      char ipStr[INET_ADDRSTRLEN];
      inet_ntop(client.sin_family, &client.sin_addr, ipStr, INET_ADDRSTRLEN);
      std::cout << "Current client's IP Address: "<< ipStr << std::endl;
    }
  }

  try{
    listenStatus = connectionSocket.closeSocket();
  } catch (std::runtime_error &e){
    throw;
  }

  return listenStatus;
}
