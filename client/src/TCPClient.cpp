#include "TCPClient.h"

#include <iostream>
#include <string>

void TCPClient::createSocket() {
  m_clientSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
  if (m_clientSocket == INVALID_SOCKET) {
    throw std::runtime_error("Client: socket() failed: " +
                             std::to_string(WSAGetLastError()));
  }
  std::cout << "Client: Socket created\n";
}

void TCPClient::connectToServer() {
  m_clientService.sin_family = AF_INET;
  InetPton(AF_INET, "127.0.0.1", &m_clientService.sin_addr.s_addr);
  m_clientService.sin_port = htons(k_port);
  if (connect(m_clientSocket, (SOCKADDR*)&m_clientService,
              sizeof(m_clientService)) == SOCKET_ERROR) {
    throw std::runtime_error("Client: connect() failed: " +
                             std::to_string(WSAGetLastError()));
  }
  std::cout << "Client: Connected to server\n";
}

void TCPClient::shutdownSocket() {
  if (shutdown(m_clientSocket, SD_SEND) == SOCKET_ERROR) {
    throw std::runtime_error("Client: shutdown() failed: " +
                             std::to_string(WSAGetLastError()));
  }
  std::cout << "Client: Socket shutdown\n";
}

int TCPClient::sendMessage(const OrderRequest& request) {
  int byteCount{send(m_clientSocket, (char*)&request, sizeof(OrderRequest), 0)};
  if (byteCount == 0)
    return 0;
  else if (byteCount < 0) {
    throw std::runtime_error("Client: send() failed: " +
                             std::to_string(WSAGetLastError()));
  }
  std::cout << "Sent: " << request << '\n';
  return byteCount;
}

int TCPClient::receiveMessage(OrderRequest& request) {
  int byteCount{recv(m_clientSocket, (char*)&request, sizeof(OrderRequest), 0)};
  if (byteCount == 0)
    return 0;
  else if (byteCount < 0) {
    throw std::runtime_error("Client: recv() failed: " +
                             std::to_string(WSAGetLastError()));
  }
  std::cout << "Received: " << request << '\n';
  return byteCount;
}
