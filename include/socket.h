#pragma once
#include <socket.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

class Socket {
  private: 
    int fileDescriptor;

  public:
  Socket();
  Socket(int domain, int type, int protocol);

  public:
    void setFileDescriptor(int fd);
    int getFileDescriptor();
    int bindSocket(sockaddr_in* address, socklen_t length);
    int openSocket();
    int acceptRequests(sockaddr* user);
};