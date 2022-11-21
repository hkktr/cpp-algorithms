#ifndef CPP_ALGORITHMS_DATA_STRUCTURES_HASH_MAP_HASH_MAP_H_
#define CPP_ALGORITHMS_DATA_STRUCTURES_HASH_MAP_HASH_MAP_H_

#include <algorithm>
#include <cmath>
#include <cstddef>
#include <functional>
#include <initializer_list>
#include <iterator>
#include <ostream>
#include <stdexcept>
#include <type_traits>
#include <utility>

#include "doubly_linked_list.h"
#include "dynamic_array.h"

template <class Key, class T, class Hash = std::hash<Key>>
class HashMap {
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

  HashMap() noexcept = default;

  HashMap(const HashMap& other) {
    max_load_factor_ = other.max_load_factor_;
    Reserve(other.Size());

    for (const value_type& value : other) {
      InsertUnchecked(value);
    }
  }

  HashMap(HashMap&& other) noexcept {
    Swap(other);
    other.max_load_factor_ = 1;
  }

  HashMap(const std::initializer_list<value_type> list) { Insert(list); }

  // Assignments

  HashMap& operator=(const HashMap& other) {
    if (this == &other) return *this;

    Clear();
    CheckRehash(other.Size());

    for (const value_type& value : other) {
      InsertUnchecked(value);
    }

    return *this;
  }

  HashMap& operator=(HashMap&& other) noexcept {
    if (this == &other) return *this;

    Clear();
    Swap(other);
    other.max_load_factor_ = 1;

    return *this;
  }

  HashMap& operator=(const std::initializer_list<value_type> list) {
    Clear();
    CheckRehash(list.size());

    for (const value_type& value : list) {
      if (Contains(value.first)) continue;
      InsertUnchecked(value);
    }

    return *this;
  }

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

  void Clear() noexcept {
    elements_.Clear();
    for (auto& bucket : buckets_) {
      bucket = {end(), end()};
    }
  }

  std::pair<iterator, bool> Insert(const value_type& value) {
    if (const iterator existing_it{Find(value.first)}; existing_it != end())
      return {existing_it, false};

    CheckRehash(1);
    const iterator it{InsertUnchecked(value)};
    return {it, true};
  }

  template <class InputIterator,
            std::enable_if_t<is_iterator<InputIterator>, bool> = false>
  void Insert(const InputIterator first, const InputIterator last) {
    if (first == last) return;

    const std::size_t distance{
        static_cast<std::size_t>(std::distance(first, last))};
    CheckRehash(distance);

    for (InputIterator it{first}; it != last; ++it) {
      if (Contains(it->first)) continue;
      InsertUnchecked(*it);
    }
  }

  void Insert(const std::initializer_list<value_type> list) {
    Insert(list.begin(), list.end());
  }

  std::pair<iterator, bool> InsertOrAssign(const Key& key, const T& value) {
    if (const iterator existing_it{Find(key)}; existing_it != end()) {
      existing_it->second = value;
      return {existing_it, false};
    }

    CheckRehash(1);
    const iterator it{InsertUnchecked({key, value})};
    return {it, true};
  }

  iterator Erase(const const_iterator position) {
    const iterator it{Find(position->first)};

    auto& bucket{buckets_[Bucket(it->first)]};
    if (bucket.first == it) {
      bucket.first = std::next(it);
    } else if (bucket.second == it) {
      bucket.second = std::prev(it);
    }

    if (bucket.first == bucket.second) {
      bucket.first = bucket.second = end();
    }

    const iterator next_it{elements_.Erase(it)};
    return next_it;
  }

  iterator Erase(const const_iterator first, const const_iterator last) {
    const iterator first_it{Find(first->first)};
    const iterator last_it{Find(last->first)};

    iterator next_it{first_it};
    do {
      next_it = Erase(next_it);
    } while (next_it != last_it);

    return next_it;
  }

  size_type Erase(const Key& key) {
    auto it{Find(key)};
    if (it == end()) return 0;

    Erase(it);
    return 1;
  }

  void Swap(HashMap& other) noexcept {
    std::swap(elements_, other.elements_);
    std::swap(buckets_, other.buckets_);
  }

  // Lookup

  reference At(const Key& key) {
    return const_cast<reference>(std::as_const(*this).At(key));
  }
  const_reference At(const Key& key) const {
    const const_iterator it{Find(key)};
    if (it == end()) throw std::out_of_range("key out of bounds");
    return *it;
  }

  reference operator[](const Key& key) {
    if (const iterator it{Find(key)}; it != end()) return *it;
    const iterator it{(Insert({key, T()}).first)};
    return *it;
  }

  size_type Count(const Key& key) const { return Contains(key) ? 1 : 0; }

  iterator Find(const Key& key) {
    if (Empty()) return end();

    const auto& bucket{buckets_[Bucket(key)]};
    if (bucket.first == bucket.second) return end();

    for (auto it{bucket.first}; it != bucket.second; ++it) {
      if (it->first == key) return it;
    }
    return end();
  }
  const_iterator Find(const Key& key) const {
    if (Empty()) return end();

    const auto& bucket{buckets_[Bucket(key)]};
    if (bucket.first == bucket.second) return end();

    for (auto it{bucket.first}; it != bucket.second; ++it) {
      if (it->first == key) return const_iterator(it);
    }
    return end();
  }

  bool Contains(const Key& key) const {
    return Find(key) == end() ? false : true;
  }

  std::pair<iterator, iterator> EqualRange(const Key& key) {
    const iterator it{Find(key)};
    return it == end() ? std::make_pair(it, it)
                       : std::make_pair(it, std::next(it));
  }
  std::pair<const_iterator, const_iterator> EqualRange(const Key& key) const {
    const const_iterator it{Find(key)};
    return it == end() ? std::make_pair(it, it)
                       : std::make_pair(it, std::next(it));
  }

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

  size_type BucketCount() const { return buckets_.Size(); }

  size_type BucketSize(const size_type n) const {
    const auto bucket{buckets_[n]};
    return std::distance(bucket.first, bucket.second);
  }

  size_type Bucket(const Key& key) const { return Hash{}(key) % BucketCount(); }

  // Hash policy

  float LoadFactor() const { return Empty() ? 0 : Size() / BucketCount(); }

  float MaxLoadFactor() const { return max_load_factor_; }
  void MaxLoadFactor(const float max_load_factor) {
    max_load_factor_ = max_load_factor;
  }

  void Rehash(const size_type count) {
    const std::size_t min_count{
        static_cast<std::size_t>(std::ceil(Size() / max_load_factor_))};
    const std::size_t new_size{std::max(min_count, count)};

    DoublyLinkedList<value_type> new_elements;
    DynamicArray<std::pair<iterator, iterator>> new_buckets;
    new_buckets.Resize(new_size, {new_elements.end(), new_elements.end()});

    DoublyLinkedList<value_type> old_elements{std::move(elements_)};
    elements_ = std::move(new_elements);
    buckets_ = std::move(new_buckets);

    for (const value_type& pair : old_elements) {
      InsertUnchecked(pair);
    }
  }

  void Reserve(const size_type count) {
    Rehash(std::ceil(count / max_load_factor_));
  }

  // Comparison operators

  bool operator==(const HashMap& other) const noexcept {
    if (Size() != other.Size()) return false;

    for (const value_type& value : *this) {
      const const_iterator it{other.Find(value.first)};
      if (it == other.end() || it->second != value.second) return false;
    }
    return true;
  }

  bool operator!=(const HashMap& other) const noexcept {
    return !(*this == other);
  }

  // Debug

  friend std::ostream& operator<<(std::ostream& os,
                                  const HashMap& hash_map) noexcept {
    os << "[";

    if (!hash_map.Empty()) {
      for (auto it{hash_map.begin()}; it != std::prev(hash_map.end()); ++it) {
        const auto& [key, value] = *it;
        const std::size_t bucket{hash_map.Bucket(key)};
        os << key << " -> " << value << " (" << bucket << "), ";
      }

      const auto& [key, value] = *std::prev(hash_map.end());
      const std::size_t bucket{hash_map.Bucket(key)};
      os << key << " -> " << value << " (" << bucket << ")";
    }

    os << "] (" << hash_map.Size() << ", buckets: " << hash_map.BucketCount()
       << ")\n";
    return os;
  }

 private:
  iterator InsertUnchecked(const value_type& value) {
    auto& bucket{buckets_[Bucket(value.first)]};
    iterator it{elements_.Insert(bucket.first, value)};

    if (bucket.first == bucket.second) {
      bucket.first = it;
      bucket.second = std::next(it);
    } else {
      bucket.first = it;
    }

    return it;
  }

  void CheckRehash(const std::size_t additional) {
    const std::size_t new_size{Size() + additional};
    if (new_size > max_load_factor_ * BucketCount())
      Rehash(std::max(new_size, Size() * 2));
  }

  DoublyLinkedList<value_type> elements_;
  DynamicArray<std::pair<iterator, iterator>> buckets_;
  float max_load_factor_{1.0};
};

#endif  // CPP_ALGORITHMS_DATA_STRUCTURES_HASH_MAP_HASH_MAP_H_