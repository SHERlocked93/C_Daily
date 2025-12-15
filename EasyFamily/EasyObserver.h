#pragma once

#include <functional>

template <typename T>
class MyObserver {
 public:
  MyObserver() {}
  virtual ~MyObserver() {}

 public:
  T getValue() const { return m_value; }

  void setValue(const T& newValue) {
    if (m_value != newValue) {
      m_value = newValue;
      emitValueChanged(newValue);
    }
  }

  void setValueChangeCallback(const std::function<void(T)>& callback) { m_cb = callback; }

  T operator=(const T& initVal) {
    setValue(initVal);
    return m_value;
  }

 protected:
  virtual void emitValueChanged(const T& newValue) {
    if (m_cb) {
      m_cb(newValue);
    }
  }

 private:
  T m_value;
  std::function<void(T)> m_cb;
};
