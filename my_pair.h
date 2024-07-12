#include <concepts>
#include <iostream>
#include <stdexcept>
#include <string>
#include <type_traits>
#include <utility>

template <typename T>
concept Pairable = std::integral<T> || std::floating_point<T> ||
                   std::convertible_to<T, std::string>;

template <typename T1, typename T2>
  requires Pairable<T1> && Pairable<T2>
class MyPair {
 public:
  T1 first;
  T2 second;

  // Конструкторы
  MyPair() : first(T1()), second(T2()) {}
  MyPair(const T1& a, const T2& b) : first(a), second(b) {}

  template <typename U1, typename U2>
    requires(std::is_constructible_v<T1, const U1&> &&
             std::is_constructible_v<T2, const U2&>)
  MyPair(const MyPair<U1, U2>& other)
      : first(other.first), second(other.second) {}

  // Операторы присваивания
  MyPair& operator=(const MyPair& other) {
    if (this != &other) {
      first = other.first;
      second = other.second;
    }
    return *this;
  }

  template <typename U1, typename U2>
    requires(std::is_assignable_v<T1&, const U1&> &&
             std::is_assignable_v<T2&, const U2&>)
  MyPair& operator=(const MyPair<U1, U2>& other) {
    first = other.first;
    second = other.second;
    return *this;
  }

  // Операторы сравнения
  bool operator==(const MyPair& other) const {
    return first == other.first && second == other.second;
  }

  bool operator!=(const MyPair& other) const { return !(*this == other); }

  template <typename U1, typename U2>
  bool operator==(const MyPair<U1, U2>& other) const {
    return first == other.first && second == other.second;
  }

  template <typename U1, typename U2>
  bool operator!=(const MyPair<U1, U2>& other) const {
    return !(*this == other);
  }

  void swap(MyPair& other) {
    std::swap(first, other.first);
    std::swap(second, other.second);
  }
};

// Утилита для создания пары, похожая на std::make_pair
template <typename T1, typename T2>
MyPair<T1, T2> make_my_pair(T1&& first, T2&& second) {
  return MyPair<T1, T2>(std::forward<T1>(first), std::forward<T2>(second));
}

// Перегрузка оператора << для вывода
template <typename T1, typename T2>
  requires Pairable<T1> && Pairable<T2>
std::ostream& operator<<(std::ostream& os, const MyPair<T1, T2>& p) {
  os << "{" << p.first << ", " << p.second << "}";
  return os;
}

int main() {
  auto p1 = make_my_pair(42, std::string("Hello"));
  auto p2 = make_my_pair(3.14, 42);

  std::cout << "Pair 1: " << p1 << 'n';
  std::cout << "Pair 2: " << p2 << 'n';

  if (p1 != p2) {
    std::cout << "Pairs are not equal" << 'n';
  }

  p1.swap(p2);

  std::cout << "After swapn";
  std::cout << "Pair 1: " << p1 << 'n';
  std::cout << "Pair 2: " << p2 << 'n';

  return 0;
}
