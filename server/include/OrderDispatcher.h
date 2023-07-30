#pragma once
#include "OrderBook.h"
#include "OrderRequest.h"
class OrderDispatcher {
 public:
  OrderDispatcher(OrderBook& orderBook)
      : m_orderBook{orderBook} {};
  void dispatch(OrderRequest& orderRequest) {
    orderRequest.setOrderID(m_currentID++);
    Order order{orderRequest};
    switch (orderRequest.getType()) {
      case RequestType::CXL:
        m_orderBook.cancelOrder(orderRequest.getCancelledOrderID());
        return;
      case RequestType::SUB_LO:
      case RequestType::SUB_MO:
        if (order.getSide() == Side::buy) {
          m_orderBook.executeBuyOrder(order);
        } else {
          m_orderBook.executeSellOrder(order);
        }
      default:
        return;
    }
  }

 private:
  OrderBook& m_orderBook;
  int m_currentID{1};
};
