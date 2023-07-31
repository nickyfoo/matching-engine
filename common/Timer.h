#pragma once

#include <chrono>
#include <iomanip>
#include <iostream>
#include <sstream>

using Clock = std::chrono::system_clock;
using Timestamp = std::chrono::time_point<Clock>;

namespace detail {
inline int64_t milliseconds_between(const Timestamp& start,
                                    const Timestamp& end) {
  return std::chrono::duration_cast<std::chrono::milliseconds>(end - start)
      .count();
}

}  // namespace detail

class Timer {
 public:
  Timer() : m_start_ts(Clock::now()) {
    std::time_t t = Clock::to_time_t(m_start_ts);
    std::cout << "[Timer] start at: "
              << std::put_time(std::localtime(&t), "%F %T") << '\n';
  }
  ~Timer() {
    Timestamp end_ts = Clock::now();
    std::time_t t = Clock::to_time_t(end_ts);
    std::cout << "[Timer] end at: "
              << std::put_time(std::localtime(&t), "%F %T") << '\n';
    std::cout << "[Timer] milliseconds elapsed: "
              << detail::milliseconds_between(m_start_ts, end_ts) << '\n';
  }

 private:
  Timestamp m_start_ts;
};