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
        Order('B', 1, 100, 10), Order('B', 2, 100, 11), Order('B', 3, 100, 12),
        Order('S', 4, 50, 10),  Order('S', 5, 100, 12),
    };
    for (auto& order : orders) {
      if (order.m_side == 'B')
        orderBook.buyOrder(order);
      else
        orderBook.sellOrder(order);
    }

    std::string ans{"Buy : 100@11#2 100@10#1 \nSell: 50@12#5 \n"};
    return validate(orderBook, ans);
  }

  bool testOrderBook_LimitOrder_NoLeftoverOrders() {
    OrderBook orderBook{};
    std::vector<Order> orders{
        Order('B', 1, 100, 10),
        Order('B', 2, 100, 10),
        Order('B', 3, 100, 12),
        Order('S', 5, 500, 10),
    };
    for (auto& order : orders) {
      if (order.m_side == 'B')
        orderBook.buyOrder(order);
      else
        orderBook.sellOrder(order);
    }

    std::string ans{"Buy : \nSell: 200@10#5 \n"};
    return validate(orderBook, ans);
  }

  bool testOrderBook_MarketOrder_LeftoverOrders() {
    OrderBook orderBook{};
    std::vector<Order> orders{
        Order('B', 1, 100, 10),
        Order('B', 2, 100, 10),
        Order('B', 3, 100, 12),
        Order('S', 5, 250, Order::k_sellMarketOrderPrice),
    };
    for (auto& order : orders) {
      if (order.m_side == 'B')
        orderBook.buyOrder(order);
      else
        orderBook.sellOrder(order);
    }
    std::string ans{"Buy : 50@10#2 \nSell: \n"};
    return validate(orderBook, ans);
  }

  bool testOrderBook_MarketOrder_NoLeftoverOrders() {
    OrderBook orderBook{};
    std::vector<Order> orders{
        Order('S', 1, 100, 10),
        Order('S', 2, 100, 10),
        Order('S', 3, 100, 12),
        Order('B', 5, 400, Order::k_buyMarketOrderPrice),
    };
    for (auto& order : orders) {
      if (order.m_side == 'B')
        orderBook.buyOrder(order);
      else
        orderBook.sellOrder(order);
    }
    std::string ans{"Buy : \nSell: \n"};
    return validate(orderBook, ans);
  }

  bool testOrderBook_CancelOrder_StartOfBook() {
    OrderBook orderBook{};
    std::vector<Order> orders{
        Order('B', 1, 100, 10), Order('B', 2, 100, 11), Order('B', 3, 100, 11),
        Order('B', 4, 100, 11), Order('B', 5, 100, 12), Order('S', 6, 50, 10),
        Order('S', 7, 100, 12),
    };
    for (auto& order : orders) {
      if (order.m_side == 'B')
        orderBook.buyOrder(order);
      else
        orderBook.sellOrder(order);
    }
    orderBook.cancelOrder(2);
    Order o{'S', 8, 250, 10};
    orderBook.sellOrder(o);

    std::string ans{"Buy : 50@10#1 \nSell: 50@12#7 \n"};
    return validate(orderBook, ans);
  }

  bool testOrderBook_CancelOrder_MiddleOfBook() {
    OrderBook orderBook{};
    std::vector<Order> orders{
        Order('B', 1, 100, 10), Order('B', 2, 100, 11), Order('B', 3, 100, 11),
        Order('B', 4, 100, 11), Order('B', 5, 100, 12), Order('S', 6, 50, 10),
        Order('S', 7, 100, 12),
    };
    for (auto& order : orders) {
      if (order.m_side == 'B')
        orderBook.buyOrder(order);
      else
        orderBook.sellOrder(order);
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
