#include <WinSock2.h>
#include <ws2tcpip.h>

#include <exception>
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>

#include "TCPClient.h"
#include "WSALoader.h"

int main() {
  WSALoader wsaLoader{};

  try {
    TCPClient client{};
    std::vector<OrderRequest> orders{
        OrderRequest(RequestType::SUB_LO, Side::buy, 100, 10),
        OrderRequest(RequestType::SUB_LO, Side::buy, 100, 11),
        OrderRequest(RequestType::SUB_LO, Side::buy, 100, 11),
        OrderRequest(RequestType::SUB_LO, Side::buy, 100, 11),
        OrderRequest(RequestType::SUB_LO, Side::buy, 100, 12),
        OrderRequest(RequestType::SUB_LO, Side::sell, 50, 10),
        OrderRequest(RequestType::SUB_LO, Side::sell, 100, 12),
        OrderRequest(RequestType::CXL, 3)};
    for (auto& order : orders) {
      client.sendMessage(order);
      OrderRequest reply{client.receiveMessage()};
    }

  } catch (const std::runtime_error& e) {
    std::cout << "Runtime error: " << e.what() << '\n';
  }
}