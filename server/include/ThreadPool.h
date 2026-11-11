#pragma once
#include <condition_variable>
#include <functional>
#include <mutex>
#include <queue>
#include <thread>
#include <vector>

class ThreadPool {
 public:
  explicit ThreadPool(size_t numThreads = std::thread::hardware_concurrency()) {
    if (numThreads == 0) numThreads = 4;
    for (size_t i = 0; i < numThreads; ++i) {
      m_workers.emplace_back([this]() {
        while (true) {
          std::function<void()> task;
          {
            std::unique_lock<std::mutex> lock(m_queueMutex);
            m_cv.wait(lock, [this]() {
              return m_stop || !m_tasks.empty();
            });
            if (m_stop && m_tasks.empty()) {
              return;
            }
            task = std::move(m_tasks.front());
            m_tasks.pop();
          }
          task();
        }
      });
    }
  }

  template <typename F>
  void enqueue(F&& f) {
    {
      std::unique_lock<std::mutex> lock(m_queueMutex);
      if (m_stop) {
        throw std::runtime_error("enqueue on stopped ThreadPool");
      }
      m_tasks.emplace(std::forward<F>(f));
    }
    m_cv.notify_one();
  }

  ~ThreadPool() {
    {
      std::unique_lock<std::mutex> lock(m_queueMutex);
      m_stop = true;
    }
    m_cv.notify_all();
  }

 private:
  std::vector<std::jthread> m_workers;
  std::queue<std::function<void()>> m_tasks;
  std::mutex m_queueMutex;
  std::condition_variable m_cv;
  bool m_stop{false};
};
