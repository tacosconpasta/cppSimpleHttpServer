#include <iostream>
#include <socket.h>
#include <sys/types.h>
#include <sys/socket.h>

//Constructor for server class
Socket::Socket(){};

//Constructor for initializing socket
Socket::Socket(int domain, int type, int protocol){
  fileDescriptor = ::socket(domain, type, protocol);
  if(fileDescriptor < 0){
    throw std::runtime_error("Error trying to create file descriptor for socket. ::socket()'s result was negative.");
  }
};

//File descriptors getters and setters
void Socket::setFileDescriptor(int fd){ 
  fileDescriptor = fd; 

  if(fileDescriptor < 0){
    throw std::runtime_error("Error trying to create file descriptor for socket. ::socket()'s result was negative.");
  }
};
int Socket::getFileDescriptor(){ return fileDescriptor; };

//Socket binder with expected errors
int Socket::bindSocket(struct sockaddr_in* address, socklen_t length){
  int socketBindingResult = bind(fileDescriptor, (struct sockaddr*) address, length);

  //If result is not 
  if(socketBindingResult == EADDRINUSE){
    throw std::runtime_error("The address is already in use.");
  }

  if(socketBindingResult == EADDRNOTAVAIL){
    throw std::runtime_error("Address is not from local machine.");
  }

  if(socketBindingResult == EINVAL){
    throw std::runtime_error("Socket already bound to an address.");
  }

  if(socketBindingResult == EROFS){
    throw std::runtime_error("Socket can't be mounted on a read-only file system.");
  }

  return socketBindingResult;
};