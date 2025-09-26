#include <server.h>
#include <socket.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

#include <iostream>
#include <string>

int error(const char* msg){
  perror(msg);
  exit(1);
}

int main (void){
  Socket socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
  Server server;
  
  server.setConnectionSocket(socket);

  Socket receivingSocket = server.getConnectionSocket();

  std::cout << receivingSocket.getFileDescriptor() << std::endl;

  return 0;
}