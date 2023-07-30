#pragma once
#include <algorithm>
#include <iostream>
#include <list>
#include <map>
#include <unordered_map>

#include "Order.h"

class OrderBook {
 public:
  void executeBuyOrder(Order& buyOrder);
  void executeSellOrder(Order& sellOrder);
  void cancelOrder(int orderID);

  friend std::ostream& operator<<(std::ostream& out,
                                  const OrderBook& orderBook) {
    out << "Buy : ";
    for (auto& [price, orders] : orderBook.m_buy) {
      for (auto& order : orders) {
        out << order << ' ';
      }
    }
    out << '\n';
    out << "Sell: ";
    for (auto& [price, orders] : orderBook.m_sell) {
      for (auto& order : orders) {
        out << order << ' ';
      }
    }
    out << '\n';
    return out;
  }


 private:
  std::map<int, std::list<Order>, std::greater<>> m_buy;
  std::map<int, std::list<Order>> m_sell;
  std::unordered_map<int, std::list<Order>::iterator> m_orderIDs;

  void transactList(std::list<Order>& orderList, Order& incomingOrder);
  void addToList(std::list<Order>& orderList, Order&& incomingOrder);

};
