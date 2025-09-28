#include <server.h>
#include <socket.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string>
#include <iostream>

Server::Server(){
};

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
int Server::listen(void){
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

  //Create an empty client
  sockaddr_in client;

  //Pass the empty client, as a sockadrr*, which is what "::listen()" requires
  operationStatus = connectionSocket.acceptRequests((sockaddr*) &client);

  //If couldn't open socket
  if(operationStatus < 0){
    throw std::runtime_error(
      std::string("Socket isn't able to accept requests: ") +
      std::to_string(operationStatus) + " Error Number: " +
      std::to_string(errno)
    );
  }

  //Push client to clients list
  clients.push_back(client);
  return operationStatus;
}
