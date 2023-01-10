#ifndef CPP_ALGORITHMS_DATA_STRUCTURES_HASH_TABLE_HASH_TABLE_H_
#define CPP_ALGORITHMS_DATA_STRUCTURES_HASH_TABLE_HASH_TABLE_H_

#include <doubly_linked_list.h>
#include <dynamic_array.h>

#include <algorithm>
#include <cstddef>
#include <functional>
#include <initializer_list>
#include <iterator>
#include <type_traits>
#include <utility>

template <class Key, class T, class Hash = std::hash<Key>>
class HashTable {
 public:
  using key_type = Key;
  using mapped_type = T;
  using value_type = std::pair<const Key, T>;
  using size_type = std::size_t;
  using difference_type = std::ptrdiff_t;
  using hasher = Hash;
  using reference = value_type&;
  using const_reference = const value_type&;
  using pointer = value_type*;
  using const_pointer = const value_type*;
  using iterator = typename DoublyLinkedList<value_type>::iterator;
  using const_iterator = typename DoublyLinkedList<value_type>::const_iterator;
  using local_iterator = iterator;
  using const_local_iterator = const_iterator;

  // Constructors

  // TODO: Default constructor
  HashTable() noexcept = default;

  // TODO: Copy constructor
  HashTable(const HashTable& other);

  // TODO: Move constructor
  HashTable(HashTable&& other) noexcept;

  // TODO: Initializer list constructor
  HashTable(const std::initializer_list<value_type> list);

  // Iterators

  iterator begin() noexcept { return elements_.begin(); }
  const_iterator begin() const noexcept { return elements_.begin(); }
  const_iterator cbegin() const noexcept { return elements_.cbegin(); }

  iterator end() noexcept { return elements_.end(); }
  const_iterator end() const noexcept { return elements_.end(); }
  const_iterator cend() const noexcept { return elements_.cend(); }

  // Capacity

  bool Empty() const noexcept { return elements_.Empty(); }

  size_type Size() const noexcept { return elements_.Size(); }

  // Modifiers

  // TODO: Clear
  void Clear() noexcept;

  // TODO: Insert_Value
  // std::pair<iterator, bool> Insert(const value_type& value);

  // TODO: Insert_Range
  template <class InputIterator,
            std::enable_if_t<is_iterator<InputIterator>, bool> = false>
  void Insert(const InputIterator first, const InputIterator last);

  // TODO: Insert_InitializerList
  void Insert(const std::initializer_list<value_type> list);

  // TODO: InsertOrAssign
  // std::pair<iterator, bool> InsertOrAssign(const Key& key,
  //                                          const value_type& value);

  // TODO: Erase_Iterator
  // iterator Erase(const const_iterator position);

  // TODO: Erase_Range
  // iterator Erase(const const_iterator first, const const_iterator last);

  // TODO: Erase_Key
  size_type Erase(const Key& key);

  void Swap(HashTable& other) noexcept {
    std::swap(elements_, other.elements_);
    std::swap(buckets_, other.buckets_);
  }

  // Lookup

  // TODO: At
  reference At(const Key& key) {
    return const_cast<reference>(std::as_const(*this).At(key));
  }
  // TODO: At_Const
  const_reference At(const Key& key) const;

  // TODO: SubscriptOperator
  reference operator[](const Key& key) {
    return const_cast<reference>(std::as_const(*this)[key]);
  }
  // TODO: SubscriptOperator_Const
  const_reference operator[](const Key& key) const;

  // TODO: Count
  size_type Count(const Key& key) const;

  // TODO: Find
  // iterator Find(const Key& key);
  // TODO: Find_Const
  // const_iterator Find(const Key& key) const;

  // TODO: Contains
  bool Contains(const Key& key) const;

  // TODO: EqualRange
  // std::pair<iterator, iterator> EqualRange(const Key& key);
  // TODO: EqualRange_Const
  // std::pair<const_iterator, const_iterator> EqualRange(const Key& key) const;

  // Bucket interface

  local_iterator begin(const size_type n) noexcept { return buckets_[n].first; }
  const_local_iterator begin(const size_type n) const noexcept {
    return buckets_[n].first;
  }
  const_local_iterator cbegin(const size_type n) const noexcept {
    return begin(n);
  }

  local_iterator end(const size_type n) noexcept { return buckets_[n].second; }
  const_local_iterator end(const size_type n) const noexcept {
    return buckets_[n].second;
  }
  const_local_iterator cend(const size_type n) const noexcept { return end(n); }

  size_type BucketCount() const { return buckets_.Capacity(); }

  size_type BucketSize(const size_type n) const {
    const auto range{buckets_[n]};
    return std::distance(range.first, range.second);
  }

  size_type Bucket(const Key& key) const { return Hash{}(key) % BucketCount(); }

  // Hash policy

  float LoadFactor() const { return Size() / BucketCount(); }

  float MaxLoadFactor() const { return max_load_factor_; }
  void MaxLoadFactor(const float max_load_factor) {
    max_load_factor_ = max_load_factor;
  }

  // TODO: Rehash
  void Rehash(const size_type count);

  // TODO: Reserve
  void Reserve(const size_type count);

  // Observers

  hasher HashFunction() const { return Hash{}; }

 private:
  DoublyLinkedList<value_type> elements_;
  DynamicArray<std::pair<iterator, iterator>> buckets_;
  float max_load_factor_{1.0};
};

#endif  // CPP_ALGORITHMS_DATA_STRUCTURES_HASH_TABLE_HASH_TABLE_H_