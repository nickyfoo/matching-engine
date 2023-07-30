#include "Order.h"

void Order::transactWith(Order& other) {
  int quantityTransacted = std::min<>(m_quantity, other.m_quantity);
  m_quantity -= quantityTransacted;
  other.m_quantity -= quantityTransacted;
}
