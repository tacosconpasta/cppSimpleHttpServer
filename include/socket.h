#pragma once
#include <sys/socket.h>

class Socket {
  private: 
    int fileDescriptor;

  public:
  Socket();
  Socket(int domain, int type, int protocol);

  public:
    void setFileDescriptor(int fd);
    int getFileDescriptor();
    int bindSocket(struct sockaddr_in* address, socklen_t length);
};