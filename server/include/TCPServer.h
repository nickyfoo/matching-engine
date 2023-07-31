#pragma once
#include <WinSock2.h>
#include <ws2tcpip.h>

#include <string>

#include "OrderRequest.h"
class TCPServer {
 public:
  TCPServer() { setupServerSocket(); };

  ~TCPServer() {
    shutdownSocket();
    closesocket(m_serverSocket);
  }
  SOCKET acceptConnection();
  void shutdownSocket();

 private:
  static constexpr int k_port{55555};
  static constexpr int k_maxBacklogConnections{1000};
  SOCKET m_serverSocket{INVALID_SOCKET};
  sockaddr_in serverService{};

  void createSocket();
  void bindSocket();
  void listenOnSocket();
  void setupServerSocket();
};
