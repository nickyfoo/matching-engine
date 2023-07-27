#include "TCPServer.h"

void TCPServer::createSocket(){
  m_serverSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
  if (m_serverSocket == INVALID_SOCKET) {
    throw std::runtime_error("Server: socket() failed: " +
                             std::to_string(WSAGetLastError()));
  }

  std::cout << "Server: Socket created\n";
}

void TCPServer::bindSocket(){
  serverService.sin_family = AF_INET;
  InetPton(AF_INET, "127.0.0.1", &serverService.sin_addr.s_addr);
  serverService.sin_port = htons(k_port);
  if (bind(m_serverSocket, (SOCKADDR*)&serverService, sizeof(serverService)) ==
      SOCKET_ERROR) {
    throw std::runtime_error("Server: bind() failed: " +
                             std::to_string(WSAGetLastError()));
  }
  std::cout << "Server: Socket bound\n";
}

void TCPServer::listenOnSocket(){
  if (listen(m_serverSocket, 1) == SOCKET_ERROR) {
    throw std::runtime_error("Server: listen() failed: " +
                             std::to_string(WSAGetLastError()));
  }

  std::cout << "Server: Listening for connections...\n";
}

void TCPServer::setupSocket() {
  createSocket();
  bindSocket();
  listenOnSocket();
}

void TCPServer::acceptConnection(){
  m_acceptSocket = accept(m_serverSocket, nullptr, nullptr);
  if (m_acceptSocket == INVALID_SOCKET) {
    throw std::runtime_error("Server: accept() failed: " +
                             std::to_string(WSAGetLastError()));
  }
  std::cout << "Server: Connection accepted\n";
}

void TCPServer::sendMessage(const Data& data){
  int byteCount{send(m_acceptSocket, (char*)&data, sizeof(Data), 0)};
  if(byteCount<=0) {
    throw std::runtime_error("Server: sendMessage() failed");
  }
  std::cout << "Sent: " << data << '\n';
}

Data TCPServer::receiveMessage() {
  Data data{};
  int byteCount{recv(m_acceptSocket, (char*)&data, sizeof(Data), 0)};
  if (byteCount <= 0) {
    throw std::runtime_error("Client: receiveMessage() failed");
  }
  std::cout << "Received: " << data << '\n';
  return data;
}