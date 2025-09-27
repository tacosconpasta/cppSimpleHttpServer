#pragma once
#include <socket.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

class Server {
  private:
  sockaddr_in address;
  Socket connectionSocket;
  char buffer[256];
    
  public:
  Server();

  public:
  //Setters & Getters
  void setConnectionSocket(Socket socket);
  void setAddress(sockaddr_in addr);
  Socket getConnectionSocket();
  sockaddr_in getAddress();
  char* getBuffer();

  public:
  int bindSocket();
  int listen();
};