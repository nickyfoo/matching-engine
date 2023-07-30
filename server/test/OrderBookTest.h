#pragma once
#include <iostream>
#include <sstream>

#include "OrderBook.h"
class OrderBookTest {
 public:
  int m_testCount{6};
  int m_passCount{};
  OrderBookTest() {
    suite();
    std::cout << "OrderBookTest: " << m_passCount << "/" << m_testCount << '\n';
  }

  bool validate(const OrderBook& orderBook, std::string_view ans) {
    std::stringstream ss;
    ss << orderBook;
    return ss.str() == ans;
  }

  bool testOrderBook_LimitOrder_LeftoverOrders() {
    OrderBook orderBook{};
    std::vector<Order> orders{
        Order(Side::buy, 1, 100, 10),
        Order(Side::buy, 2, 100, 11),
        Order(Side::buy, 3, 100, 12),
        Order(Side::sell, 4, 50, 10),
        Order(Side::sell, 5, 100, 12),
    };
    for (auto& order : orders) {
      if (order.getSide() == Side::buy)
        orderBook.executeBuyOrder(order);
      else
        orderBook.executeSellOrder(order);
    }

    std::string ans{"Buy : 100@11#2 100@10#1 \nSell: 50@12#5 \n"};
    return validate(orderBook, ans);
  }

  bool testOrderBook_LimitOrder_NoLeftoverOrders() {
    OrderBook orderBook{};
    std::vector<Order> orders{
        Order(Side::buy, 1, 100, 10),
        Order(Side::buy, 2, 100, 10),
        Order(Side::buy, 3, 100, 12),
        Order(Side::sell, 4, 500, 10),
    };
    for (auto& order : orders) {
      if (order.getSide() == Side::buy)
        orderBook.executeBuyOrder(order);
      else
        orderBook.executeSellOrder(order);
    }

    std::string ans{"Buy : \nSell: 200@10#4 \n"};
    return validate(orderBook, ans);
  }

  bool testOrderBook_MarketOrder_LeftoverOrders() {
    OrderBook orderBook{};
    std::vector<Order> orders{
        Order(Side::buy, 1, 100, 10),
        Order(Side::buy, 2, 100, 10),
        Order(Side::buy, 3, 100, 12),
        Order(Side::sell, 5, 250, Order::k_sellMarketOrderPrice),
    };
    for (auto& order : orders) {
      if (order.getSide() == Side::buy)
        orderBook.executeBuyOrder(order);
      else
        orderBook.executeSellOrder(order);
    }
    std::string ans{"Buy : 50@10#2 \nSell: \n"};
    return validate(orderBook, ans);
  }

  bool testOrderBook_MarketOrder_NoLeftoverOrders() {
    OrderBook orderBook{};
    std::vector<Order> orders{
        Order(Side::sell, 1, 100, 10),
        Order(Side::sell, 2, 100, 10),
        Order(Side::sell, 3, 100, 12),
        Order(Side::buy, 5, 400, Order::k_buyMarketOrderPrice),
    };
    for (auto& order : orders) {
      if (order.getSide() == Side::buy)
        orderBook.executeBuyOrder(order);
      else
        orderBook.executeSellOrder(order);
    }
    std::string ans{"Buy : \nSell: \n"};
    return validate(orderBook, ans);
  }

  bool testOrderBook_CancelOrder_StartOfBook() {
    OrderBook orderBook{};
    std::vector<Order> orders{
        Order(Side::buy, 1, 100, 10),
        Order(Side::buy, 2, 100, 11),
        Order(Side::buy, 3, 100, 11),
        Order(Side::buy, 4, 100, 11),
        Order(Side::buy, 5, 100, 12),
        Order(Side::sell, 6, 50, 10),
        Order(Side::sell, 7, 100, 12),
    };
    for (auto& order : orders) {
      if (order.getSide() == Side::buy)
        orderBook.executeBuyOrder(order);
      else
        orderBook.executeSellOrder(order);
    }
    orderBook.cancelOrder(2);
    Order o{Side::sell, 8, 250, 10};
    orderBook.executeSellOrder(o);

    std::string ans{"Buy : 50@10#1 \nSell: 50@12#7 \n"};
    return validate(orderBook, ans);
  }

  bool testOrderBook_CancelOrder_MiddleOfBook() {
    OrderBook orderBook{};
    std::vector<Order> orders{
        Order(Side::buy, 1, 100, 10),
        Order(Side::buy, 2, 100, 11),
        Order(Side::buy, 3, 100, 11),
        Order(Side::buy, 4, 100, 11),
        Order(Side::buy, 5, 100, 12),
        Order(Side::sell, 6, 50, 10),
        Order(Side::sell, 7, 100, 12),
    };
    for (auto& order : orders) {
      if (order.getSide() == Side::buy)
        orderBook.executeBuyOrder(order);
      else
        orderBook.executeSellOrder(order);
    }
    orderBook.cancelOrder(3);

    std::string ans{"Buy : 100@11#2 100@11#4 100@10#1 \nSell: 50@12#7 \n"};
    return validate(orderBook, ans);
  }

  void suite() {
    m_passCount += testOrderBook_LimitOrder_LeftoverOrders();
    m_passCount += testOrderBook_LimitOrder_NoLeftoverOrders();
    m_passCount += testOrderBook_MarketOrder_LeftoverOrders();
    m_passCount += testOrderBook_MarketOrder_NoLeftoverOrders();
    m_passCount += testOrderBook_CancelOrder_StartOfBook();
    m_passCount += testOrderBook_CancelOrder_MiddleOfBook();
  }
};
