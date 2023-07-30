#pragma once
#include <stdexcept>
#include "Types.h"
class OrderRequest {
 public:
  OrderRequest(RequestType type, int orderID)
      : m_requestType{type}, m_orderID{orderID} {
    if (type != RequestType::CXL) {
      throw std::runtime_error("Invalid CXL request");
    }
  }

  OrderRequest(RequestType type, Side side, int quantity)
      : m_requestType{type}, m_side{side}, m_quantity{quantity} {
    if (type != RequestType::SUB_MO) {
      throw std::runtime_error("Invalid SUB_MO request");
    }
  }

  OrderRequest(RequestType type, Side side, int quantity, int price)
      : m_requestType{type},
        m_side{side},
        m_quantity{quantity},
        m_price{price}{
    if (type != RequestType::SUB_LO) {
      throw std::runtime_error("Invalid SUB_LO request");
    }
  }
  RequestType getType() const { return m_requestType; }
  const Side getSide() const { return m_side; }
  const int getPrice() const { return m_price; }
  const int getQuantity() const { return m_quantity; }
  const int getOrderID() const { return m_orderID; }

 private:
  RequestType m_requestType{};
  Side m_side{};
  int m_quantity{-1};
  int m_price{-1};
  int m_orderID{-1};
};
