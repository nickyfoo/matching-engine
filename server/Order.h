#pragma once
#include <iostream>

#include "OrderRequest.h"
#include "Types.h"

class Order {
 public:
  static constexpr int k_buyMarketOrderPrice{INT_MAX};
  static constexpr int k_sellMarketOrderPrice{INT_MIN};

  Order(Side side, int orderID, int quantity, int price)
      : m_side{side},
        m_orderID{orderID},
        m_quantity{quantity},
        m_price{price} {}

  Order(const OrderRequest& request, int orderID)
      : m_side{request.getSide()},
        m_orderID{request.getOrderID()},
        m_quantity{request.getQuantity()},
        m_price{request.getPrice()} {
    if (request.getOrderID() == -1) m_orderID = orderID;
    if (request.getType() == RequestType::SUB_MO) {
      m_price = request.getSide() == Side::buy ? k_buyMarketOrderPrice
                                               : k_sellMarketOrderPrice;
    }
  }

  friend std::ostream& operator<<(std::ostream& out, const Order& orderBook) {
    out << orderBook.m_quantity << "@" << orderBook.m_price << "#"
        << orderBook.m_orderID;
    return out;
  }

  const Side getSide() const { return m_side; }
  const int getOrderID() const { return m_orderID; }
  const int getPrice() const { return m_price; }
  const int getQuantity() const { return m_quantity; }

  void transactWith(Order& other);

 private:
  Side m_side{};
  int m_orderID{};
  int m_quantity{};
  int m_price{};
};
