#ifndef CPP_ALGORITHMS_DATA_STRUCTURES_DOUBLY_LINKED_LIST_DOUBLY_LINKED_LIST_H_
#define CPP_ALGORITHMS_DATA_STRUCTURES_DOUBLY_LINKED_LIST_DOUBLY_LINKED_LIST_H_

#include <cstddef>
#include <initializer_list>
#include <iostream>
#include <iterator>
#include <memory>
#include <ostream>
#include <type_traits>
#include <utility>

#include "is_iterator.h"

template <class T, class Allocator = std::allocator<T>>
class DoublyLinkedList {
 private:
  template <class U>
  struct Node {
    U value{};
    Node<U>* prev{nullptr};
    Node<U>* next{nullptr};
  };

  class Iterator {
   public:
    using iterator_category = std::bidirectional_iterator_tag;
    using value_type = T;
    using difference_type = std::ptrdiff_t;
    using pointer = T*;
    using reference = T&;

    Iterator(Node<T>* const node) noexcept : node_{node} {}

    reference operator*() const noexcept { return node_->value; }

    pointer operator->() const noexcept { return &node_->value; }

    Iterator& operator++() noexcept {
      node_ = node_->next;
      return *this;
    }

    Iterator operator++(int) noexcept {
      Iterator temp{*this};
      ++(*this);
      return temp;
    }

    Iterator& operator--() noexcept {
      node_ = node_->prev;
      return *this;
    }

    Iterator operator--(int) noexcept {
      Iterator temp{*this};
      --(*this);
      return temp;
    }

    bool operator==(const Iterator& other) const noexcept {
      return node_ == other.node_;
    }

    bool operator!=(const Iterator& other) const noexcept {
      return !(*this == other);
    }

   private:
    Node<T>* node_;

    friend class DoublyLinkedList;
  };

  class ConstIterator {
   public:
    using iterator_category = std::bidirectional_iterator_tag;
    using value_type = T;
    using difference_type = std::ptrdiff_t;
    using pointer = const T*;
    using reference = const T&;

    ConstIterator(Node<T>* const node) noexcept : node_{node} {}

    ConstIterator(const Iterator iterator) noexcept : node_{iterator.node_} {}

    reference operator*() const noexcept { return node_->value; }

    pointer operator->() const noexcept { return &node_->value; }

    ConstIterator& operator++() noexcept {
      node_ = node_->next;
      return *this;
    }

    ConstIterator operator++(int) noexcept {
      ConstIterator temp{*this};
      ++(*this);
      return temp;
    }

    ConstIterator& operator--() noexcept {
      node_ = node_->prev;
      return *this;
    }

    ConstIterator operator--(int) noexcept {
      ConstIterator temp{*this};
      --(*this);
      return temp;
    }

    bool operator==(const ConstIterator& other) const noexcept {
      return node_ == other.node_;
    }

    bool operator!=(const ConstIterator& other) const noexcept {
      return !(*this == other);
    }

   private:
    Node<T>* node_;

    friend class DoublyLinkedList;
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

  DoublyLinkedList() noexcept {
    NodeAllocator node_allocator;
    head_ = node_allocator.allocate(1);
    head_->prev = head_->next = head_;
  }

  DoublyLinkedList(const DoublyLinkedList& other) : DoublyLinkedList() {
    Insert(cbegin(), other.cbegin(), other.cend());
  }

  DoublyLinkedList(DoublyLinkedList&& other) { TakeContent(std::move(other)); }

  DoublyLinkedList(const std::initializer_list<T> list) : DoublyLinkedList() {
    Insert(cbegin(), list);
  }

  ~DoublyLinkedList() {
    Clear();
    NodeAllocator node_allocator;
    node_allocator.deallocate(head_, 1);
  }

  // Assignments

  DoublyLinkedList& operator=(const DoublyLinkedList& other) {
    DoublyLinkedList temp{other};
    std::swap(*this, temp);
    return *this;
  }

  DoublyLinkedList& operator=(DoublyLinkedList&& other) noexcept {
    if (this == &other) return *this;

    Clear();
    NodeAllocator node_allocator;
    node_allocator.deallocate(head_, 1);
    TakeContent(std::move(other));

    return *this;
  }

  DoublyLinkedList& operator=(const std::initializer_list<T> list) {
    DoublyLinkedList temp{list};
    std::swap(*this, temp);
    return *this;
  }

  // Element access

  reference Front() { return head_->next->value; }
  const_reference Front() const { return head_->next->value; }

  reference Back() { return head_->prev->value; }
  const_reference Back() const { return head_->prev->value; }

  // Iterators

  iterator begin() noexcept { return iterator(head_->next); }
  const_iterator begin() const noexcept { return const_iterator(head_->next); }
  const_iterator cbegin() const noexcept { return begin(); }

  iterator end() noexcept { return iterator(head_); }
  const_iterator end() const noexcept { return const_iterator(head_); }
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

  // Modifiers

  void Clear() noexcept {
    if (size_ == 0) return;

    Node<T>* node{head_->next};
    while (node != head_) {
      Node<T>* const temp{node->next};
      delete node;
      node = temp;
    }

    size_ = 0;
    head_->prev = head_->next = head_;
  }

  iterator Insert(const_iterator position, const_reference value) {
    return Insert(position, 1, value);
  }

  iterator Insert(const_iterator position, size_type count,
                  const_reference value) {
    if (count == 0) return iterator(position.node_);

    Node<T>* prev_node{size_ == 0 ? head_ : position.node_->prev};
    Node<T>* const next_node{position.node_ == nullptr ? head_
                                                       : position.node_};

    Node<T>* node{nullptr};
    for (std::size_t i{0}; i < count; ++i) {
      node = new Node<T>{value, prev_node};
      prev_node->next = node;
      prev_node = node;
    }

    node->next = next_node;
    next_node->prev = node;
    size_ += count;

    return iterator(node);
  }

  template <class InputIterator,
            std::enable_if_t<is_iterator<InputIterator>, bool> = true>
  iterator Insert(const_iterator position, const InputIterator first,
                  const InputIterator last) {
    if (first == last) return iterator(position.node_);

    Node<T>* prev_node{size_ == 0 ? head_ : position.node_->prev};
    Node<T>* const next_node{position.node_ == nullptr ? head_
                                                       : position.node_};

    std::size_t distance{0};
    Node<T>* node{nullptr};

    for (InputIterator it{first}; it != last; ++it) {
      node = new Node<T>{*it, prev_node};
      prev_node->next = node;
      prev_node = node;
      ++distance;
    }

    node->next = next_node;
    next_node->prev = node;
    size_ += distance;

    return iterator(node);
  }

  iterator Insert(const_iterator position,
                  const std::initializer_list<T> list) {
    return Insert(position, list.begin(), list.end());
  }

  iterator Erase(const_iterator position) {
    return Erase(position, std::next(position));
  }

  iterator Erase(const_iterator first, const_iterator last) {
    if (first == last) return iterator(first.node_);

    Node<T>* const prev_node{first.node_->prev};

    std::size_t distance{0};
    Node<T>* node{prev_node->next};

    while (node != last.node_) {
      Node<T>* const next_node{node->next};
      delete node;
      node = next_node;
      ++distance;
    }

    prev_node->next = node;
    node->prev = prev_node;
    size_ -= distance;

    return iterator(node);
  }

  void PushBack(const_reference value) { Insert(cend(), 1, value); }

  void PopBack() { Erase(std::prev(cend())); }

  void PushFront(const_reference value) { Insert(cbegin(), 1, value); }

  void PopFront() { Erase(cbegin()); }

  void Resize(const size_type new_size, const_reference value = T()) {
    if (size_ > new_size) {
      auto it{cbegin()};
      std::advance(it, new_size);
      Erase(it, cend());
    } else if (size_ < new_size) {
      const std::size_t distance{new_size - size_};
      Insert(cend(), distance, value);
    }
  }

  void Swap(DoublyLinkedList& other) noexcept {
    std::swap(size_, other.size_);
    std::swap(head_, other.head_);
  }

  // Operations

  void Reverse() noexcept {
    if (size_ == 0) return;

    Node<T>* current_node{head_->next};
    while (current_node != head_) {
      std::swap(current_node->prev, current_node->next);
      current_node = current_node->prev;
    }

    std::swap(head_->prev, head_->next);
  }

  void Splice(const_iterator position, DoublyLinkedList& other) {
    if (other.size_ == 0) return;

    Node<T>* const prev_node{position.node_ == nullptr ? head_
                                                       : position.node_->prev};
    Node<T>* const next_node{position.node_ == nullptr ? head_
                                                       : position.node_};

    Node<T>* const other_first_node{other.head_->next};
    other_first_node->prev = prev_node;
    prev_node->next = other_first_node;

    Node<T>* const other_last_node{other.head_->prev};
    other_last_node->next = next_node;
    next_node->prev = other_last_node;

    size_ += other.size_;
    other.size_ = 0;
    other.head_->prev = other.head_->next = nullptr;
  }

  void Splice(const_iterator position, DoublyLinkedList& other,
              const_iterator it) {
    Splice(position, other, it, std::next(it));
  }

  void Splice(const_iterator position, DoublyLinkedList& other,
              const_iterator first, const_iterator last) {
    if (first == last) return;

    const std::size_t distance{static_cast<std::size_t>(
        this == &other ? 0 : std::distance(first, last))};

    Node<T>* const other_prev_node{first.node_->prev};
    Node<T>* const other_first_node{first.node_};
    Node<T>* const other_last_node{last.node_->prev};
    Node<T>* const other_next_node{last.node_};

    other_prev_node->next = other_next_node;
    other_next_node->prev = other_prev_node;

    Node<T>* const prev_node{position.node_ == nullptr ? head_
                                                       : position.node_->prev};
    Node<T>* const next_node{position.node_ == nullptr ? head_
                                                       : position.node_};

    other_first_node->prev = prev_node;
    prev_node->next = other_first_node;

    other_last_node->next = next_node;
    next_node->prev = other_last_node;

    size_ += distance;
    other.size_ -= distance;
    if (other.size_ == 0) other.head_->prev = other.head_->next = nullptr;
  }

  // Comparison operators

  bool operator==(const DoublyLinkedList& other) const noexcept {
    if (size_ != other.size_) return false;
    if (size_ == 0) return true;

    for (auto it{begin()}, other_it{other.begin()}; it != cend();
         ++it, ++other_it) {
      if (*it != *other_it) return false;
    }
    return true;
  }

  bool operator!=(const DoublyLinkedList& other) const noexcept {
    return !(*this == other);
  }

  bool operator<(const DoublyLinkedList& other) const noexcept {
    if (size_ != other.size_) return size_ < other.size_;
    if (size_ == 0) return true;

    for (auto it{begin()}, other_it{other.begin()}; it != cend();
         ++it, ++other_it) {
      if (*it < *other_it) return true;
    }
    return false;
  }

  bool operator<=(const DoublyLinkedList& other) const noexcept {
    return !(other < *this);
  }

  bool operator>(const DoublyLinkedList& other) const noexcept {
    return other < *this;
  }

  bool operator>=(const DoublyLinkedList& other) const noexcept {
    return !(*this < other);
  }

  // Debug

  friend std::ostream& operator<<(std::ostream& os,
                                  const DoublyLinkedList& list) noexcept {
    os << "[";
    if (list.size_ != 0) {
      DoublyLinkedList::Node<T>* node{list.head_};
      for (std::size_t i{0}; i < list.size_ - 1; ++i) {
        os << node->value << ", ";
        node = node->next;
      }
      os << node->value;
    }
    os << "] (" << list.size_ << ")";
    return os;
  }

 private:
  using NodeAllocator =
      typename std::allocator_traits<Allocator>::template rebind_alloc<Node<T>>;

  void TakeContent(DoublyLinkedList&& other) noexcept {
    NodeAllocator node_allocator;
    size_ = other.size_;
    head_ = other.head_;
    other.size_ = 0;
    other.head_ = node_allocator.allocate(1);
    other.head_->prev = other.head_->next = other.head_;
  }

  std::size_t size_{0};
  Node<T>* head_;
};

#endif  // CPP_ALGORITHMS_DATA_STRUCTURES_DOUBLY_LINKED_LIST_DOUBLY_LINKED_LIST_H_