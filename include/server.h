#pragma once
#include <socket.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <list>

class Server {
  private:
  sockaddr_in address;
  Socket connectionSocket;
  std::list<sockaddr_in> clients;
  char buffer[256];
    
  public:
  Server();

  public:
  //Setters & Getters
  void setConnectionSocket(Socket socket);
  void setAddress(sockaddr_in addr);
  Socket getConnectionSocket();
  sockaddr_in getAddress();
  std::list<sockaddr_in> getClients();
  char* getBuffer();

  public:
  int bindSocket();
  int listen();
};