#include <server.h>
#include <socket.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <errno.h>

#include <iostream>
#include <string>
#include <cmath>
#include <list>
#include <arpa/inet.h>

int error(const char* msg){
  perror(msg);
  exit(1);
}

int main(int argc, char *argv[]){
  int portNumber;

  //If user doesnt input port number
  if(argc < 2){
    std::cerr << "Argument count is less than 2, PORT_NUMBER was NOT provided." << std::endl;
    return 1;
  }

  //If user inputs too many arguments
  if(argc > 2){
    std::cerr << "Too many arguments were provided." << std::endl;
    return 1;
  }

  //Parse portNumber parameter for checking
  portNumber = atoi(argv[1]);
  int digits = (portNumber == 0) ? 1 : (int)std::log10(std::abs(portNumber)) + 1;

  //If PORT is not safe for "toying around" return; CHANGE THIS LINE AT OWN EXPENSE
  if (digits != 4 || portNumber < 1024 || portNumber > 9999) {
    std::cerr << "Port Number is not safe for testing" << std::endl;
    return 1;
  }

  Socket socket;
  Server server;

  //Set the file descriptor for the socket.
  try{
    int fd = ::socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    socket.setFileDescriptor(fd);
  } catch (std::runtime_error &e){
    std::cerr << "File Descriptor Exception: " << e.what() << std::endl;
    return 1;
  }

  if(socket.getFileDescriptor() > 0) {
    server.setConnectionSocket(socket);
    std::cout << "Socket's Fd: " << server.getConnectionSocket().getFileDescriptor() << std::endl;
  }

  //Structure that holds server address and port
  struct sockaddr_in serverAddress;

  //Convert port from "host byte order" => "network byte order"
  int portNumberNetByteOrder = htons(portNumber);

  //Set up the serverAddress
  serverAddress.sin_family = AF_INET;
  serverAddress.sin_addr.s_addr = htonl(INADDR_ANY);
  serverAddress.sin_port = portNumberNetByteOrder;

  //Set address to server
  server.setAddress(serverAddress);

  //Print address details
  sockaddr_in address = server.getAddress();

  //Get IP address into a string
  char ipStr[INET_ADDRSTRLEN];
  inet_ntop(AF_INET, &address.sin_addr, ipStr, INET_ADDRSTRLEN);

  std::cout << "Listening IP Address: " << ipStr << " Port#: " << ntohs(address.sin_port) << std::endl;

  //Bind socket & print result of binding operation
  try {
    int bindingResult = server.bindSocket();
    std::cout << "Binding result: " << bindingResult << std::endl;

  } catch (std::runtime_error &e){
    std::cerr << "\nSocket bind exception: \n" << e.what() << '\n' << std::endl;
    return 1;
  }

  try{
    int listeningState = server.listen();
    std::cout << "Listening result: " << listeningState << std::endl;
  } catch (std::runtime_error &e){
    std::cerr << "\nSocket listening exception: \n" << e.what() << '\n' << std::endl;
    return 1;
  }

  //Print ALL clients after ending execution
  std::list<sockaddr_in> clients = server.getClients();

  int clientNumber = 0;
  for(sockaddr_in client : clients){
    //Get IP address into a string
    char ipStr[INET_ADDRSTRLEN];
    inet_ntop(client.sin_family, &client.sin_addr, ipStr, INET_ADDRSTRLEN);

    std::cout << "Client #" << clientNumber << "'s IP Address: "<< ipStr << std::endl;
  }

  return 0;
}