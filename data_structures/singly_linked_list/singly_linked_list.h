#ifndef CPP_ALGORITHMS_DATA_STRUCTURES_LINKED_LIST_LINKED_LIST_H_
#define CPP_ALGORITHMS_DATA_STRUCTURES_LINKED_LIST_LINKED_LIST_H_

#include <cstddef>
#include <initializer_list>
#include <iterator>
#include <memory>
#include <ostream>
#include <type_traits>
#include <utility>

#include "is_iterator.h"

template <class T, class Allocator = std::allocator<T>>
class SinglyLinkedList {
 private:
  template <class U>
  struct Node {
    U value{};
    Node<U>* next{nullptr};
  };

  class Iterator {
   public:
    using iterator_category = std::forward_iterator_tag;
    using value_type = T;
    using difference_type = std::ptrdiff_t;
    using pointer = T*;
    using reference = T&;

    Iterator(Node<T>* node) noexcept : node_{node} {}

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

    bool operator==(const Iterator& other) const noexcept {
      return node_ == other.node_;
    }

    bool operator!=(const Iterator& other) const noexcept {
      return !(*this == other);
    }

   private:
    Node<T>* node_;

    friend class SinglyLinkedList;
  };

  class ConstIterator {
   public:
    using iterator_category = std::forward_iterator_tag;
    using value_type = T;
    using difference_type = std::ptrdiff_t;
    using pointer = const T*;
    using reference = const T&;

    ConstIterator(Node<T>* node) noexcept : node_{node} {}

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

    bool operator==(const ConstIterator& other) const noexcept {
      return node_ == other.node_;
    }

    bool operator!=(const ConstIterator& other) const noexcept {
      return !(*this == other);
    }

   private:
    Node<T>* node_;

    friend class SinglyLinkedList;
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

  // Constructors

  SinglyLinkedList() noexcept {
    NodeAllocator node_allocator;
    head_ = node_allocator.allocate(1);
  }

  SinglyLinkedList(const SinglyLinkedList& other) : SinglyLinkedList() {
    if (other.size_ == 0) return;

    size_ = other.size_;

    Node<T>* prev_node{new Node<T>{other.head_->next->value}};
    head_->next = prev_node;

    Node<T>* other_node{other.head_->next->next};
    while (other_node != nullptr) {
      Node<T>* const node{new Node<T>{other_node->value}};
      prev_node->next = node;
      prev_node = node;
      other_node = other_node->next;
    }
  }

  SinglyLinkedList(SinglyLinkedList&& other) noexcept {
    TakeContent(std::move(other));
  }

  SinglyLinkedList(const std::initializer_list<T> list) : SinglyLinkedList() {
    if (list.size() == 0) return;

    size_ = list.size();
    const T* it{list.begin()};

    Node<T>* prev_node{new Node<T>{*it}};
    head_->next = prev_node;

    while (++it != list.end()) {
      Node<T>* const node{new Node<T>{*it}};
      prev_node->next = node;
      prev_node = node;
    }
  }

  ~SinglyLinkedList() {
    Clear();
    NodeAllocator node_allocator;
    node_allocator.deallocate(head_, 1);
  }

  // Assignments

  SinglyLinkedList& operator=(const SinglyLinkedList& other) {
    SinglyLinkedList temp{other};
    std::swap(*this, temp);
    return *this;
  }

  SinglyLinkedList& operator=(SinglyLinkedList&& other) noexcept {
    if (this == &other) return *this;

    Clear();
    NodeAllocator node_allocator;
    node_allocator.deallocate(head_, 1);
    TakeContent(std::move(other));

    return *this;
  }

  SinglyLinkedList& operator=(const std::initializer_list<T> list) {
    SinglyLinkedList temp{list};
    std::swap(*this, temp);
    return *this;
  }

  // Element access

  reference Front() { return head_->next->value; }
  const_reference Front() const { return head_->next->value; }

  // Iterators

  iterator before_begin() noexcept { return iterator(head_); }
  const_iterator before_begin() const noexcept { return const_iterator(head_); }
  const_iterator cbefore_begin() const noexcept { return before_begin(); }

  iterator begin() noexcept { return iterator(head_->next); }
  const_iterator begin() const noexcept { return const_iterator(head_->next); }
  const_iterator cbegin() const noexcept { return begin(); }

  iterator end() noexcept { return iterator(nullptr); }
  const_iterator end() const noexcept { return const_iterator(nullptr); }
  const_iterator cend() const noexcept { return end(); }

  // Capacity

  bool Empty() const noexcept { return size_ == 0; }

  size_type Size() const noexcept { return size_; }

  // Modifiers

  void Clear() noexcept {
    if (size_ == 0) return;

    Node<T>* node{head_->next};
    while (node != nullptr) {
      Node<T>* const temp{node->next};
      delete node;
      node = temp;
    }

    size_ = 0;
    head_->next = nullptr;
  }

  iterator InsertAfter(const_iterator position, const_reference value) {
    return InsertAfter(position, {value});
  }

  iterator InsertAfter(const_iterator position, size_type count,
                       const_reference value) {
    if (count == 0) return iterator(position.node_);

    Node<T>* prev_node{position.node_};
    Node<T>* const next_node{prev_node->next};

    Node<T>* node{nullptr};
    for (std::size_t i{0}; i < count; ++i) {
      node = new Node<T>{value};
      prev_node->next = node;
      prev_node = node;
    }

    node->next = next_node;
    size_ += count;

    return iterator(node);
  }

  template <class InputIterator,
            std::enable_if_t<is_iterator<InputIterator>, bool> = true>
  iterator InsertAfter(const_iterator position, const InputIterator first,
                       const InputIterator last) {
    if (first == last) return iterator(position.node_);

    Node<T>* prev_node{position.node_};
    Node<T>* const next_node{prev_node->next};

    std::size_t distance{0};
    Node<T>* node{nullptr};

    for (InputIterator it{first}; it != last; ++it) {
      node = new Node<T>{*it};
      prev_node->next = node;
      prev_node = node;
      ++distance;
    }

    node->next = next_node;
    size_ += distance;

    return iterator(node);
  }

  iterator InsertAfter(const_iterator position,
                       const std::initializer_list<T> list) {
    return InsertAfter(position, list.begin(), list.end());
  }

  iterator EraseAfter(const_iterator position) {
    return EraseAfter(position, std::next(std::next(position)));
  }

  iterator EraseAfter(const_iterator first, const_iterator last) {
    if (first == last || first.node_->next == last.node_)
      return iterator(last.node_);

    Node<T>* const prev_node{first.node_};

    std::size_t distance{0};
    Node<T>* node{prev_node->next};

    while (node != last.node_) {
      Node<T>* const next_node{node->next};
      delete node;
      node = next_node;
      ++distance;
    }

    prev_node->next = node;
    size_ -= distance;

    return iterator(node);
  }

  void Erase(const size_type index) { Erase(index, index + 1); }

  void Erase(const size_type first, const size_type last) {
    const std::size_t distance{last - first};
    if (distance == 0) return;

    Node<T>* const prev_node{first == 0 ? head_ : NodeAt(first - 1)};
    Node<T>* node{prev_node->next};

    for (std::size_t i{0}; i < distance; ++i) {
      Node<T>* const next_node{node->next};
      delete node;
      node = next_node;
    }

    prev_node->next = node;
    size_ -= distance;
  }

  void PushFront(const_reference value) {
    Node<T>* const node{new Node<T>{value, head_->next}};
    head_->next = node;
    size_ += 1;
  }

  void PopFront() {
    Node<T>* const node{head_->next->next};
    delete head_->next;
    head_->next = node;
    size_ -= 1;
  }

  void Resize(const size_type new_size, const_reference value = T()) {
    if (size_ > new_size) {
      Erase(new_size, size_);
    } else if (size_ < new_size) {
      auto it{const_iterator(size_ == 0 ? head_ : NodeAt(size_ - 1))};
      const std::size_t count{new_size - size_};
      InsertAfter(it, count, value);
    }
  }

  void Swap(SinglyLinkedList& other) noexcept {
    std::swap(size_, other.size_);
    std::swap(head_, other.head_);
  }

  // Operations

  void SpliceAfter(const_iterator position, SinglyLinkedList& other) {
    if (other.size_ == 0) return;

    Node<T>* const prev_node{position.node_};
    Node<T>* const next_node{prev_node->next};

    Node<T>* const other_first{other.head_->next};
    prev_node->next = other_first;
    other.NodeAt(other.size_ - 1)->next = next_node;

    size_ += other.size_;
    other.size_ = 0;
    other.head_->next = nullptr;
  }

  void SpliceAfter(const_iterator position, SinglyLinkedList& other,
                   const_iterator it) {
    SpliceAfter(position, other, it, std::next(std::next(it)));
  }

  void SpliceAfter(const_iterator position, SinglyLinkedList& other,
                   const_iterator first, const_iterator last) {
    if (first == last || first.node_->next == last.node_) return;

    Node<T>* const other_prev_node{first.node_};
    Node<T>* const other_first_node{other_prev_node->next};

    Node<T>* other_last_node{other_first_node};
    std::size_t distance{1};

    while (other_last_node->next != last.node_) {
      other_last_node = other_last_node->next;
      ++distance;
    }

    Node<T>* const other_next_node{last.node_};
    other_prev_node->next = other_next_node;

    Node<T>* const prev_node{position.node_};
    Node<T>* const next_node{prev_node->next};
    prev_node->next = other_first_node;
    other_last_node->next = next_node;

    size_ += distance;
    other.size_ -= distance;
  }

  void Reverse() noexcept {
    Node<T>* prev_node{nullptr};
    Node<T>* current_node{head_->next};
    Node<T>* next_node{nullptr};

    while (current_node != nullptr) {
      next_node = current_node->next;
      current_node->next = prev_node;
      prev_node = current_node;
      current_node = next_node;
    }

    head_->next = prev_node;
  }

  // Comparison operators

  bool operator==(const SinglyLinkedList& other) const noexcept {
    if (size_ != other.size_) return false;

    for (auto it{begin()}, other_it{other.begin()}; it != nullptr;
         ++it, ++other_it) {
      if (*it != *other_it) return false;
    }
    return true;
  }

  bool operator!=(const SinglyLinkedList& other) const noexcept {
    return !(*this == other);
  }

  bool operator<(const SinglyLinkedList& other) const noexcept {
    if (size_ != other.size_) return size_ < other.size_;

    for (auto it{begin()}, other_it{other.begin()}; it != nullptr;
         ++it, ++other_it) {
      if (*it < *other_it) return true;
    }
    return false;
  }

  bool operator<=(const SinglyLinkedList& other) const noexcept {
    return !(other < *this);
  }

  bool operator>(const SinglyLinkedList& other) const noexcept {
    return other < *this;
  }

  bool operator>=(const SinglyLinkedList& other) const noexcept {
    return !(*this < other);
  }

  // Debug

  friend std::ostream& operator<<(std::ostream& os,
                                  const SinglyLinkedList& list) noexcept {
    os << "[";
    if (list.size_ != 0) {
      SinglyLinkedList::Node<T>* node{list.head_->next};
      for (std::size_t i{0}; i < list.size_ - 1; ++i) {
        os << node->value << ", ";
        node = node->next;
      }
      os << node->value;
    }
    os << "] (" << list.size_ << ")\n";
    return os;
  }

 private:
  using NodeAllocator =
      typename std::allocator_traits<Allocator>::template rebind_alloc<Node<T>>;

  Node<T>* const NodeAt(std::size_t index) {
    return const_cast<Node<T>* const>(std::as_const(*this).NodeAt(index));
  }
  const Node<T>* const NodeAt(std::size_t index) const {
    Node<T>* node{head_->next};
    for (std::size_t i{0}; i < index; ++i) {
      node = node->next;
    }
    return node;
  }

  void TakeContent(SinglyLinkedList&& other) noexcept {
    NodeAllocator allocator;
    size_ = other.size_;
    head_ = other.head_;
    other.size_ = 0;
    other.head_ = allocator.allocate(1);
    other.head_->next = nullptr;
  }

  std::size_t size_{0};
  Node<T>* head_;
};

#endif  // CPP_ALGORITHMS_DATA_STRUCTURES_LINKED_LIST_LINKED_LIST_H_