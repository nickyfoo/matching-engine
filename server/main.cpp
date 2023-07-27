#include <WinSock2.h>
#include <ws2tcpip.h>

#include <iostream>
#include "Data.h"
#include "TCPServer.h"
int main() {
  WSADATA wsaData;
  int wsaerr;
  WORD wVersionRequested = MAKEWORD(2, 2);
  wsaerr = WSAStartup(wVersionRequested, &wsaData);
  if (wsaerr) {
    std::cout << "The Winsock dll not found!" << std::endl;
    return 0;
  } else {
    std::cout << "The Winsock dll found!" << std::endl;
    std::cout << "The status: " << wsaData.szSystemStatus << std::endl << '\n';
  }

  try {
    TCPServer server{}; 
    Data d{server.receiveMessage()};

    d.m_x *= 2;
    d.m_y *= 2;
    server.sendMessage(d);

  } catch (const std::runtime_error& e){
    std::cout << "Runtime error: " << e.what() << '\n';
  }
  WSACleanup();
}