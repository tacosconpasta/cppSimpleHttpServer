#include <iostream>
#include <socket.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>

//Constructor for server class
Socket::Socket(): fileDescriptor(-1){};

//Constructor for initializing socket
Socket::Socket(int domain, int type, int protocol){
  fileDescriptor = ::socket(domain, type, protocol);
  if(fileDescriptor < 0){
    throw std::runtime_error("Error trying to create file descriptor for socket. ::socket()'s result was negative.");
  }
};

//File descriptors getters and setters
void Socket::setFileDescriptor(int fd){ 
  if(fd < 0){
    throw std::runtime_error("Error trying to create file descriptor for socket. Provided file descriptor was negative.");
  }
  fileDescriptor = fd; 
};

int Socket::getFileDescriptor(){ return fileDescriptor; };

//Socket binder with expected errors
int Socket::bindSocket(sockaddr_in* address, socklen_t length){
  int socketBindingResult = bind(fileDescriptor, (sockaddr*) address, length);
  return socketBindingResult;
};

int Socket::openSocket(void){
  return ::listen(fileDescriptor, 10);
}

int Socket::acceptRequests(sockaddr* user) {
  socklen_t len = sizeof(*user);
  return ::accept(fileDescriptor, user, &len);
}

int Socket::closeSocket() {
  //If fileDescriptor is not closed
  if (fileDescriptor >= 0) {
    //If result is less than 0
    if (::close(fileDescriptor) < 0) {
      throw std::runtime_error("Failed to close socket: " + std::to_string(errno));
    }

    fileDescriptor = -1;
  }
  return 0;
}