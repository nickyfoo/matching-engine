#include "TCPConnection.h"
#include <string>
#include <iostream>

void TCPConnection::shutdownSocket() {
  if (shutdown(m_socket, SD_SEND) == SOCKET_ERROR) {
    throw std::runtime_error("Server: shutdown() failed: " +
                             std::to_string(WSAGetLastError()));
  }
  //std::cout << "Server: Socket shutdown\n";
}

int TCPConnection::sendMessage(const OrderRequest& request) {
  int byteCount{send(m_socket, (char*)&request, sizeof(OrderRequest), 0)};
  if (byteCount == 0) return 0;
  if (byteCount < 0) {
    throw std::runtime_error("Server: send() failed: " +
                             std::to_string(WSAGetLastError()));
  }
  //std::cout << "Sent: " << request << '\n';
  return byteCount;
}

int TCPConnection::receiveMessage(OrderRequest& request) {
  int byteCount{recv(m_socket, (char*)&request, sizeof(OrderRequest), 0)};
  if (byteCount == 0)
    return 0;
  else if (byteCount < 0) {
    throw std::runtime_error("Server: recv() failed: " +
                             std::to_string(WSAGetLastError()));
  }
  //std::cout << "Received: " << request << '\n';
  return byteCount;
}