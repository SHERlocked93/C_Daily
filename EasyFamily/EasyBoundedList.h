#pragma once

#include <condition_variable>
#include <memory>
#include <mutex>
#include <queue>

namespace common_util {
template <typename MSG_TYPE>
class BoundedQueue {
 public:
  explicit BoundedQueue(size_t capacity) : capacity_(capacity) {}

  bool push(std::unique_ptr<MSG_TYPE> message) {
    std::unique_lock lock(mutex_);
    not_full_.wait(lock, [this]() { return queue_.size() < capacity_; });

    queue_.push(std::move(message));
    lock.unlock();
    not_empty_.notify_one();
    return true;
  }

  std::unique_ptr<MSG_TYPE> pop() {
    std::unique_lock lock(mutex_);
    not_empty_.wait(lock, [this]() { return !queue_.empty(); });

    auto message = std::move(queue_.front());
    queue_.pop();
    lock.unlock();
    not_full_.notify_one();
    return message;
  }

  bool empty() const {
    std::lock_guard lock(mutex_);
    return queue_.empty();
  }

  size_t size() const {
    std::lock_guard lock(mutex_);
    return queue_.size();
  }

 private:
  const size_t capacity_;
  std::queue<std::unique_ptr<MSG_TYPE>> queue_;
  mutable std::mutex mutex_;

  std::condition_variable not_empty_;
  std::condition_variable not_full_;
};

}  // namespace common_util
