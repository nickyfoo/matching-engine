#include <WinSock2.h>
#include <ws2tcpip.h>

#include <iostream>
#include <stdexcept>
#include <string>
#include <exception>
#include "TCPClient.h"

int main() {
  WSADATA wsaData{};
  int wsaerr{};
  WORD wVersionRequested{MAKEWORD(2, 2)};
  wsaerr = WSAStartup(wVersionRequested, &wsaData);
  if (wsaerr) {
    std::cout << "The Winsock dll not found!" << std::endl;
    return 0;
  } else {
    std::cout << "The Winsock dll found!" << std::endl;
    std::cout << "The status: " << wsaData.szSystemStatus << std::endl << '\n';
  }

  try {
    TCPClient client{};

    Data d{ 1, 2};
    client.sendMessage(d);

    Data reply{client.receiveMessage()};

  } catch (const std::runtime_error& e) {
    std::cout << "Runtime error: " << e.what() << '\n';
  }
  WSACleanup();
} 