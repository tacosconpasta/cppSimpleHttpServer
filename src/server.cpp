#include <server.h>
#include <socket.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string>
#include <iostream>
#include <atomic>

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
  int operationStatus;
  operationStatus = connectionSocket.openSocket();

  //If couldn't open socket
  if(operationStatus < 0){
    throw std::runtime_error(
      std::string("Couldn't make the socket listen correctly: ") +
      std::to_string(operationStatus) + " Error Number: " +
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

    //If couldn't accept the request
    if(clientConnectionFd < 0){
      //If stop server
      if (stopServerFlag)
        break;

      //Else, throw an exception
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

      //Log client info
      char ipStr[INET_ADDRSTRLEN];
      inet_ntop(client.sin_family, &client.sin_addr, ipStr, INET_ADDRSTRLEN);
      std::cout << "Current client's IP Address: "<< ipStr << std::endl;
    }

  }

  try{
    operationStatus = connectionSocket.closeSocket();
  } catch (std::runtime_error &e){
    throw;
  }

  return operationStatus;
}
