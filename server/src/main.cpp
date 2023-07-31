#include <WinSock2.h>
#include <ws2tcpip.h>

#include <iostream>
#include <sstream>
#include <string_view>
#include <thread>
#include <vector>

#include "Order.h"
#include "OrderBook.h"
#include "OrderBookTest.h"
#include "OrderDispatcherTest.h"
#include "TCPConnection.h"
#include "TCPServer.h"
#include "WSALoader.h"

void handleConnection(SOCKET clientSocket, OrderDispatcher& dispatcher,
                      OrderBook& orderBook) {
  TCPConnection connection{clientSocket};
  int byteCount{};
  do {
    OrderRequest request{};
    byteCount = connection.receiveMessage(request);
    if (byteCount > 0) {
      //std::cout << "Received " << byteCount << " bytes\n";
      dispatcher.dispatch(request);
      connection.sendMessage(request);
    } else if (byteCount == 0) {
      //std::cout << "Closing connection\n";
    }
  } while (byteCount > 0);
}

int main() {
  OrderBookTest test{};
  OrderDispatcherTest test2{};
  WSALoader wsaLoader{};

  try {
    TCPServer server{};
    OrderBook orderBook{};
    OrderDispatcher dispatcher{orderBook};
    std::vector<std::jthread> threads;
    while (true) {
      SOCKET clientSocket = server.acceptConnection();
      threads.push_back(std::jthread(handleConnection, clientSocket,
                                     std::ref(dispatcher),
                                     std::ref(orderBook)));
    }
    std::cout << "Total threads spawned: " << threads.size() << '\n';
    std::cout << orderBook << '\n';
  } catch (const std::runtime_error& e) {
    std::cout << "Runtime error: " << e.what() << '\n';
  }
}