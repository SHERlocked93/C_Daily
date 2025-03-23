#pragma once

#include <initializer_list>
#include <map>
#include <stdexcept>
#include <vector>


//! 一个简单的枚举类
//! \tparam keyT 键类型
//! \tparam valT 值类型
template <class keyT, class valT>
class EasyEnum {
 public:
  EasyEnum() = default;
  //! 构造函数接受 initializer_list
  EasyEnum(std::initializer_list<std::pair<keyT, valT>> initList) {
    for (const auto& item : initList) {
      myEnum_.emplace(item);
    }
  }
  EasyEnum(std::initializer_list<EasyEnum> initEasyEnums) {
    for (const auto& item : initEasyEnums) {
      for (const auto& pair : item.myEnum_) {
        if (!myEnum_.count(pair.first)) myEnum_.emplace(pair);
      }
    }
  }
  ~EasyEnum() = default;

  //! 获取特定键的值
  valT get(const keyT& key) const {
    auto it = myEnum_.find(key);
    if (it != myEnum_.end()) {
      return it->second;
    }
    throw std::runtime_error("Key not found");
  }

  //! 获取特定值的键
  keyT getKeyOf(const valT& val) const {
    for (const auto& pair : myEnum_) {
      if (pair.second == val) {
        return pair.first;
      }
    }
    throw std::runtime_error("Value not found");
  }

  // 重载 operator[]
  valT operator[](const keyT& key) const { return get(key); }

  //! 获取所有键的 vector
  std::vector<keyT> getKeys() const {
    std::vector<keyT> keys;
    for (const auto& [first, second] : myEnum_) {
      keys.push_back(first);
    }
    return keys;
  }

  //! 获取所有值的 vector
  std::vector<valT> getVals() const {
    std::vector<valT> vals;
    for (const auto& [first, second] : myEnum_) {
      vals.push_back(second);
    }
    return vals;
  }

  bool hasKey(const keyT& key) const { return myEnum_.count(key); }

  bool hasVal(const valT& val) const {
    for (const auto& pair : myEnum_) {
      if (pair.second == val) return true;
    }
    return false;
  }

  void add(const keyT& key, const valT& val) { myEnum_.emplace(key, val); }

 private:
  std::map<keyT, valT> myEnum_;
};

template <class keyT, class valT>
valT getValFromEnum(EasyEnum<keyT, valT> easyEnum_, keyT key_) {
  if (easyEnum_.hasKey(key_)) return easyEnum_.get(key_);
  return valT{};
}
template <class keyT, class valT>
keyT getKeyFromEnum(EasyEnum<keyT, valT> easyEnum_, valT val_) {
  if (easyEnum_.hasVal(val_)) return easyEnum_.getKeyOf(val_);
  return keyT{};
}
