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

  Order(const OrderRequest& request)
      : m_side{request.getSide()},
        m_orderID{request.getOrderID()},
        m_quantity{request.getQuantity()},
        m_price{setPrice(request)} {}

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
  const Side m_side{};
  const int m_orderID{};
  int m_quantity{};
  const int m_price{};

  int setPrice(const OrderRequest& request) {
    if (request.getType() == RequestType::SUB_MO) {
      return (request.getSide() == Side::buy) ? k_buyMarketOrderPrice
                                              : k_sellMarketOrderPrice;
    } else {
      return request.getPrice();
    }
  }
};
