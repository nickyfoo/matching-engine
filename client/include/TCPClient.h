#pragma once
#include <WinSock2.h>
#include <ws2tcpip.h>

#include <stdexcept>
#include "OrderRequest.h"

class TCPClient {
 public:
  TCPClient() {
    createSocket();
    connectToServer();
  };

  ~TCPClient() {
    closesocket(m_clientSocket);
  }

  void shutdownSocket();
  int sendMessage(const OrderRequest& request);
  int receiveMessage(OrderRequest& request);
  void shutdownClient();

 private:
  static constexpr int k_port{55555};
  static constexpr std::size_t k_bufferSize{200};
  SOCKET m_clientSocket{INVALID_SOCKET};
  sockaddr_in m_clientService{};
  char m_buffer[k_bufferSize]{};

  void createSocket();
  void connectToServer();
};