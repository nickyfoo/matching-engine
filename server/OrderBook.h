#pragma once
#include <algorithm>
#include <iostream>
#include <list>
#include <map>
#include <unordered_map>

#include "Order.h"

class OrderBook {
 public:
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

  void transactList(std::list<Order>& orderList, Order& incomingOrder) {
    while (orderList.size()) {
      auto& frontOrder = orderList.front();
      int quantityTransacted =
          std::min<>(frontOrder.m_quantity, incomingOrder.m_quantity);
      frontOrder.m_quantity -= quantityTransacted;
      incomingOrder.m_quantity -= quantityTransacted;

      if (frontOrder.m_quantity == 0) {
        m_orderIDs.erase(frontOrder.m_orderID);
        orderList.erase(orderList.begin());
      }
      if (incomingOrder.m_quantity == 0) return;
    }
  }

  void addToList(std::list<Order>& orderList, Order& incomingOrder) {
    orderList.push_back(incomingOrder);
    m_orderIDs.insert(
        {incomingOrder.m_orderID, {incomingOrder, --orderList.end()}});
  }

  void buyOrder(Order& buyOrder) {
    while (m_sell.size() && m_sell.begin()->first <= buyOrder.m_price &&
           buyOrder.m_quantity) {
      auto it = m_sell.begin();

      transactList(it->second, buyOrder);
      if (it->second.empty()) {
        m_sell.erase(it);
      }
    }

    if (buyOrder.m_quantity &&
        buyOrder.m_price != Order::k_buyMarketOrderPrice) {
      addToList(m_buy[buyOrder.m_price], buyOrder);
    }
  }

  void sellOrder(Order& sellOrder) {
    while (m_buy.size() && m_buy.begin()->first >= sellOrder.m_price &&
           sellOrder.m_quantity) {
      auto it = m_buy.begin();

      transactList(it->second, sellOrder);
      if (it->second.empty()) {
        m_buy.erase(it);
      }
    }

    if (sellOrder.m_quantity &&
        sellOrder.m_price != Order::k_sellMarketOrderPrice) {
      addToList(m_sell[sellOrder.m_price], sellOrder);
    }
  }

  void cancelOrder(int orderID) {
    auto it = m_orderIDs.find(orderID);
    if (it == m_orderIDs.end()) return;
    Order& order = it->second.first;
    if(order.m_side == 'B'){
      m_buy[order.m_price].erase(it->second.second);
    } else {
      m_sell[order.m_price].erase(it->second.second);
    }
    m_orderIDs.erase(it);
  }

 private:
  std::map<int, std::list<Order>, std::greater<>> m_buy;
  std::map<int, std::list<Order>> m_sell;
  std::unordered_map<int, std::pair<Order&, std::list<Order>::iterator>> m_orderIDs;
};
