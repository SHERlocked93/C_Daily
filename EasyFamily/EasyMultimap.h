#pragma once
#include <algorithm>
#include <optional>
#include <stdexcept>
#include <string>
#include <tuple>
#include <unordered_set>
#include <vector>

template <typename... Types>
class EasyMultimap {
 public:
  using TupleType = std::tuple<Types...>;
  using VectorType = std::vector<TupleType>;

  // import initializer_list
  EasyMultimap(std::initializer_list<TupleType> initList) { addTuplesWithCheck(initList); }

  // add a tuple with uniqueness check
  EasyMultimap& insert(const TupleType& elem) {
    addTuplesWithCheck({elem});
    return *this;
  }

  EasyMultimap& merge(const EasyMultimap& r) {
    for (const auto& elem : r.maptuples_) addTuple(elem);
    return *this;
  }

  //! ddd a tuple without uniqueness check, allow duplicate
  //! \param elem tuple elem
  //! \return *this
  EasyMultimap& insertWithoutCheck(const TupleType& elem) {
    maptuples_.push_back(elem);
    return *this;
  }

  EasyMultimap& mergeWithoutCheck(const EasyMultimap& r) {
    for (const auto& elem : r.maptuples_) addTupleWithoutCheck(elem);
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
      return std::get<FromIndex>(elem) == fromValue;
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

  // ensure each column's items are unique when adding tuples
  void addTuplesWithCheck(std::initializer_list<TupleType> initList) {
    constexpr size_t TupleSize = std::tuple_size_v<TupleType>;
    std::vector<std::unordered_set<std::string>> uniqueColumns(TupleSize);

    for (const auto& elem : initList) {
      bool hasDuplicate = false;

      // iterate over tuple items
      std::apply(
          [&](const auto&... args) {
            size_t idx = 0;
            ((hasDuplicate |= !uniqueColumns[idx++].insert(toString(args)).second), ...);
          },
          elem);

      if (hasDuplicate)
        throw std::runtime_error("Duplicate value found in one of tuple's columns!");

      maptuples_.push_back(elem);
    }
  }

  template <typename T>
  static std::string toString(const T& value) {
    if constexpr (std::is_same_v<T, std::string>) return value;
    else return std::to_string(value);
  }
};
