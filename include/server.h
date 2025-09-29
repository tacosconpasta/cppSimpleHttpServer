#pragma once
#include <socket.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <list>
#include <atomic>

static constexpr size_t BUFFER_SIZE = 8192;

class Server {
  private:
  sockaddr_in address;
  Socket connectionSocket;
  std::list<sockaddr_in> clients;
  char buffer[BUFFER_SIZE + 1];
    
  public:
  Server();
  ~Server();

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
  int listen(std::atomic<bool> &stopServerFlag);
};