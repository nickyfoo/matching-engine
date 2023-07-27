#pragma once
#include <WinSock2.h>
#include <ws2tcpip.h>

#include <stdexcept>
#include "Data.h"
class TCPClient {
 public:
  TCPClient() {
    createSocket();
    connectToServer();
  };
  void sendMessage(const Data& data);
  Data receiveMessage();

 private:
  static constexpr int k_port{55555};
  static constexpr std::size_t k_bufferSize{200};
  SOCKET m_clientSocket{INVALID_SOCKET};
  sockaddr_in m_clientService{};
  char m_buffer[k_bufferSize]{};

  void createSocket();
  void connectToServer();
};