#ifndef CPP_ALGORITHMS_DATA_STRUCTURES_DYNAMIC_ARRAY_DYNAMIC_ARRAY_H_
#define CPP_ALGORITHMS_DATA_STRUCTURES_DYNAMIC_ARRAY_DYNAMIC_ARRAY_H_

#include <algorithm>
#include <cstddef>
#include <initializer_list>
#include <iterator>
#include <memory>
#include <ostream>
#include <stdexcept>
#include <type_traits>
#include <utility>

#include "is_iterator.h"

template <class T, class Allocator = std::allocator<T>>
class DynamicArray {
 private:
  class Iterator {
   public:
    using iterator_category = std::random_access_iterator_tag;
    using value_type = T;
    using difference_type = std::ptrdiff_t;
    using pointer = T*;
    using reference = T&;

    Iterator(T* ptr, const std::size_t offset = 0) noexcept
        : ptr_{ptr + offset} {}

    reference operator*() const noexcept { return *ptr_; }

    pointer operator->() const noexcept { return ptr_; }

    Iterator& operator++() noexcept {
      ++ptr_;
      return *this;
    }

    Iterator operator++(int) noexcept {
      Iterator temp{*this};
      ++(*this);
      return temp;
    }

    Iterator& operator--() noexcept {
      --ptr_;
      return *this;
    }

    Iterator operator--(int) noexcept {
      Iterator temp{*this};
      --(*this);
      return temp;
    }

    Iterator& operator+=(const difference_type offset) noexcept {
      ptr_ += offset;
      return *this;
    }

    Iterator operator+(const difference_type offset) const noexcept {
      Iterator temp{*this};
      temp += offset;
      return temp;
    }

    Iterator& operator-=(const difference_type offset) noexcept {
      ptr_ -= offset;
      return *this;
    }

    Iterator operator-(const difference_type offset) const noexcept {
      Iterator temp{*this};
      temp -= offset;
      return temp;
    }

    difference_type operator-(const Iterator& other) const noexcept {
      return ptr_ - other.ptr_;
    }

    reference operator[](const difference_type offset) const noexcept {
      return ptr_[offset];
    }

    bool operator==(const Iterator& other) const noexcept {
      return ptr_ == other.ptr_;
    }

    bool operator!=(const Iterator& other) const noexcept {
      return !(*this == other);
    }

    bool operator<(const Iterator& other) const noexcept {
      return ptr_ < other.ptr_;
    }

    bool operator<=(const Iterator& other) const noexcept {
      return !(*this > other);
    }

    bool operator>(const Iterator& other) const noexcept {
      return other < *this;
    }

    bool operator>=(const Iterator& other) const noexcept {
      return !(*this < other);
    }

   private:
    T* ptr_;

    friend class DynamicArray;
  };

  class ConstIterator {
   public:
    using iterator_category = std::random_access_iterator_tag;
    using value_type = T;
    using difference_type = std::ptrdiff_t;
    using pointer = const T*;
    using reference = const T&;

    ConstIterator(T* ptr, const std::size_t offset = 0) noexcept
        : ptr_{ptr + offset} {}

    ConstIterator(const Iterator iterator) noexcept : ptr_{iterator.ptr_} {}

    reference operator*() const noexcept { return *ptr_; }

    pointer operator->() const noexcept { return ptr_; }

    ConstIterator& operator++() noexcept {
      ++ptr_;
      return *this;
    }

    ConstIterator operator++(int) noexcept {
      ConstIterator temp{*this};
      ++(*this);
      return temp;
    }

    ConstIterator& operator--() noexcept {
      --ptr_;
      return *this;
    }

    ConstIterator operator--(int) noexcept {
      ConstIterator temp{*this};
      --(*this);
      return temp;
    }

    ConstIterator& operator+=(const difference_type offset) noexcept {
      ptr_ += offset;
      return *this;
    }

    ConstIterator operator+(const difference_type offset) const noexcept {
      ConstIterator temp{*this};
      temp += offset;
      return temp;
    }

    ConstIterator& operator-=(const difference_type offset) noexcept {
      ptr_ -= offset;
      return *this;
    }

    ConstIterator operator-(const difference_type offset) const noexcept {
      ConstIterator temp{*this};
      temp -= offset;
      return temp;
    }

    difference_type operator-(const ConstIterator& other) const noexcept {
      return ptr_ - other.ptr_;
    }

    reference operator[](const difference_type offset) const noexcept {
      return ptr_[offset];
    }

    bool operator==(const ConstIterator& other) const noexcept {
      return ptr_ == other.ptr_;
    }

    bool operator!=(const ConstIterator& other) const noexcept {
      return !(*this == other);
    }

    bool operator<(const ConstIterator& other) const noexcept {
      return ptr_ < other.ptr_;
    }

    bool operator<=(const ConstIterator& other) const noexcept {
      return !(*this > other);
    }

    bool operator>(const ConstIterator& other) const noexcept {
      return other < *this;
    }

    bool operator>=(const ConstIterator& other) const noexcept {
      return !(*this < other);
    }

   private:
    T* ptr_;

    friend class DynamicArray;
  };

 public:
  using value_type = T;
  using allocator_type = Allocator;
  using size_type = std::size_t;
  using difference_type = std::ptrdiff_t;
  using reference = value_type&;
  using const_reference = const value_type&;
  using pointer = value_type*;
  using const_pointer = const value_type*;
  using iterator = Iterator;
  using const_iterator = ConstIterator;
  using reverse_iterator = std::reverse_iterator<iterator>;
  using const_reverse_iterator = std::reverse_iterator<const_iterator>;

  // Constructors

  DynamicArray() noexcept = default;

  DynamicArray(const DynamicArray& other) {
    Insert(cbegin(), other.cbegin(), other.cend());
  }

  DynamicArray(DynamicArray&& other) noexcept { TakeContent(std::move(other)); }

  DynamicArray(const std::initializer_list<T> list) { Insert(cbegin(), list); }

  ~DynamicArray() {
    Allocator allocator;
    allocator.deallocate(data_, capacity_);
  }

  // Assignments

  DynamicArray& operator=(const DynamicArray& other) {
    if (this == &other) return *this;

    Reserve(other.size_);
    for (std::size_t i{0}; i < other.size_; ++i) {
      data_[i] = other[i];
    }
    size_ = other.size_;

    return *this;
  }

  DynamicArray& operator=(DynamicArray&& other) noexcept {
    if (this == &other) return *this;

    Allocator allocator;
    allocator.deallocate(data_, capacity_);
    TakeContent(std::move(other));

    return *this;
  }

  DynamicArray& operator=(const std::initializer_list<T> list) {
    Reserve(list.size());

    std::size_t i{0};
    for (const T& value : list) {
      data_[i] = value;
      ++i;
    }
    size_ = list.size();

    return *this;
  }

  // Element access

  reference At(const size_type index) {
    return const_cast<reference>(std::as_const(*this).At(index));
  }
  const_reference At(const size_type index) const {
    if (index >= size_) throw std::out_of_range("index out of bounds");
    return data_[index];
  }

  reference operator[](const size_type index) { return data_[index]; }
  const_reference operator[](const size_type index) const {
    return data_[index];
  }

  reference Front() { return data_[0]; }
  const_reference Front() const { return data_[0]; }

  reference Back() { return data_[size_ - 1]; }
  const_reference Back() const { return data_[size_ - 1]; }

  pointer Data() noexcept { return data_; }
  const_pointer Data() const noexcept { return data_; }

  // Iterators

  iterator begin() noexcept { return iterator(data_); }
  const_iterator begin() const noexcept { return const_iterator(data_); }
  const_iterator cbegin() const noexcept { return begin(); }

  iterator end() noexcept { return iterator(data_, size_); }
  const_iterator end() const noexcept { return const_iterator(data_, size_); }
  const_iterator cend() const noexcept { return end(); }

  reverse_iterator rbegin() noexcept { return reverse_iterator(end()); }
  const_reverse_iterator rbegin() const noexcept {
    return const_reverse_iterator(end());
  }
  const_reverse_iterator crbegin() const noexcept { return rbegin(); }

  reverse_iterator rend() noexcept { return reverse_iterator(begin()); }
  const_reverse_iterator rend() const noexcept {
    return const_reverse_iterator(begin());
  }
  const_reverse_iterator crend() const noexcept { return rend(); }

  // Capacity

  bool Empty() const noexcept { return size_ == 0; }

  size_type Size() const noexcept { return size_; }

  void Reserve(const size_type new_capacity) {
    if (new_capacity <= capacity_) return;
    ForceReserve(new_capacity);
  }

  size_type Capacity() const noexcept { return capacity_; }

  void ShrinkToFit() {
    if (capacity_ == size_) return;
    ForceReserve(size_);
  }

  // Modifiers

  void Clear() {
    Allocator allocator;
    allocator.deallocate(data_, capacity_);
    data_ = allocator.allocate(capacity_);
    size_ = 0;
  }

  iterator Insert(const_iterator position, const_reference value) {
    return Insert(position, {value});
  }

  iterator Insert(const_iterator position, const size_type count,
                  const_reference value) {
    if (count == 0) return iterator(position.ptr_);

    const std::size_t index{
        static_cast<std::size_t>(std::distance(cbegin(), position))};
    const std::size_t new_size{size_ + count};

    if (new_size > capacity_) {
      const std::size_t new_capacity{std::max(capacity_ * 2, new_size)};

      Allocator allocator;
      T* const new_data{allocator.allocate(new_capacity)};
      std::size_t i{0};

      for (std::size_t old_i{0}; i < index; ++old_i, ++i) {
        new_data[i] = std::move(data_[old_i]);
      }
      for (std::size_t j{0}; j < count; ++j) {
        new_data[i] = value;
        ++i;
      }
      for (std::size_t old_i{index}; old_i < size_; ++old_i, ++i) {
        new_data[i] = std::move(data_[old_i]);
      }

      allocator.deallocate(data_, capacity_);
      data_ = new_data;
      capacity_ = new_capacity;
    } else {
      for (std::size_t i{size_ + count - 1}; i > index + count - 1; --i) {
        std::swap(data_[i - count], data_[i]);
      }

      std::size_t i{index};
      for (std::size_t j{0}; j < count; ++j) {
        data_[i] = value;
        ++i;
      }
    }

    size_ = new_size;
    return iterator(data_, index);
  }

  template <class InputIterator,
            std::enable_if_t<is_iterator<InputIterator>, bool> = true>
  iterator Insert(const_iterator position, const InputIterator first,
                  const InputIterator last) {
    if (first == last) return iterator(position.ptr_);

    const std::size_t index{
        static_cast<std::size_t>(std::distance(cbegin(), position))};
    const std::size_t distance{
        static_cast<std::size_t>(std::distance(first, last))};
    const std::size_t new_size{size_ + distance};

    if (new_size > capacity_) {
      const std::size_t new_capacity{std::max(capacity_ * 2, new_size)};

      Allocator allocator;
      T* const new_data{allocator.allocate(new_capacity)};
      std::size_t i{0};

      for (std::size_t old_i{0}; i < index; ++old_i, ++i) {
        new_data[i] = std::move(data_[old_i]);
      }
      for (auto it{first}; it != last; ++it) {
        new_data[i] = *it;
        ++i;
      }
      for (std::size_t old_i{index}; old_i < size_; ++old_i, ++i) {
        new_data[i] = std::move(data_[old_i]);
      }

      allocator.deallocate(data_, capacity_);
      data_ = new_data;
      capacity_ = new_capacity;
    } else {
      for (std::size_t i{size_ + distance - 1}; i > index + distance - 1; --i) {
        std::swap(data_[i - distance], data_[i]);
      }

      std::size_t i{index};
      for (auto it{first}; it != last; ++it) {
        data_[i] = *it;
        ++i;
      }
    }

    size_ = new_size;
    return iterator(data_, index);
  }

  iterator Insert(const_iterator position,
                  const std::initializer_list<T> list) {
    return Insert(position, list.begin(), list.end());
  }

  iterator Erase(const_iterator position) {
    return Erase(position, position + 1);
  }

  iterator Erase(const_iterator first, const_iterator last) {
    if (first == cend()) return end();

    const std::size_t distance{
        static_cast<std::size_t>(std::distance(first, last))};
    if (distance == 0) return iterator(last.ptr_);

    const std::size_t first_index{
        static_cast<std::size_t>(std::distance(cbegin(), first))};
    const std::size_t last_index{first_index + distance};

    for (std::size_t i{first_index}; i < last_index; ++i) {
      data_[i].~T();
    }

    for (std::size_t i{first_index}; i + distance < size_; ++i) {
      data_[i] = std::move(data_[i + distance]);
    }
    size_ -= distance;

    return last_index == size_ ? end() : iterator(data_, last_index - distance);
  }

  void PushBack(const_reference value) {
    if (size_ == capacity_) Grow();
    data_[size_] = value;
    ++size_;
  }

  void PopBack() {
    data_[size_ - 1].~T();
    --size_;
  }

  void Resize(const size_type new_size, const_reference value = T()) {
    if (new_size < size_) {
      Erase(const_iterator(data_, new_size), cend());
    } else {
      Insert(cend(), new_size - size_, value);
    }
  }

  void Swap(DynamicArray& other) noexcept {
    std::swap(data_, other.data_);
    std::swap(capacity_, other.capacity_);
    std::swap(size_, other.size_);
  }

  // Comparison operators

  bool operator==(const DynamicArray& other) const noexcept {
    if (size_ != other.size_) return false;

    for (std::size_t i{0}; i < size_; ++i) {
      if ((*this)[i] != other[i]) return false;
    }
    return true;
  }

  bool operator!=(const DynamicArray& other) const noexcept {
    return !(*this == other);
  }

  bool operator<(const DynamicArray& other) const noexcept {
    if (size_ != other.size_) return size_ < other.size_;

    for (std::size_t i{0}; i < other.size_; ++i) {
      if (!((*this)[i] < other[i])) return false;
    }
    return true;
  }

  bool operator<=(const DynamicArray& other) const noexcept {
    return !(*this > other);
  }

  bool operator>(const DynamicArray& other) const noexcept {
    return other < *this;
  }

  bool operator>=(const DynamicArray& other) const noexcept {
    return !(*this < other);
  }

  // Debug

  friend std::ostream& operator<<(std::ostream& os,
                                  const DynamicArray<T>& array) noexcept {
    os << "[";
    if (array.size_ != 0) {
      for (std::size_t i{0}; i < array.size_ - 1; ++i) {
        os << array[i] << ", ";
      }
      os << array[array.size_ - 1];
    }
    os << "] (" << array.size_ << ", cap: " << array.capacity_ << ")\n";
    return os;
  }

 private:
  void ForceReserve(std::size_t new_capacity) {
    Allocator allocator;
    T* const new_data{allocator.allocate(new_capacity)};

    for (std::size_t i{0}; i < size_; ++i) {
      new_data[i] = std::move(data_[i]);
    }

    allocator.deallocate(data_, capacity_);
    data_ = new_data;
    capacity_ = new_capacity;
  }

  void Grow() { ForceReserve(capacity_ == 0 ? 1 : capacity_ * 2); }

  void TakeContent(DynamicArray&& other) noexcept {
    data_ = other.data_;
    capacity_ = other.capacity_;
    size_ = other.size_;
    other.data_ = nullptr;
    other.capacity_ = 0;
    other.size_ = 0;
  }

  T* data_{nullptr};
  std::size_t capacity_{0};
  std::size_t size_{0};
};

#endif  // CPP_ALGORITHMS_DATA_STRUCTURES_DYNAMIC_ARRAY_DYNAMIC_ARRAY_H_