#include <WinSock2.h>
#include <ws2tcpip.h>

#include <exception>
#include <iostream>
#include <stdexcept>
#include <string>
#include <thread>
#include <utility>
#include <vector>

#include "TCPClient.h"
#include "Timer.h"
#include "WSALoader.h"


void sendOrders(std::atomic<int>& failCounter, int numOrders) {
  try {
    TCPClient client{};
    OrderRequest reply{};
    for (int _ = 0; _ < numOrders; _++) {
      OrderRequest order{OrderRequest::getRandomOrderRequest()};
      client.sendMessage(order);
      client.receiveMessage(reply);
    }
    client.shutdownClient();
  } catch (const std::runtime_error& e) {
    failCounter++;
    std::cout << "Runtime error: " << e.what() << '\n';
  }
}
int main() {
  WSALoader wsaLoader{};
  Timer timer;
  std::atomic<int> failCounter{0};
  constexpr int totalOrders{100000};
  constexpr int numThreads{1000};
  constexpr int numOrders{totalOrders / numThreads};
  {
    std::vector<std::jthread> threads;

    for (int i = 0; i < numThreads; i++) {
      threads.push_back(std::jthread(sendOrders, std::ref(failCounter), numOrders));
    }
  }
  std::cout << "numThreads: " << numThreads << '\n';
  std::cout << "numOrders per thread: " << numOrders << '\n';
  std::cout << "failCounter: " << failCounter << '\n';
}