#include "TCPClient.h"
#include <iostream>
#include "Data.h"

void TCPClient::createSocket() {
  m_clientSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
  if (m_clientSocket == INVALID_SOCKET) {
    throw std::runtime_error("Client: socket() failed: " + std::to_string(WSAGetLastError()));
  }
  std::cout << "Client: Socket created\n";
}

void TCPClient::connectToServer() {
  m_clientService.sin_family = AF_INET;
  InetPton(AF_INET, "127.0.0.1", &m_clientService.sin_addr.s_addr);
  m_clientService.sin_port = htons(k_port);
  if (connect(m_clientSocket, (SOCKADDR*)&m_clientService,
                sizeof(m_clientService)) == SOCKET_ERROR) {
    throw std::runtime_error("Client: connect() failed: " + std::to_string(WSAGetLastError()));
  }
  std::cout << "Client: Connected to server\n";
}

void TCPClient::sendMessage(const Data& data){

  int byteCount{send(m_clientSocket, (char*)&data, sizeof(Data), 0)};
  if(byteCount <=0) {
    throw std::runtime_error("Client: sendMessage() failed");
  }
  std::cout << "Sent: " << data << '\n';
}

Data TCPClient::receiveMessage() {
  Data data{};
  int byteCount{recv(m_clientSocket, (char*)&data, sizeof(Data), 0)};
  if (byteCount <= 0) {
    throw std::runtime_error("Client: receiveMessage() failed");
  }
 std::cout << "Received: " << data << '\n';
  return data;
}
