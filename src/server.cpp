#include <server.h>
#include <socket.h>
#include <string>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

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

char* Server::getBuffer(){
  return buffer;
};

//Functional Methods

//Calls bind socket
int Server::bindSocket(void){
  return connectionSocket.bindSocket(&address, sizeof(address));
}

int Server::listen(void){
  return connectionSocket.openSocket();
}