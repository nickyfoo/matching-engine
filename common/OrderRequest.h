#pragma once
#include <stdexcept>

#include "Random.h"
#include "Types.h"
class OrderRequest {
 public:
  OrderRequest() = default;

  OrderRequest(RequestType type, int cancelledOrderID)
      : m_requestType{type}, m_cancelledOrderID{cancelledOrderID} {
    if (type != RequestType::CXL) {
      throw std::runtime_error("Invalid CXL request");
    }
    if (cancelledOrderID < 0) {
      throw std::runtime_error("Invalid CXL index");
    }
  }

  OrderRequest(RequestType type, Side side, int quantity)
      : m_requestType{type}, m_side{side}, m_quantity{quantity} {
    if (type != RequestType::SUB_MO) {
      throw std::runtime_error("Invalid SUB_MO request");
    }
    if (quantity < 0) {
      throw std::runtime_error("Invalid SUB_MO quantity");
    }
  }

  OrderRequest(RequestType type, Side side, int quantity, int price)
      : m_requestType{type},
        m_side{side},
        m_quantity{quantity},
        m_price{price} {
    if (type != RequestType::SUB_LO) {
      throw std::runtime_error("Invalid SUB_LO request");
    }
    if (quantity < 0) {
      throw std::runtime_error("Invalid SUB_LO quantity");
    }
    if (price < 0) {
      throw std::runtime_error("Invalid SUB_LO price");
    }
  }
  RequestType getType() const { return m_requestType; }
  const Side getSide() const { return m_side; }
  const int getPrice() const { return m_price; }
  const int getQuantity() const { return m_quantity; }
  const int getOrderID() const { return m_orderID; }
  const int getCancelledOrderID() const { return m_cancelledOrderID; }

  void setOrderID(int orderID) { m_orderID = orderID; }
  bool validate() const {
    switch (m_requestType) {
      case RequestType::CXL:
        return m_cancelledOrderID != -1;
      case RequestType::SUB_MO:
        return m_quantity != -1;
      case RequestType::SUB_LO:
        return m_quantity != -1 && m_price != -1;
      default:
        return false;
    }
  }

  static OrderRequest getRandomOrderRequest() {
    RequestType type{
        Random::get(0, static_cast<int>(RequestType::MAX_TYPE_NUM) - 1)};
    switch (type) {
      case RequestType::CXL:
        return OrderRequest{type, Random::get(1, 10000)};
      case RequestType::SUB_MO:
        return OrderRequest{type,
                            static_cast<Side>(Random::get(
                                1, static_cast<int>(Side::maxSide) - 1)),
                            Random::get(1, 100)};
      case RequestType::SUB_LO:
        return OrderRequest{type,
                            static_cast<Side>(Random::get(
                                1, static_cast<int>(Side::maxSide) - 1)),
                            Random::get(1, 100), Random::get(1, 100)};
    }
  }

 private:
  const RequestType m_requestType{};
  const Side m_side{};
  const int m_quantity{-1};
  const int m_price{-1};
  const int m_cancelledOrderID{-1};
  int m_orderID{-1};
};

inline std::ostream& operator<<(std::ostream& out,
                                const OrderRequest& request) {
  out << request.getType() << ' ' << request.getSide() << ' '
      << request.getQuantity() << ' ' << request.getPrice() << ' '
      << request.getCancelledOrderID() << ' ' << request.getOrderID();
  return out;
}
