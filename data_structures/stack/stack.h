#ifndef CPP_ALGORITHMS_DATA_STRUCTURES_STACK_STACK_H
#define CPP_ALGORITHMS_DATA_STRUCTURES_STACK_STACK_H

#include <initializer_list>
#include <ostream>
#include <utility>

#include "dynamic_array.h"

template <class T>
class Stack {
 public:
  using value_type = T;
  using size_type = std::size_t;
  using difference_type = std::ptrdiff_t;
  using reference = value_type&;
  using const_reference = const value_type&;
  using pointer = value_type*;
  using const_pointer = const value_type*;
  using iterator = typename DynamicArray<T>::iterator;
  using const_iterator = typename DynamicArray<T>::const_iterator;
  using reverse_iterator = typename DynamicArray<T>::reverse_iterator;
  using const_reverse_iterator =
      typename DynamicArray<T>::const_reverse_iterator;

  // Constructors

  Stack() noexcept = default;

  Stack(const Stack& other) : container_{other.container_} {}

  Stack(Stack&& other) noexcept : container_{std::move(other.container_)} {}

  Stack(const std::initializer_list<T>& list) : container_{list} {}

  // Assignments

  Stack& operator=(const Stack& other) {
    container_ = other.container_;
    return *this;
  }

  Stack& operator=(Stack&& other) noexcept {
    container_ = std::move(other.container_);
    return *this;
  }

  Stack& operator=(const std::initializer_list<T> list) {
    container_ = list;
    return *this;
  }

  // Element access

  reference Top() { return container_.Back(); }
  const_reference Top() const { return container_.Back(); }

  // Iterators

  iterator begin() noexcept { return container_.begin(); }
  const_iterator begin() const noexcept { return container_.begin(); }
  const_iterator cbegin() const noexcept { return container_.cbegin(); }

  iterator end() noexcept { return container_.end(); }
  const_iterator end() const noexcept { return container_.end(); }
  const_iterator cend() const noexcept { return container_.cend(); }

  reverse_iterator rbegin() noexcept { return container_.rbegin(); }
  const_reverse_iterator rbegin() const noexcept { return container_.rbegin(); }
  const_reverse_iterator crbegin() const noexcept {
    return container_.crbegin();
  }

  reverse_iterator rend() noexcept { return container_.rend(); }
  const_reverse_iterator rend() const noexcept { return container_.rend(); }
  const_reverse_iterator crend() const noexcept { return container_.crend(); }

  // Capacity

  bool Empty() const noexcept { return container_.Empty(); }

  size_type Size() const noexcept { return container_.Size(); }

  // Modifiers

  void Push(const_reference value) { container_.PushBack(value); }

  void Pop() { container_.PopBack(); }

  void Swap(Stack& other) noexcept { container_.Swap(other.container_); }

  // Comparison operators

  bool operator==(const Stack& other) const noexcept {
    return container_ == other.container_;
  }

  bool operator!=(const Stack& other) const noexcept {
    return container_ != other.container_;
  }

  bool operator<(const Stack& other) const noexcept {
    return container_ < other.container_;
  }

  bool operator<=(const Stack& other) const noexcept {
    return container_ <= other.container_;
  }

  bool operator>(const Stack& other) const noexcept {
    return container_ > other.container_;
  }

  bool operator>=(const Stack& other) const noexcept {
    return container_ >= other.container_;
  }

  // Debug

  friend std::ostream& operator<<(std::ostream& os,
                                  const Stack& stack) noexcept {
    os << stack.container_;
    return os;
  }

 private:
  DynamicArray<T> container_;
};

#endif  // CPP_ALGORITHMS_DATA_STRUCTURES_STACK_STACK_H