#include "TCPServer.h"

void TCPServer::createSocket() {
  m_serverSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
  if (m_serverSocket == INVALID_SOCKET) {
    throw std::runtime_error("Server: socket() failed: " +
                             std::to_string(WSAGetLastError()));
  }

  std::cout << "Server: Socket created\n";
}

void TCPServer::bindSocket() {
  serverService.sin_family = AF_INET;
  InetPton(AF_INET, L"127.0.0.1", &serverService.sin_addr.s_addr);
  serverService.sin_port = htons(k_port);
  if (bind(m_serverSocket, (SOCKADDR*)&serverService, sizeof(serverService)) ==
      SOCKET_ERROR) {
    throw std::runtime_error("Server: bind() failed: " +
                             std::to_string(WSAGetLastError()));
  }
  std::cout << "Server: Socket bound\n";
}

void TCPServer::listenOnSocket() {
  if (listen(m_serverSocket, k_maxBacklogConnections) == SOCKET_ERROR) {
    throw std::runtime_error("Server: listen() failed: " +
                             std::to_string(WSAGetLastError()));
  }

  std::cout << "Server: Listening for connections...\n";
}

void TCPServer::setupServerSocket() {
  createSocket();
  bindSocket();
  listenOnSocket();
}

void TCPServer::shutdownSocket() {
  if (shutdown(m_serverSocket, SD_SEND) == SOCKET_ERROR) {
    throw std::runtime_error("Server: shutdown() failed: " +
                             std::to_string(WSAGetLastError()));
  }
  //std::cout << "Server: Socket shutdown\n";
}

SOCKET TCPServer::acceptConnection() {
  SOCKET acceptSocket = accept(m_serverSocket, nullptr, nullptr);
  if (acceptSocket == INVALID_SOCKET) {
    throw std::runtime_error("Server: accept() failed: " +
                             std::to_string(WSAGetLastError()));
  }
  //std::cout << "Server: Connection accepted\n";
  return acceptSocket;
}
