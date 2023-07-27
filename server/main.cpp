#include <WinSock2.h>
#include <ws2tcpip.h>

#include <iostream>
#include "Data.h"
#include "TCPServer.h"
#include "WSALoader.h";
int main() {
  WSALoader wsaLoader{};

  try {
    TCPServer server{}; 
    Data d{server.receiveMessage()};

    d.m_x *= 2;
    d.m_y *= 2;
    server.sendMessage(d);

  } catch (const std::runtime_error& e){
    std::cout << "Runtime error: " << e.what() << '\n';
  }
}