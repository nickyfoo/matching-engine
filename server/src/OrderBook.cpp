#include "OrderBook.h"
void OrderBook::transactList(std::list<Order>& orderList,
                             Order& incomingOrder) {
  while (orderList.size()) {
    auto& frontOrder = orderList.front();
    incomingOrder.transactWith(frontOrder);

    if (frontOrder.getQuantity() == 0) {
      m_orderIDs.erase(frontOrder.getOrderID());
      orderList.erase(orderList.begin());
    }
    if (incomingOrder.getQuantity() == 0) return;
  }
}

void OrderBook::addToList(std::list<Order>& orderList, Order&& incomingOrder) {
  orderList.push_back(incomingOrder);
  m_orderIDs.insert({incomingOrder.getOrderID(), --orderList.end()});
}

void OrderBook::executeBuyOrder(Order& buyOrder) {
  while (m_sell.size() && m_sell.begin()->first <= buyOrder.getPrice() &&
         buyOrder.getQuantity()) {
    auto it = m_sell.begin();

    transactList(it->second, buyOrder);
    if (it->second.empty()) {
      m_sell.erase(it);
    }
  }

  if (buyOrder.getQuantity() &&
      buyOrder.getPrice() != Order::k_buyMarketOrderPrice) {
    addToList(m_buy[buyOrder.getPrice()], std::move(buyOrder));
  }
}

void OrderBook::executeSellOrder(Order& sellOrder) {
  while (m_buy.size() && m_buy.begin()->first >= sellOrder.getPrice() &&
         sellOrder.getQuantity()) {
    auto it = m_buy.begin();

    transactList(it->second, sellOrder);
    if (it->second.empty()) {
      m_buy.erase(it);
    }
  }

  if (sellOrder.getQuantity() &&
      sellOrder.getPrice() != Order::k_sellMarketOrderPrice) {
    addToList(m_sell[sellOrder.getPrice()], std::move(sellOrder));
  }
}

void OrderBook::cancelOrder(int orderID) {
  auto it = m_orderIDs.find(orderID);
  if (it == m_orderIDs.end()) return;
  Order& order = *it->second;
  if (order.getSide() == Side::buy) {
    m_buy[order.getPrice()].erase(it->second);
  } else {
    m_sell[order.getPrice()].erase(it->second);
  }
  m_orderIDs.erase(it);
}