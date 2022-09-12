#ifndef CPP_ALGORITHMS_DATA_STRUCTURES_ARRAY_ARRAY_H_
#define CPP_ALGORITHMS_DATA_STRUCTURES_ARRAY_ARRAY_H_

#include <cstddef>
#include <initializer_list>
#include <iterator>
#include <ostream>
#include <stdexcept>
#include <utility>

template <class T, std::size_t N>
class Array {
 private:
  class Iterator {
   public:
    using iterator_category = std::random_access_iterator_tag;
    using value_type = T;
    using difference_type = std::ptrdiff_t;
    using pointer = T*;
    using reference = T&;

    Iterator(pointer ptr, const std::size_t offset = 0) noexcept
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
  };

  class ConstIterator {
   public:
    using iterator_category = std::random_access_iterator_tag;
    using value_type = T;
    using difference_type = std::ptrdiff_t;
    using pointer = const T*;
    using reference = const T&;

    ConstIterator(pointer ptr, const std::size_t offset = 0) noexcept
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
    const T* ptr_;
  };

 public:
  using value_type = T;
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

  constexpr Array() = default;

  constexpr Array(const Array& other) = default;

  constexpr Array(const std::initializer_list<T> list) {
    std::size_t i{0};
    for (const T& element : list) {
      (*this)[i] = element;
      ++i;
    }
  }

  // Assignments

  constexpr Array& operator=(const Array& other) = default;

  constexpr Array& operator=(const std::initializer_list<T> list) {
    std::size_t i{0};
    for (const T& element : list) {
      (*this)[i] = element;
      ++i;
    }
    return *this;
  }

  // Element access

  constexpr reference At(const size_type index) {
    return const_cast<reference>(std::as_const(*this).At(index));
  }
  constexpr const_reference At(const size_type index) const {
    if (index >= N) throw std::out_of_range("index out of bounds");
    return (*this)[index];
  }

  constexpr reference operator[](const size_type index) {
    return const_cast<reference>(std::as_const(*this)[index]);
  }
  constexpr const_reference operator[](const size_type index) const {
    return data_[index];
  }

  constexpr reference Front() { return (*this)[0]; }
  constexpr const_reference Front() const { return (*this)[0]; }

  constexpr reference Back() { return (*this)[N - 1]; }
  constexpr const_reference Back() const { return (*this)[N - 1]; }

  constexpr pointer Data() noexcept { return data_; }
  constexpr const_pointer Data() const noexcept { return data_; }

  // Iterators

  constexpr iterator begin() noexcept { return iterator(data_); }
  constexpr const_iterator begin() const noexcept {
    return const_iterator(data_);
  }
  constexpr const_iterator cbegin() const noexcept { return begin(); }

  constexpr iterator end() noexcept { return iterator(data_, N); }
  constexpr const_iterator end() const noexcept {
    return const_iterator(data_, N);
  }
  constexpr const_iterator cend() const noexcept { return end(); }

  constexpr reverse_iterator rbegin() noexcept {
    return reverse_iterator(end());
  }
  constexpr const_reverse_iterator rbegin() const noexcept {
    return const_reverse_iterator(end());
  }
  constexpr const_reverse_iterator crbegin() const noexcept { return rbegin(); }

  constexpr reverse_iterator rend() noexcept {
    return reverse_iterator(begin());
  }
  constexpr const_reverse_iterator rend() const noexcept {
    return const_reverse_iterator(begin());
  }
  constexpr const_reverse_iterator crend() const noexcept { return rend(); }

  // Capacity

  constexpr bool Empty() const noexcept { return N == 0; }

  constexpr size_type Size() const noexcept { return N; }

  // Operations

  void Fill(const_reference value) {
    for (std::size_t i{0}; i < N; ++i) {
      (*this)[i] = value;
    }
  }

  void Swap(Array& other) noexcept {
    for (std::size_t i{0}; i < N; ++i) {
      std::swap((*this)[i], other[i]);
    }
  }

  // Comparison operators

  bool operator==(const Array& other) const noexcept {
    for (std::size_t i{0}; i < N; ++i) {
      if ((*this)[i] != other[i]) return false;
    }
    return true;
  }

  bool operator!=(const Array& other) const noexcept {
    return !(*this == other);
  }

  bool operator<(const Array& other) const noexcept {
    for (std::size_t i{0}; i < N; ++i) {
      if (!((*this)[i] < other[i])) return false;
    }
    return true;
  }

  bool operator<=(const Array& other) const noexcept {
    return !(*this > other);
  }

  bool operator>(const Array& other) const noexcept { return other < *this; }

  bool operator>=(const Array& other) const noexcept {
    return !(*this < other);
  }

  // Debug

  friend std::ostream& operator<<(std::ostream& os,
                                  const Array& array) noexcept {
    os << "[";
    if (N != 0) {
      for (std::size_t i{0}; i < N - 1; ++i) {
        os << array[i] << ", ";
      }
      os << array[N - 1];
    }

    os << "] (" << N << ")\n";
    return os;
  }

 private:
  T data_[N]{};
};

#endif  // CPP_ALGORITHMS_DATA_STRUCTURES_ARRAY_ARRAY_H_
