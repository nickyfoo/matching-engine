#pragma once
#include <iostream>
enum class Side { buy, sell, maxSide };


inline std::ostream& operator<<(std::ostream& out, const Side& side) {
  using enum Side;
  switch (side) {
    case buy:
      out << "BUY";
      return out;
    case sell:
      out << "SELL";
      return out;
    default:
      out << "???";
      return out;
  }
}

enum class RequestType {
  SUB_LO,
  SUB_MO,
  CXL,
  MAX_TYPE_NUM

};

inline std::ostream& operator<<(std::ostream& out, const RequestType& requestType) {
    using enum RequestType;
  switch (requestType) {
    case SUB_LO:
      out << "SUB_LO";
      return out;
    case SUB_MO:
      out << "SUB_MO";
      return out;
    case CXL:
      out << "CXL";
      return out;
    default:
      out << "???";
      return out;
  }
}