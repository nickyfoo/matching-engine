#pragma once
#include <iostream>
#include <sstream>

#include "OrderBook.h"
#include "OrderDispatcher.h"
class OrderDispatcherTest {
 public:
  int m_testCount{6};
  int m_passCount{};
  OrderDispatcherTest() {
    suite();
    std::cout << "OrderBookDispatcherTest: " << m_passCount << "/"
              << m_testCount << '\n';
  }

  bool validate(const OrderBook& orderBook, std::string_view ans) {
    std::stringstream ss;
    ss << orderBook;
    return ss.str() == ans;
  }

  bool testOrderBookDispatcher_LimitOrder_LeftoverOrders() {
    OrderBook orderBook{};
    OrderDispatcher dispatcher{orderBook};
    std::vector<OrderRequest> orders{
        OrderRequest(RequestType::SUB_LO, Side::buy, 100, 10),
        OrderRequest(RequestType::SUB_LO, Side::buy, 100, 11),
        OrderRequest(RequestType::SUB_LO, Side::buy, 100, 12),
        OrderRequest(RequestType::SUB_LO, Side::sell, 50, 10),
        OrderRequest(RequestType::SUB_LO, Side::sell, 100, 12),
    };
    for (auto& order : orders) {
      dispatcher.dispatch(order);
    }

    std::string ans{"Buy : 100@11#2 100@10#1 \nSell: 50@12#5 \n"};
    return validate(orderBook, ans);
  }

  bool testOrderBookDispatcher_LimitOrder_NoLeftoverOrders() {
    OrderBook orderBook{};
    OrderDispatcher dispatcher{orderBook};
    std::vector<OrderRequest> orders{
        OrderRequest(RequestType::SUB_LO, Side::buy, 100, 10),
        OrderRequest(RequestType::SUB_LO, Side::buy, 100, 10),
        OrderRequest(RequestType::SUB_LO, Side::buy, 100, 12),
        OrderRequest(RequestType::SUB_LO, Side::sell, 500, 10),
    };
    for (auto& order : orders) {
      dispatcher.dispatch(order);
    }

    std::string ans{"Buy : \nSell: 200@10#4 \n"};
    return validate(orderBook, ans);
  }

  bool testOrderBookDispatcher_MarketOrder_LeftoverOrders() {
    OrderBook orderBook{};
    OrderDispatcher dispatcher{orderBook};
    std::vector<OrderRequest> orders{
        OrderRequest(RequestType::SUB_LO, Side::buy, 100, 10),
        OrderRequest(RequestType::SUB_LO, Side::buy, 100, 10),
        OrderRequest(RequestType::SUB_LO, Side::buy, 100, 12),
        OrderRequest(RequestType::SUB_MO, Side::sell, 250),
    };
    for (auto& order : orders) {
      dispatcher.dispatch(order);
    }
    std::string ans{"Buy : 50@10#2 \nSell: \n"};
    return validate(orderBook, ans);
  }

  bool testOrderBookDispatcher_MarketOrder_NoLeftoverOrders() {
    OrderBook orderBook{};
    OrderDispatcher dispatcher{orderBook};
    std::vector<OrderRequest> orders{
        OrderRequest(RequestType::SUB_LO, Side::sell, 100, 10),
        OrderRequest(RequestType::SUB_LO, Side::sell, 100, 10),
        OrderRequest(RequestType::SUB_LO, Side::sell, 100, 12),
        OrderRequest(RequestType::SUB_MO, Side::buy, 400),
    };
    for (auto& order : orders) {
      dispatcher.dispatch(order);
    }
    std::string ans{"Buy : \nSell: \n"};
    return validate(orderBook, ans);
  }

  bool testOrderBookDispatcher_CancelOrder_StartOfBook() {
    OrderBook orderBook{};
    OrderDispatcher dispatcher{orderBook};
    std::vector<OrderRequest> orders{
        OrderRequest(RequestType::SUB_LO, Side::buy, 100, 10),
        OrderRequest(RequestType::SUB_LO, Side::buy, 100, 11),
        OrderRequest(RequestType::SUB_LO, Side::buy, 100, 11),
        OrderRequest(RequestType::SUB_LO, Side::buy, 100, 11),
        OrderRequest(RequestType::SUB_LO, Side::buy, 100, 12),
        OrderRequest(RequestType::SUB_LO, Side::sell, 50, 10),
        OrderRequest(RequestType::SUB_LO, Side::sell, 100, 12),
        OrderRequest(RequestType::CXL, 2),
        OrderRequest(RequestType::SUB_LO, Side::sell, 250, 10),
    };
    for (auto& order : orders) {
      dispatcher.dispatch(order);
    }

    std::string ans{"Buy : 50@10#1 \nSell: 50@12#7 \n"};
    return validate(orderBook, ans);
  }

  bool testOrderBookDispatcher_CancelOrder_MiddleOfBook() {
    OrderBook orderBook{};
    OrderDispatcher dispatcher{orderBook};
    std::vector<OrderRequest> orders{
        OrderRequest(RequestType::SUB_LO, Side::buy, 100, 10),
        OrderRequest(RequestType::SUB_LO, Side::buy, 100, 11),
        OrderRequest(RequestType::SUB_LO, Side::buy, 100, 11),
        OrderRequest(RequestType::SUB_LO, Side::buy, 100, 11),
        OrderRequest(RequestType::SUB_LO, Side::buy, 100, 12),
        OrderRequest(RequestType::SUB_LO, Side::sell, 50, 10),
        OrderRequest(RequestType::SUB_LO, Side::sell, 100, 12),
        OrderRequest(RequestType::CXL, 3)};
    for (auto& order : orders) {
      dispatcher.dispatch(order);

    }

    std::string ans{"Buy : 100@11#2 100@11#4 100@10#1 \nSell: 50@12#7 \n"};
    return validate(orderBook, ans);
  }

  void suite() {
    m_passCount += testOrderBookDispatcher_LimitOrder_LeftoverOrders();
    m_passCount += testOrderBookDispatcher_LimitOrder_NoLeftoverOrders();
    m_passCount += testOrderBookDispatcher_MarketOrder_LeftoverOrders();
    m_passCount += testOrderBookDispatcher_MarketOrder_NoLeftoverOrders();
    m_passCount += testOrderBookDispatcher_CancelOrder_StartOfBook();
    m_passCount += testOrderBookDispatcher_CancelOrder_MiddleOfBook();
  }
};
