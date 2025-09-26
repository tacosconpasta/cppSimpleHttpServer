#pragma once
#include <socket.h>

class Server {
  private:
  int portNumber;
  Socket connectionSocket;
  char buffer[256];
    
  public:
  Server();

  public:
  void setConnectionSocket(Socket socket);
  int getPortNumber();
  Socket getConnectionSocket();
  char* getBuffer();
};