#include <algorithm>
#include <iostream>
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

  // Constructor with initializer_list; uses uniqueness check by default.
  EasyMultimap(std::initializer_list<TupleType> initList) { addTuplesWithCheck(initList); }

  // Add a tuple with uniqueness check (ensures each column's values are unique)
  EasyMultimap& addTuple(const TupleType& elem) {
    addTuplesWithCheck({elem});
    return *this;
  }

  EasyMultimap& merge(const EasyMultimap& other) {
    for (const auto& elem : other.maptuples_) addTuple(elem);
    return *this;
  }

  //! Add a tuple without uniqueness check (duplicates allowed)
  //! \param elem tuple elem
  //! \return *this
  EasyMultimap& addTupleWithoutCheck(const TupleType& elem) {
    maptuples_.push_back(elem);
    return *this;
  }

  EasyMultimap& mergeWithoutCheck(const EasyMultimap& other) {
    for (const auto& elem : other.maptuples_) addTupleWithoutCheck(elem);
    return *this;
  }

  //! Get the value of column ToIndex based on the value of column FromIndex (returns )
  //! \tparam FromIndex idx of enter param
  //! \tparam ToIndex idx of target tuple item
  //! \tparam ValueType type of enter param
  //! \param fromValue value of enter param
  //! \return the first match
  template <size_t FromIndex, size_t ToIndex, typename ValueType>
  std::tuple_element_t<ToIndex, TupleType> getVal(const ValueType& fromValue) const {
    auto it = std::find_if(maptuples_.begin(), maptuples_.end(), [&](const auto& elem) { return std::get<FromIndex>(elem) == fromValue; });
    if (it != maptuples_.end()) return std::get<ToIndex>(*it);
    throw std::runtime_error("Value not found");
  }

  //! Get all elements of the N-th column
  //! \tparam N The index of the column to retrieve
  //! \return A vector containing all elements of the N-th column
  template <size_t N>
  std::vector<std::tuple_element_t<N, TupleType>> getColumn() const {
    std::vector<std::tuple_element_t<N, TupleType>> column;
    column.reserve(maptuples_.size());
    for (const auto& elem : maptuples_) column.emplace_back(std::get<N>(elem));

    return column;
  }

 private:
  VectorType maptuples_;

  // check each column's elements are unique
  void addTuplesWithCheck(std::initializer_list<TupleType> initList) {
    constexpr size_t TupleSize = std::tuple_size_v<TupleType>;
    std::vector<std::unordered_set<std::string>> uniqueColumns(TupleSize);

    for (const auto& elem : initList) {
      bool hasDuplicate = false;

      std::apply(
          [&](const auto&... args) {
            size_t idx = 0;
            ((hasDuplicate |= !uniqueColumns[idx++].insert(toString(args)).second), ...);
          },
          elem);

      if (hasDuplicate) throw std::runtime_error("Duplicate value found in one of the tuple's columns!");

      maptuples_.push_back(elem);
    }
  }

  template <typename T>
  static std::string toString(const T& value) {
    if constexpr (std::is_same_v<T, std::string>)
      return value;
    else
      return std::to_string(value);
  }
};


// Test cases
int main() {
  try {
    std::cout << "=== Testing addTuple (unique values) ===" << std::endl;
    // Initialize with 4-element tuple: int, string, int, double
    common_util::EasyMultimap<int, std::string, int, double> em1{
        {1, "hello", 11, 543.1},
        {4, "world", 88, 545.0},
    };
    // Using uniqueness check: no duplicate allowed
    em1.addTuple({2, "yyy", 99, 1.2});
    // Uncommenting the following line would throw an exception due to duplicate in a column:
    // em1.addTuple({4, "zzz", 100, 2.3});

    // std::cout << "=== Testing addTuple (allows duplicates) ===" << std::endl;
    // // Using non-check method: duplicates are allowed
    // em1.addTupleWithoutCheck({4, "zzz", 100, 2.3});
    // em1.addTupleWithoutCheck({4, "abc", 101, 9.9});

    std::cout << "=== Testing getValueByAnotherColumn ===" << std::endl;
    // Find the first tuple whose 0-th column equals 4, and return its 3rd column value
    auto res = em1.getVal<0, 3>(4);
    std::cout << "Found: " << res << std::endl;  // Expected output: 545.0 (from the first matching tuple)

    // 获取第1列（std::string 类型）
    auto column1 = em1.getColumn<1>();
    std::cout << "Column 1 (strings): ";
    for (const auto& val : column1) {
      std::cout << val << " ";
    }
    std::cout << std::endl;  // 输出: hello world yyy

  } catch (const std::exception& e) {
    std::cerr << "Error: " << e.what() << std::endl;
  }

  return 0;
}
