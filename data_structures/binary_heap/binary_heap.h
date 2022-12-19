#ifndef CPP_ALGORITHMS_DATA_STRUCTURES_BINARY_HEAP_BINARY_HEAP_H_
#define CPP_ALGORITHMS_DATA_STRUCTURES_BINARY_HEAP_BINARY_HEAP_H_

#include <cstddef>
#include <functional>
#include <initializer_list>
#include <ostream>
#include <utility>

#include "dynamic_array.h"
#include "is_iterator.h"

template <class T, class Compare = std::less<T>,
          class Allocator = std::allocator<T>>
class BinaryHeap {
 public:
  using value_compare = Compare;
  using value_type = T;
  using allocator_type = Allocator;
  using size_type = std::size_t;
  using difference_type = std::ptrdiff_t;
  using const_reference = const value_type&;
  using pointer = value_type*;
  using const_pointer = const value_type*;
  using const_iterator = typename DynamicArray<T, Allocator>::const_iterator;

  // Constructors

  BinaryHeap() noexcept = default;

  BinaryHeap(const BinaryHeap& other) : array_{other.array_} {}

  BinaryHeap(BinaryHeap&& other) noexcept : array_{std::move(other.array_)} {}

  BinaryHeap(const std::initializer_list<T> list) : array_{list} {
    HeapifyArray();
  }

  // Assignments

  BinaryHeap& operator=(const BinaryHeap& other) {
    if (this == &other) return *this;

    array_ = other.array_;

    return *this;
  }

  BinaryHeap& operator=(BinaryHeap&& other) noexcept {
    if (this == &other) return *this;

    array_ = std::move(other.array_);

    return *this;
  }

  BinaryHeap& operator=(const std::initializer_list<T> list) {
    array_ = list;
    HeapifyArray();

    return *this;
  }

  // Element access

  const_reference Top() const { return array_[0]; }

  pointer Data() noexcept { return array_.Data(); }
  const_pointer Data() const noexcept { return array_.Data(); }

  // Iterators

  const_iterator cbegin() const noexcept { return array_.cbegin(); }
  const_iterator cend() const noexcept { return array_.cend(); }

  // Capacity

  bool Empty() const noexcept { return array_.Empty(); }

  size_type Size() const noexcept { return array_.Size(); }

  void Reserve(const size_type new_capacity) { array_.Reserve(new_capacity); }

  size_type Capacity() const noexcept { return array_.Capacity(); }

  void ShrinkToFit() { array_.ShrinkToFit(); }

  // Modifiers

  void Clear() noexcept { array_.Clear(); }

  void Insert(const_reference value) { Insert({value}); }

  template <class InputIterator,
            std::enable_if_t<is_iterator<InputIterator>, bool> = true>
  void Insert(const InputIterator first, const InputIterator last) {
    array_.Insert(array_.end(), first, last);
    HeapifyArray();
  }

  void Insert(const std::initializer_list<T> list) {
    Insert(list.begin(), list.end());
  }

  void Erase(const_reference value) { Erase({value}); }

  template <class InputIterator,
            std::enable_if_t<is_iterator<InputIterator>, bool> = true>
  void Erase(const InputIterator first, const InputIterator last) {
    for (InputIterator it{first}; it != last; ++it) {
      for (std::size_t i{0}; i < Size(); ++i) {
        if (array_[i] == *it) {
          std::swap(array_[i], array_[Size() - 1]);
          array_.PopBack();
          break;
        }
      }
    }

    HeapifyArray();
  }

  void Erase(const std::initializer_list<T> list) {
    Erase(list.begin(), list.end());
  }

  void Pop() {
    std::swap(array_[0], array_[Size() - 1]);
    array_.PopBack();
    HeapifyArray();
  }

  void Swap(BinaryHeap& other) noexcept { std::swap(array_, other.array_); }

  // Comparison operators

  bool operator==(const BinaryHeap& other) const noexcept {
    return array_ == other.array_;
  }

  bool operator!=(const BinaryHeap& other) const noexcept {
    return array_ != other.array_;
  }

  // Debug

  friend std::ostream& operator<<(std::ostream& os,
                                  const BinaryHeap& binary_heap) noexcept {
    os << binary_heap.array_;
    return os;
  }

 private:
  void HeapifyArray() {
    if (Size() <= 1) return;

    const std::size_t start{Size() / 2 - 1};
    for (std::size_t i{start}; i <= start; --i) {
      Heapify(i);
    }
  }

  void Heapify(const std::size_t index) {
    const std::size_t left{2 * index + 1};
    const std::size_t right{2 * index + 2};
    std::size_t largest{index};

    if (left < Size() && !Compare{}(array_[left], array_[largest])) {
      largest = left;
    }

    if (right < Size() && !Compare{}(array_[right], array_[largest])) {
      largest = right;
    }

    if (largest != index) {
      std::swap(array_[index], array_[largest]);
      Heapify(largest);
    }
  }

  DynamicArray<T, Allocator> array_;
};

#endif  // CPP_ALGORITHMS_DATA_STRUCTURES_BINARY_HEAP_BINARY_HEAP_H_