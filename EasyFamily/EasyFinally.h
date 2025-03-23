#pragma once

#include <functional>


class EasyFinally {
 public:
  explicit EasyFinally(std::function<void()> func, bool active_ = true)
      : cleanupFunc(std::move(func)), active(active_) {}

  EasyFinally(const EasyFinally&) = delete;
  EasyFinally& operator=(const EasyFinally&) = delete;

  // 析构时执行
  ~EasyFinally() {
    if (active && cleanupFunc) cleanupFunc();
  }

  // 手动取消清理操作
  void dismiss() { active = false; }

 private:
  std::function<void()> cleanupFunc;
  bool active;  // 是否需要执行回调
};
