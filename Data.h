#pragma once
#include <string>
#include <string_view>
#include <iostream>

class Data {
 public:
  int m_x{};
  int m_y{};

  Data(int x = 0, int y = 0) : m_x{x}, m_y{y} {};

  friend std::ostream& operator<<(std::ostream& out, const Data& data){
    out << "Data {" << data.m_x << ", " << data.m_y << "}";
    return out;
  }
};