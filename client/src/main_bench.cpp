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
  }
}

int main(int argc, char* argv[]) {
  WSALoader wsaLoader{};
  int numThreads = 1000;
  int numOrders = 100;
  if (argc > 1) numThreads = std::stoi(argv[1]);
  if (argc > 2) numOrders = std::stoi(argv[2]);

  std::atomic<int> failCounter{0};
  {
    Timer timer;
    std::vector<std::jthread> threads;
    threads.reserve(numThreads);
    for (int i = 0; i < numThreads; i++) {
      threads.push_back(std::jthread(sendOrders, std::ref(failCounter), numOrders));
    }
  }
  std::cout << "Threads: " << numThreads << " | Orders/Thread: " << numOrders << " | Fails: " << failCounter.load() << std::endl;
}
