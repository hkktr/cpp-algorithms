#ifndef CPP_ALGORITHMS_DATA_STRUCTURES_PRIORITY_QUEUE_PRIORITY_QUEUE_H_
#define CPP_ALGORITHMS_DATA_STRUCTURES_PRIORITY_QUEUE_PRIORITY_QUEUE_H_

#include <cstddef>
#include <functional>
#include <initializer_list>
#include <memory>
#include <ostream>
#include <utility>

#include "binary_heap.h"

template <class T, class Compare = std::less<T>,
          class Allocator = std::allocator<T>>
class PriorityQueue {
 public:
  using value_compare = Compare;
  using value_type = T;
  using size_type = std::size_t;
  using reference = value_type&;
  using const_reference = const value_type&;

  // Constructors

  PriorityQueue() noexcept = default;

  PriorityQueue(const PriorityQueue& other) : container_{other.container_} {}

  PriorityQueue(PriorityQueue&& other) noexcept
      : container_{std::move(other.container_)} {}

  PriorityQueue(const std::initializer_list<T> list) : container_{list} {}

  // Assignments

  PriorityQueue& operator=(const PriorityQueue& other) {
    container_ = other.container_;
    return *this;
  }

  PriorityQueue& operator=(PriorityQueue&& other) noexcept {
    container_ = std::move(other.container_);
    return *this;
  }

  PriorityQueue& operator=(const std::initializer_list<T> list) {
    container_ = list;
    return *this;
  }

  // Element access

  const_reference Top() const { return container_.Top(); }

  // Capacity

  bool Empty() const noexcept { return container_.Empty(); }

  size_type Size() const noexcept { return container_.Size(); }

  // Modifiers

  void Push(const value_type& value) { container_.Insert(value); }

  void Pop() { container_.Pop(); }

  void Swap(PriorityQueue& other) { container_.Swap(other.container_); }

  // Debug

  friend std::ostream& operator<<(
      std::ostream& os, const PriorityQueue& priority_queue) noexcept {
    os << priority_queue.container_;
    return os;
  }

 private:
  BinaryHeap<T, Compare, Allocator> container_;
};

#endif  // CPP_ALGORITHMS_DATA_STRUCTURES_PRIORITY_QUEUE_PRIORITY_QUEUE_H_