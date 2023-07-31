#include <WinSock2.h>
#include <ws2tcpip.h>

#include <iostream>
#include <sstream>
#include <string_view>
#include <vector>

#include "Order.h"
#include "OrderBook.h"
#include "OrderBookTest.h"
#include "OrderDispatcherTest.h"
#include "TCPServer.h"
#include "WSALoader.h"

int main() {
  OrderBookTest test{};
  OrderDispatcherTest test2{};
  WSALoader wsaLoader{};

  try {
    TCPServer server{};
    OrderBook orderBook{};
    OrderDispatcher dispatcher{orderBook};
    int byteCount{};
    do {
      OrderRequest request{};
      byteCount = server.receiveMessage(request);
      if (byteCount > 0) {
        std::cout << "Received " << byteCount << " bytes\n";
        dispatcher.dispatch(request);
        std::cout << orderBook << '\n';
        server.sendMessage(request);
      } else if (byteCount == 0) {
        std::cout << "Closing connection\n";
      }
    } while (byteCount > 0);
    server.shutdownSocket();
  } catch (const std::runtime_error& e) {
    std::cout << "Runtime error: " << e.what() << '\n';
  }
}