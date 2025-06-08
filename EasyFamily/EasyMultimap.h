#pragma once
#include <algorithm>
#include <optional>
#include <stdexcept>
#include <string>
#include <tuple>
#include <unordered_set>
#include <vector>

namespace common_util {
template <typename... Types>
class EasyMultimap {
 public:
  using TupleType = std::tuple<Types...>;
  using VectorType = std::vector<TupleType>;

  // import initializer_list
  EasyMultimap(std::initializer_list<TupleType> initList) { addTuplesWithCheck(initList); }

  size_t size() const { return maptuples_.size(); }

  EasyMultimap& merge(const EasyMultimap& r) {
    for (const auto& elem : r.maptuples_) insert(elem);
    return *this;
  }

  // add a tuple with uniqueness check
  EasyMultimap& insert(const TupleType& elem) {
    addTuplesWithCheck({elem});
    return *this;
  }

  EasyMultimap& insertWithFirstCheck(const TupleType& elem) {
    addTuplesWithFirstCheck({elem});
    return *this;
  }

  //! add a tuple without uniqueness check, allow duplicate
  //! \param elem tuple elem
  //! \return *this
  EasyMultimap& insertWithoutCheck(const TupleType& elem) {
    maptuples_.push_back(elem);
    return *this;
  }

  EasyMultimap& mergeWithoutCheck(const EasyMultimap& r) {
    for (const auto& elem : r.maptuples_) insertWithoutCheck(elem);  // 修复：应该调用insertWithoutCheck
    return *this;
  }

  //! get value of column ToIndex based on value of column FromIndex
  //! \tparam FromIndex idx of in param
  //! \tparam ToIndex idx of target tuple item
  //! \tparam ValueType type of in param
  //! \param fromValue value of in param
  //! \return first match
  template <size_t FromIndex, size_t ToIndex, typename ValueType>
  std::tuple_element_t<ToIndex, TupleType> getVal(const ValueType& fromValue) const {
    auto it = std::find_if(maptuples_.begin(), maptuples_.end(), [&](const auto& elem) {
      // 修复1：使用更通用的比较方式，支持枚举类型
      return isEqual(std::get<FromIndex>(elem), fromValue);
    });
    if (it != maptuples_.end()) return std::get<ToIndex>(*it);
    throw std::runtime_error("Value not found");
  }

  //! get all items of N-th column
  //! \tparam N The index of column to retrieve
  //! \return all items N-th column
  template <size_t N>
  std::vector<std::tuple_element_t<N, TupleType>> getColumn() const {
    std::vector<std::tuple_element_t<N, TupleType>> column;
    column.reserve(maptuples_.size());
    for (const auto& elem : maptuples_) column.emplace_back(std::get<N>(elem));
    return column;
  }

  VectorType getRaws() const { return maptuples_; }

 private:
  VectorType maptuples_;

  // 添加通用的相等性比较函数
  template <typename T, typename U>
  static bool isEqual(const T& a, const U& b) {
    // 如果类型相同，直接比较
    if constexpr (std::is_same_v<T, U>) {
      return a == b;
    }
    // 如果都是枚举类型，转换为底层类型比较
    else if constexpr (std::is_enum_v<T> && std::is_enum_v<U>) {
      return static_cast<std::underlying_type_t<T>>(a) == static_cast<std::underlying_type_t<U>>(b);
    }
    // 如果一个是枚举，一个是整数
    else if constexpr (std::is_enum_v<T> && std::is_integral_v<U>) {
      return static_cast<std::underlying_type_t<T>>(a) == b;
    } else if constexpr (std::is_integral_v<T> && std::is_enum_v<U>) {
      return a == static_cast<std::underlying_type_t<U>>(b);
    }
    // 其他情况直接比较（可能需要类型转换）
    else {
      return a == b;
    }
  }

  // ensure each column's items are unique when adding tuples
  void addTuplesWithCheck(std::initializer_list<TupleType> initList) {
    constexpr size_t TupleSize = std::tuple_size_v<TupleType>;
    std::vector<std::unordered_set<std::string>> uniqueColumns(TupleSize);

    for (const auto& elem : initList) {
      bool hasDuplicate = false;
      // iterate over tuple items
      // 修复3：使用apply的正确方式
      applyToTuple(elem, uniqueColumns, hasDuplicate);

      if (hasDuplicate) throw std::runtime_error("Duplicate value found in one of tuple's columns!");
      maptuples_.push_back(elem);
    }
  }

  // ensure only first column's items are unique when adding tuples
  void addTuplesWithFirstCheck(std::initializer_list<TupleType> initList) {
    std::unordered_set<std::string> uniqueFirstColumn;

    // 检查现有数据的第一列
    for (const auto& existingElem : maptuples_) {
      uniqueFirstColumn.insert(toString(std::get<0>(existingElem)));
    }

    for (const auto& elem : initList) {
      // 只检查第一列是否重复
      std::string firstValue = toString(std::get<0>(elem));
      if (!uniqueFirstColumn.insert(firstValue).second) {
        throw std::runtime_error("Duplicate value found in first column!");
      }
      maptuples_.push_back(elem);
    }
  }

  template <size_t I = 0>
  void applyToTuple(const TupleType& elem, std::vector<std::unordered_set<std::string>>& uniqueColumns, bool& hasDuplicate) const {
    if constexpr (I < std::tuple_size_v<TupleType>) {
      hasDuplicate |= !uniqueColumns[I].insert(toString(std::get<I>(elem))).second;
      applyToTuple<I + 1>(elem, uniqueColumns, hasDuplicate);
    }
  }

  template <typename T>
  static std::string toString(const T& value) {
    if constexpr (std::is_same_v<T, std::string>) {
      return value;
    } else if constexpr (std::is_enum_v<T>) {
      // 对于枚举类型，转换为底层类型
      return std::to_string(static_cast<std::underlying_type_t<T>>(value));
    } else if constexpr (std::is_arithmetic_v<T>) {
      return std::to_string(value);
    } else {
      // 对于其他类型，尝试转换为int（可能需要根据实际情况调整）
      return std::to_string(static_cast<int>(value));
    }
  }
};
}  // namespace common_util
