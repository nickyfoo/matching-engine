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
#include "ThreadPool.h"

void handleConnection(SOCKET clientSocket, OrderDispatcher& dispatcher,
                      OrderBook& orderBook) {
  TCPConnection connection{clientSocket};
  int byteCount{};
  do {
    OrderRequest request{};
    byteCount = connection.receiveMessage(request);
    if (byteCount > 0) {
      dispatcher.dispatch(request);
      connection.sendMessage(request);
    }
  } while (byteCount > 0);
}

int main(int argc, char* argv[]) {
  OrderBookTest test{};
  OrderDispatcherTest test2{};
  WSALoader wsaLoader{};

  size_t poolSize = std::thread::hardware_concurrency();
  if (argc > 1) {
    poolSize = std::stoi(argv[1]);
  }
  std::cout << "Starting server with ThreadPool size: " << poolSize << '\n';

  try {
    TCPServer server{};
    OrderBook orderBook{};
    OrderDispatcher dispatcher{orderBook};
    ThreadPool pool{poolSize};

    while (true) {
      SOCKET clientSocket = server.acceptConnection();
      pool.enqueue([clientSocket, &dispatcher, &orderBook]() {
        handleConnection(clientSocket, dispatcher, orderBook);
      });
    }
  } catch (const std::runtime_error& e) {
    std::cout << "Runtime error: " << e.what() << '\n';
  }
}
