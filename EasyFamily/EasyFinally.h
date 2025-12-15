#pragma once

#include <functional>

namespace common_util {

class EasyFinally {
 public:
  explicit EasyFinally(std::function<void()> func, bool active_ = true) : cleanupFunc(std::move(func)), active(active_) {}

  EasyFinally(const EasyFinally&) = delete;
  EasyFinally& operator=(const EasyFinally&) = delete;

  // Execute on destruction
  ~EasyFinally() {
    if (active && cleanupFunc) cleanupFunc();
  }

  // Manually cancel execution
  void dismiss() { active = false; }

 private:
  std::function<void()> cleanupFunc;
  bool active;  // Whether callback needs to be executed
};

}  // namespace common_util
