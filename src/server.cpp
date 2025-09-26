#include <server.h>
#include <socket.h>
#include <string>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

Server::Server(){
};

void Server::setConnectionSocket(Socket socket){
  connectionSocket = socket;
};

int Server::getPortNumber(){
  return portNumber;
};

Socket Server::getConnectionSocket(){
  return connectionSocket;
};

char* Server::getBuffer(){
  return buffer;
};