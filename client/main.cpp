#include <WinSock2.h>
#include <ws2tcpip.h>

#include <iostream>
#include <stdexcept>
#include <string>
#include <exception>
#include "TCPClient.h"
#include "WSALoader.h";

int main() {
  WSALoader wsaLoader{};

  try {
    TCPClient client{};

    Data d{ 1, 2};
    client.sendMessage(d);

    Data reply{client.receiveMessage()};

  } catch (const std::runtime_error& e) {
    std::cout << "Runtime error: " << e.what() << '\n';
  }
} 