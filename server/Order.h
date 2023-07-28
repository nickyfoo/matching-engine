#pragma once
#include <iostream>
class Order {
 public:
  Order(char side, int orderID, int quantity, int price)
      : m_side{side},
        m_orderID{orderID},
        m_quantity{quantity},
        m_price{price} {}

  friend std::ostream& operator<<(std::ostream& out, const Order& orderBook) {
    out << orderBook.m_quantity << "@" << orderBook.m_price << "#"
        << orderBook.m_orderID;
    return out;
  }
  // private:
  static constexpr int k_buyMarketOrderPrice{INT_MAX};
  static constexpr int k_sellMarketOrderPrice{INT_MIN};
  char m_side{};
  int m_orderID{};
  int m_quantity{};
  int m_price{};
};
