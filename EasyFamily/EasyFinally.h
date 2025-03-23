#pragma once

#include <functional>


class EasyFinally {
 public:
  explicit EasyFinally(std::function<void()> func, bool active_ = true)
      : cleanupFunc(std::move(func)), active(active_) {}

  EasyFinally(const EasyFinally&) = delete;
  EasyFinally& operator=(const EasyFinally&) = delete;

  // ����ʱִ��
  ~EasyFinally() {
    if (active && cleanupFunc) cleanupFunc();
  }

  // �ֶ�ȡ���������
  void dismiss() { active = false; }

 private:
  std::function<void()> cleanupFunc;
  bool active;  // �Ƿ���Ҫִ�лص�
};
