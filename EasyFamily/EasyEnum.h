#pragma once

#include <initializer_list>
#include <map>
#include <stdexcept>
#include <vector>

namespace common_util {
/// A simple enumeration utility
/// \tparam keyT Key type
/// \tparam valT Value type
template <class keyT, class valT>
class EasyEnum {
 public:
  EasyEnum() = default;
  /// Constructor using initializer_list
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

  /// Get value for specific key
  valT get(const keyT& key) const {
    auto it = myEnum_.find(key);
    if (it != myEnum_.end()) {
      return it->second;
    }
    throw std::runtime_error("Key not found");
  }

  /// Get key for specific value
  keyT getKeyOf(const valT& val) const {
    for (const auto& pair : myEnum_) {
      if (pair.second == val) {
        return pair.first;
      }
    }
    throw std::runtime_error("Value not found");
  }

  /// Overload operator[]
  valT operator[](const keyT& key) const { return get(key); }

  /// Get all keys as vector
  std::vector<keyT> getKeys() const {
    std::vector<keyT> keys;
    for (const auto& [first, second] : myEnum_) {
      keys.push_back(first);
    }
    return keys;
  }

  /// Get all values as vector
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

}  // namespace common_util
