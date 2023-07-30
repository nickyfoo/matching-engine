#pragma once
#include <WinSock2.h>
#include <ws2tcpip.h>

#include <string>

#include "OrderRequest.h"
class TCPServer {
 public:
  TCPServer() {
    setupSocket();
    acceptConnection();
  };

  ~TCPServer() {
    closesocket(m_serverSocket);
    closesocket(m_acceptSocket);
  }
  void sendMessage(const OrderRequest& request);
  OrderRequest receiveMessage();

 private:
  static constexpr int k_port{55555};
  SOCKET m_serverSocket{INVALID_SOCKET};
  SOCKET m_acceptSocket{INVALID_SOCKET};
  sockaddr_in serverService{};

  void createSocket();
  void bindSocket();
  void listenOnSocket();
  void setupSocket();
  void acceptConnection();
};
