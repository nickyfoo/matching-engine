#pragma once
#include < WinSock2.h>
#include <stdexcept>

class WSALoader {
 public:
  WSALoader() {
    if (WSAStartup(wVersionRequested, &wsaData)) {
      throw std::runtime_error("The Winsock dll is not found!\n");
    }
    std::cout << "The Winsock dll found!\n";
    std::cout << "The status: " << wsaData.szSystemStatus << '\n';
  }

  ~WSALoader() { WSACleanup(); }

 private:
  static constexpr WORD wVersionRequested{MAKEWORD(2, 2)};
  WSADATA wsaData{};
  int wsaerr{};
};