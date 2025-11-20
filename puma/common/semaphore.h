#pragma once
#include <mutex>
#include <condition_variable>

class Semaphore {
public:
  explicit Semaphore(int count = 0) : count_(count) {
  }

  void Reset() {
    std::unique_lock<std::mutex> lock(mutex_);
    count_ = 0;
  }

  void Signal() {
    std::unique_lock<std::mutex> lock(mutex_);
    ++count_;
    cv_.notify_one();
  }

  void Wait() {
    std::unique_lock<std::mutex> lock(mutex_);
    cv_.wait(lock, [=] { return count_ > 0; });
    --count_;
  }

  bool WaitFor(uint64_t milliseconds_timeout) {
    std::unique_lock<std::mutex> lock(mutex_);
    auto status = cv_.wait_for(lock, std::chrono::milliseconds(milliseconds_timeout), [=] { return (count_ > 0); });
    count_ = 0;
    return status;
  }

  bool WaitFor(uint64_t milliseconds_timeout, bool* stopping) {
      std::unique_lock<std::mutex> lock(mutex_);
      auto status = cv_.wait_for(lock, std::chrono::milliseconds(milliseconds_timeout), [=] { return (count_ > 0) || (stopping ? (*stopping) : false) ; });
      count_ = 0;
      return status;
  }

  bool WaitFor(uint64_t milliseconds_timeout, bool* stopping, bool* connection_remove) {
      std::unique_lock<std::mutex> lock(mutex_);
      auto status = cv_.wait_for(lock, std::chrono::milliseconds(milliseconds_timeout), [=] { return (count_ > 0) || (stopping ? (*stopping) : false) || (connection_remove ? (*connection_remove) : false); });
      count_ = 0;
      return status;
  }

private:
  std::mutex mutex_;
  std::condition_variable cv_;
  int count_;
};