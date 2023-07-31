#pragma once
#include <WinSock2.h>
#include <ws2tcpip.h>

#include "OrderRequest.h"

class TCPConnection {
 public:
  TCPConnection(SOCKET socket) : m_socket{socket} {}
  ~TCPConnection() {
    shutdownSocket();
    closesocket(m_socket);
  }
  void shutdownSocket();
  int sendMessage(const OrderRequest& request);
  int receiveMessage(OrderRequest& request);

 private:
  SOCKET m_socket{INVALID_SOCKET};
};
