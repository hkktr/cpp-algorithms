#ifndef CPP_ALGORITHMS_DATA_STRUCTURES_DEQUE_DEQUE_H
#define CPP_ALGORITHMS_DATA_STRUCTURES_DEQUE_DEQUE_H

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
class Deque {
 private:
  class Iterator {
   public:
    using iterator_category = std::random_access_iterator_tag;
    using value_type = T;
    using difference_type = std::ptrdiff_t;
    using pointer = T*;
    using reference = T&;

    Iterator(Deque* container, const std::size_t offset = 0) noexcept
        : container_{container}, offset_{offset} {}

    reference operator*() const noexcept { return (*container_)[offset_]; }

    pointer operator->() const noexcept { return &(**this); }

    Iterator& operator++() noexcept {
      ++offset_;
      return *this;
    }

    Iterator operator++(int) noexcept {
      Iterator temp{*this};
      ++(*this);
      return temp;
    }

    Iterator& operator--() noexcept {
      --offset_;
      return *this;
    }

    Iterator operator--(int) noexcept {
      Iterator temp{*this};
      --(*this);
      return temp;
    }

    Iterator& operator+=(const difference_type offset) noexcept {
      offset_ += offset;
      return *this;
    }

    Iterator operator+(const difference_type offset) const noexcept {
      Iterator temp{*this};
      temp += offset;
      return temp;
    }

    Iterator& operator-=(const difference_type offset) noexcept {
      offset_ -= offset;
      return *this;
    }

    Iterator operator-(const difference_type offset) const noexcept {
      Iterator temp{*this};
      temp -= offset;
      return temp;
    }

    difference_type operator-(const Iterator& other) const noexcept {
      return offset_ - other.offset_;
    }

    reference operator[](const difference_type offset) const noexcept {
      return *(*this + offset);
    }

    bool operator==(const Iterator& other) const noexcept {
      return offset_ == other.offset_;
    }

    bool operator!=(const Iterator& other) const noexcept {
      return !(*this == other);
    }

    bool operator<(const Iterator& other) const noexcept {
      return offset_ < other.offset_;
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
    Deque* container_;
    std::size_t offset_;

    friend class Deque;
  };

  class ConstIterator {
   public:
    using iterator_category = std::random_access_iterator_tag;
    using value_type = T;
    using difference_type = std::ptrdiff_t;
    using pointer = const T*;
    using reference = const T&;

    ConstIterator(const Deque* const container,
                  const std::size_t offset = 0) noexcept
        : container_{container}, offset_{offset} {}

    ConstIterator(const Iterator iterator) noexcept
        : container_{iterator.container_}, offset_{iterator.offset_} {}

    reference operator*() const noexcept { return (*container_)[offset_]; }

    pointer operator->() const noexcept { return &(**this); }

    ConstIterator& operator++() noexcept {
      ++offset_;
      return *this;
    }

    ConstIterator operator++(int) noexcept {
      ConstIterator temp{*this};
      ++(*this);
      return temp;
    }

    ConstIterator& operator--() noexcept {
      --offset_;
      return *this;
    }

    ConstIterator operator--(int) noexcept {
      ConstIterator temp{*this};
      --(*this);
      return temp;
    }

    ConstIterator& operator+=(const difference_type offset) noexcept {
      offset_ += offset;
      return *this;
    }

    ConstIterator operator+(const difference_type offset) const noexcept {
      ConstIterator temp{*this};
      temp += offset;
      return temp;
    }

    ConstIterator& operator-=(const difference_type offset) noexcept {
      offset_ -= offset;
      return *this;
    }

    ConstIterator operator-(const difference_type offset) const noexcept {
      ConstIterator temp{*this};
      temp -= offset;
      return temp;
    }

    difference_type operator-(const ConstIterator& other) const noexcept {
      return offset_ - other.offset_;
    }

    reference operator[](const difference_type offset) const noexcept {
      return *(*this + offset);
    }

    bool operator==(const ConstIterator& other) const noexcept {
      return offset_ == other.offset_;
    }

    bool operator!=(const ConstIterator& other) const noexcept {
      return !(*this == other);
    }

    bool operator<(const ConstIterator& other) const noexcept {
      return offset_ < other.offset_;
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
    const Deque* const container_;
    std::size_t offset_;

    friend class Deque;
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

  Deque() = default;

  Deque(const Deque& other) {
    Grow(Position::kBack, other.size_);
    size_ = other.size_;

    for (std::size_t i{0}; i < size_; ++i) {
      (*this)[i] = other[i];
    }
  }

  Deque(Deque&& other) noexcept { TakeContent(std::move(other)); }

  Deque(std::initializer_list<T> list) {
    Grow(Position::kBack, list.size());
    size_ = list.size();

    std::size_t i{0};
    for (const T& value : list) {
      (*this)[i] = value;
      ++i;
    }
  }

  ~Deque() {
    Allocator allocator;
    for (std::size_t i{0}; i < map_capacity_; ++i) {
      allocator.deallocate(map_[i], kBlockSize);
    }
    delete[] map_;
  }

  // Assignments

  Deque& operator=(const Deque& other) {
    if (this == &other) return *this;

    Clear();
    Grow(Position::kBack, other.size_);
    size_ = other.size_;

    for (std::size_t i{0}; i < size_; ++i) {
      (*this)[i] = other[i];
    }

    return *this;
  }

  Deque& operator=(Deque&& other) noexcept {
    if (this == &other) return *this;

    Clear();
    TakeContent(std::move(other));

    return *this;
  }

  Deque& operator=(const std::initializer_list<T> list) {
    Clear();
    Grow(Position::kBack, list.size());

    std::size_t i{0};
    for (const T& value : list) {
      (*this)[i] = value;
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
    return (*this)[index];
  }

  reference operator[](const size_type index) {
    return const_cast<reference>(std::as_const(*this)[index]);
  }
  const_reference operator[](const size_type index) const {
    return map_[(map_head_ + (block_head_ + index) / kBlockSize) %
                map_capacity_][(block_head_ + index) % kBlockSize];
  }

  reference Front() { return (*this)[0]; }
  const_reference Front() const { return (*this)[0]; }

  reference Back() { return (*this)[size_ - 1]; }
  const_reference Back() const { return (*this)[size_ - 1]; }

  // Iterators

  iterator begin() noexcept { return iterator(this); }
  const_iterator begin() const noexcept { return const_iterator(this); }
  const_iterator cbegin() const noexcept { return begin(); }

  iterator end() noexcept { return iterator(this, size_); }
  const_iterator end() const noexcept { return const_iterator(this, size_); }
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

  void ShrinkToFit() {
    const std::size_t required_blocks{(size_ - 1) / kBlockSize + 1};
    const std::size_t free_space{required_blocks * kBlockSize - size_};
    const std::size_t block_head{free_space / 2};
    const std::size_t block_tail{((size_ - 1) % kBlockSize + block_head_) %
                                 kBlockSize};

    Allocator allocator;
    T** const map{new T*[required_blocks]};

    for (std::size_t i{0}; i < required_blocks; ++i) {
      map[i] = allocator.allocate(kBlockSize);
    }

    for (std::size_t i{0}; i < size_; ++i) {
      map[(block_head_ + i) / kBlockSize][(block_head_ + i) % kBlockSize] =
          std::move((*this)[i]);
    }

    for (std::size_t i{0}; i < map_capacity_; ++i) {
      allocator.deallocate(map_[i], kBlockSize);
    }
    delete[] map_;

    map_ = map;
    map_capacity_ = required_blocks;
    map_size_ = required_blocks;
    map_head_ = 0;
    block_head_ = block_head;
    block_tail_ = block_tail;
  }

  // Modifiers

  void Clear() noexcept {
    Allocator allocator;
    for (std::size_t i{0}; i < map_size_; ++i) {
      allocator.deallocate(map_[(map_head_ + i) % map_capacity_], kBlockSize);
      map_[(map_head_ + i) % map_capacity_] = allocator.allocate(kBlockSize);
    }

    map_size_ = 0;
    map_head_ = 0;
    size_ = 0;
    block_head_ = 0;
    block_tail_ = 0;
  }

  iterator Insert(const_iterator position, const_reference value) {
    return Insert(position, {value});
  }

  iterator Insert(const_iterator position, const size_type count,
                  const_reference value) {
    if (count == 0) return iterator(this, position.offset_);

    const std::size_t index{
        static_cast<std::size_t>(std::distance(cbegin(), position))};
    const Position insert_position{
        index < size_ + count - index - 1 ? Position::kFront : Position::kBack};

    Grow(insert_position, count);
    size_ += count;

    if (insert_position == Position::kFront) {
      for (std::size_t i{count}; i < index + count; ++i) {
        (*this)[i - count] = std::move((*this)[i]);
      }
    } else if (insert_position == Position::kBack) {
      for (std::size_t i{size_ - 1}; i >= index + count; --i) {
        (*this)[i] = std::move((*this)[i - count]);
      }
    }

    std::size_t i{index};
    for (std::size_t j{0}; j < count; ++j) {
      (*this)[i] = value;
      ++i;
    }

    return iterator(this, index);
  }

  template <class InputIterator,
            std::enable_if_t<is_iterator<InputIterator>, bool> = true>
  iterator Insert(const_iterator position, const InputIterator first,
                  const InputIterator last) {
    if (first == last) return iterator(this, position.offset_);

    const std::size_t index{
        static_cast<std::size_t>(std::distance(cbegin(), position))};
    const std::size_t distance{
        static_cast<std::size_t>(std::distance(first, last))};

    const Position insert_position{index < size_ + distance - index - 1
                                       ? Position::kFront
                                       : Position::kBack};

    Grow(insert_position, distance);
    size_ += distance;

    if (insert_position == Position::kFront) {
      for (std::size_t i{distance}; i < index + distance; ++i) {
        (*this)[i - distance] = std::move((*this)[i]);
      }
    } else if (insert_position == Position::kBack) {
      for (std::size_t i{size_ - 1}; i >= index + distance; --i) {
        (*this)[i] = std::move((*this)[i - distance]);
      }
    }

    std::size_t i{index};
    for (auto it{first}; it != last; ++it) {
      (*this)[i] = *it;
      ++i;
    }

    return iterator(this, index);
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
    if (distance == 0) return iterator(this, last.offset_);

    const std::size_t first_index{
        static_cast<std::size_t>(std::distance(cbegin(), first))};
    const std::size_t last_index{first_index + distance};

    for (std::size_t i{first_index}; i < last_index; ++i) {
      (*this)[i].~T();
    }

    const Position position{first_index < size_ + distance - last_index - 1
                                ? Position::kFront
                                : Position::kBack};

    if (position == Position::kFront) {
      for (std::size_t i{last_index - 1}; i >= distance; --i) {
        (*this)[i] = std::move((*this)[i - distance]);
      }

      const std::size_t erased_blocks{(distance + block_head_) / kBlockSize};
      const std::size_t offset{distance % kBlockSize};
      map_size_ -= erased_blocks;

      map_head_ = (map_head_ + erased_blocks) % map_capacity_;
      block_head_ = (block_head_ + offset) % kBlockSize;
    } else if (position == Position::kBack) {
      for (std::size_t i{first_index}; i + distance < size_; ++i) {
        (*this)[i] = std::move((*this)[i + distance]);
      }

      std::size_t erased_blocks{distance / kBlockSize +
                                (distance % kBlockSize > block_tail_)};
      const std::size_t offset{distance % kBlockSize};
      map_size_ -= erased_blocks;

      if (block_tail_ >= offset) {
        block_tail_ = block_tail_ - offset;
      } else {
        block_tail_ = kBlockSize - (offset - block_tail_);
      }
    }

    size_ -= distance;
    return last_index == size_ ? end() : iterator(this, last_index - distance);
  }

  void PushBack(const_reference value) {
    Grow(Position::kBack, 1);
    (*this)[size_] = value;
    ++size_;
  }

  void PopBack() { Erase(cend() - 1); }

  void PushFront(const_reference value) {
    Grow(Position::kFront, 1);
    (*this)[0] = value;
    ++size_;
  }

  void PopFront() { Erase(cbegin()); }

  void Resize(const size_type new_size, const_reference value = T()) {
    if (new_size < size_) {
      Erase(const_iterator(this, new_size), cend());
    } else if (new_size > size_) {
      Insert(cend(), new_size - size_, value);
    }
  }

  void Swap(Deque& other) noexcept {
    std::swap(map_, other.map_);
    std::swap(map_capacity_, other.map_capacity_);
    std::swap(map_size_, other.map_size_);
    std::swap(map_head_, other.map_head_);
    std::swap(size_, other.size_);
    std::swap(block_head_, other.block_head_);
    std::swap(block_tail_, other.block_tail_);
  }

  // Comparison operators

  bool operator==(const Deque& other) const noexcept {
    if (size_ != other.size_) return false;
    for (std::size_t i{0}; i < size_; ++i) {
      if ((*this)[i] != other[i]) return false;
    }
    return true;
  }

  bool operator!=(const Deque& other) const noexcept {
    return !(*this == other);
  }

  bool operator<(const Deque& other) const noexcept {
    if (size_ != other.size_) return size_ < other.size_;
    for (std::size_t i{0}; i < size_; ++i) {
      if (!((*this)[i] < other[i])) return false;
    }
    return true;
  }

  bool operator<=(const Deque& other) const noexcept {
    return !(*this > other);
  }

  bool operator>(const Deque& other) const noexcept { return other < *this; }

  bool operator>=(const Deque& other) const noexcept {
    return !(*this < other);
  }

  // Debug

  friend std::ostream& operator<<(std::ostream& os,
                                  const Deque& deque) noexcept {
    os << "[";
    if (deque.size_ != 0) {
      for (std::size_t i{0}; i < deque.size_ - 1; ++i) {
        os << deque[i] << ", ";
      }
      os << deque[deque.size_ - 1];
    }
    os << "] (" << deque.size_ << ", map head: " << deque.map_head_
       << ", map size: " << deque.map_size_
       << ", map cap: " << deque.map_capacity_
       << ", block head: " << deque.block_head_
       << ", block tail: " << deque.block_tail_
       << ", block size: " << deque.kBlockSize << ")\n";
    return os;
  }

 private:
  enum class Position { kFront, kBack };

  static constexpr std::size_t kBytes{sizeof(T)};
  static constexpr std::size_t kBlockSize{kBytes <= 1   ? 16
                                          : kBytes <= 2 ? 8
                                          : kBytes <= 4 ? 4
                                          : kBytes <= 8 ? 2
                                                        : 1};

  void Grow(const Position position, const std::size_t size) {
    std::size_t free_block_space;
    if (map_capacity_ == 0) {
      free_block_space = 0;
    } else if (position == Position::kFront) {
      free_block_space = block_head_;
    } else if (position == Position::kBack) {
      free_block_space = kBlockSize - (block_tail_ + 1);
    }

    if (size <= free_block_space) {
      if (position == Position::kFront) {
        block_head_ -= size;
      } else if (position == Position::kBack) {
        block_tail_ += size;
      }
      return;
    }

    auto update_head_tail = [=]() -> void {
      if (size_ == 0) {
        const std::size_t used_blocks{(size - 1) / kBlockSize + 1};
        const std::size_t free_space{used_blocks * kBlockSize - size};
        block_head_ = free_space / 2;
        block_tail_ = ((size - 1) % kBlockSize + block_head_) % kBlockSize;
        return;
      }

      const std::size_t offset{size % kBlockSize};
      if (position == Position::kFront) {
        if (block_head_ >= offset) {
          block_head_ = block_head_ - offset;
        } else {
          block_head_ = kBlockSize - (offset - block_head_);
        }
      } else if (position == Position::kBack) {
        block_tail_ = (block_tail_ + offset) % kBlockSize;
      }
    };

    const std::size_t required_space{size - free_block_space};
    const std::size_t new_blocks{(required_space - 1) / kBlockSize + 1};

    if (new_blocks <= map_capacity_ - map_size_) {
      GrowBlocks(position, new_blocks);
      update_head_tail();
      return;
    }

    const std::size_t new_capacity{
        std::max(2 * map_capacity_, map_capacity_ + new_blocks)};
    GrowMap(new_capacity);
    GrowBlocks(position, new_blocks);
    update_head_tail();
  }

  void GrowMap(const std::size_t new_capacity) {
    T** new_map_{new T*[new_capacity]};
    for (std::size_t i{0}; i < map_size_; ++i) {
      new_map_[i] = map_[(map_head_ + i) % map_capacity_];
    }

    Allocator allocator;
    for (std::size_t i{map_size_}; i < new_capacity; ++i) {
      new_map_[i] = allocator.allocate(kBlockSize);
    }

    delete[] map_;
    map_ = new_map_;
    map_capacity_ = new_capacity;
    map_head_ = 0;
  }

  void GrowBlocks(const Position position, std::size_t size) {
    map_size_ += size;

    if (position == Position::kFront) {
      if (size > map_head_) {
        size -= map_head_ + 1;
        map_head_ = map_capacity_ - 1;
      }
      map_head_ -= size;
    }
  }

  void TakeContent(Deque&& other) noexcept {
    map_ = other.map_;
    map_capacity_ = other.map_capacity_;
    map_size_ = other.map_size_;
    map_head_ = other.map_head_;
    size_ = other.size_;
    block_head_ = other.block_head_;
    block_tail_ = other.block_tail_;
    other.map_ = nullptr;
    other.map_capacity_ = 0;
    other.map_size_ = 0;
    other.map_head_ = 0;
    other.size_ = 0;
    other.block_head_ = 0;
    other.block_tail_ = 0;
  }

  T** map_{nullptr};
  std::size_t map_capacity_{0};
  std::size_t map_size_{0};
  std::size_t map_head_{0};
  std::size_t size_{0};
  std::size_t block_head_{0};
  std::size_t block_tail_{0};
};

#endif  // CPP_ALGORITHMS_DATA_STRUCTURES_DEQUE_DEQUE_H