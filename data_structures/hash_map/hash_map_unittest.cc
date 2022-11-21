#include "hash_map.h"

#include <gtest/gtest.h>

#include <stdexcept>
#include <utility>

using Pair = std::pair<const int, int>;

template <class K, class T>
typename HashMap<K, T>::iterator At(HashMap<K, T>& hash_map,
                                    std::size_t index) {
  auto it{hash_map.begin()};
  std::advance(it, index);
  return it;
}

// Constructors

TEST(HashMapTest, Constructor) {
  const HashMap<int, int> hash_map;
  EXPECT_EQ(hash_map.Size(), 0);
  EXPECT_EQ(hash_map.BucketCount(), 0);
}

TEST(HashMapTest, CopyConstructor) {
  const HashMap<int, int> hash_map{{1, 1}, {2, 4}, {3, 9}};

  const HashMap<int, int> copy{hash_map};
  EXPECT_EQ(copy, hash_map);
}

TEST(HashMapTest, MoveConstructor) {
  HashMap<int, int> hash_map{{1, 1}, {2, 4}, {3, 9}};

  const HashMap<int, int> moved_hash_map{std::move(hash_map)};
  EXPECT_EQ(moved_hash_map.Size(), 3);
  EXPECT_EQ(hash_map.Size(), 0);
}

TEST(HashMapTest, InitializerListConstructor) {
  const HashMap<int, int> hash_map{{1, 1}, {2, 4}, {3, 9}};
  EXPECT_EQ(hash_map.Size(), 3);
  EXPECT_EQ(hash_map.At(1), (Pair{1, 1}));
  EXPECT_EQ(hash_map.At(2), (Pair{2, 4}));
  EXPECT_EQ(hash_map.At(3), (Pair{3, 9}));
}

// Assignments

TEST(HashMapTest, CopyAssignment) {
  const HashMap<int, int> hash_map{{1, 1}, {2, 4}, {3, 9}};
  HashMap<int, int> copy;

  copy = hash_map;
  EXPECT_EQ(copy, hash_map);
}

TEST(HashMapTest, MoveAssignment) {
  HashMap<int, int> hash_map{{1, 1}, {2, 4}, {3, 9}};
  HashMap<int, int> moved_hash_map;

  moved_hash_map = std::move(hash_map);
  EXPECT_EQ(moved_hash_map.Size(), 3);
  EXPECT_EQ(hash_map.Size(), 0);
}

TEST(HashMapTest, InitializerListAssigment) {
  HashMap<int, int> hash_map;

  hash_map = {{1, 1}, {2, 4}, {3, 9}};
  EXPECT_EQ(hash_map.Size(), 3);
  EXPECT_EQ(hash_map.At(1), (Pair{1, 1}));
  EXPECT_EQ(hash_map.At(2), (Pair{2, 4}));
  EXPECT_EQ(hash_map.At(3), (Pair{3, 9}));
}

// Iterators

TEST(HashMapTest, Begin) {
  HashMap<int, int> hash_map;
  EXPECT_EQ(hash_map.begin(), hash_map.end());

  hash_map = {{1, 1}, {2, 4}};
  auto it{hash_map.begin()};
  EXPECT_EQ(*it, (Pair{1, 1}));
  EXPECT_NE(it, hash_map.end());

  it->second = 10;
  EXPECT_EQ(hash_map.At(1), (Pair{1, 10}));

  ++it;
  EXPECT_EQ(*it, (Pair{2, 4}));
  EXPECT_NE(it, hash_map.end());

  ++it;
  EXPECT_EQ(it, hash_map.end());
}

TEST(HashMapTest, Begin_Const) {
  const HashMap<int, int> hash_map{{1, 1}, {2, 4}};

  auto it{hash_map.begin()};
  EXPECT_EQ(*it, (Pair{1, 1}));
  EXPECT_NE(it, hash_map.end());

  ++it;
  EXPECT_EQ(*it, (Pair{2, 4}));
  EXPECT_NE(it, hash_map.end());

  ++it;
  EXPECT_EQ(it, hash_map.end());
}

TEST(HashMapTest, Cbegin) {
  const HashMap<int, int> hash_map{{1, 1}, {2, 4}};

  auto it{hash_map.cbegin()};
  EXPECT_EQ(*it, (Pair{1, 1}));
  EXPECT_NE(it, hash_map.cend());

  ++it;
  EXPECT_EQ(*it, (Pair{2, 4}));
  EXPECT_NE(it, hash_map.cend());

  ++it;
  EXPECT_EQ(it, hash_map.cend());
}

TEST(HashMapTest, End) {
  HashMap<int, int> hash_map{{1, 1}};
  EXPECT_NE(hash_map.end(), hash_map.begin());
  EXPECT_EQ(hash_map.end(), ++hash_map.begin());
}

TEST(HashMapTest, End_Const) {
  const HashMap<int, int> hash_map{{1, 1}};
  EXPECT_NE(hash_map.end(), hash_map.begin());
  EXPECT_EQ(hash_map.end(), ++hash_map.begin());
}

TEST(HashMapTest, Cend) {
  const HashMap<int, int> hash_map{{1, 1}};
  EXPECT_NE(hash_map.cend(), hash_map.cbegin());
  EXPECT_EQ(hash_map.cend(), ++hash_map.cbegin());
}

// Capacity

TEST(HashMapTest, Empty) {
  const HashMap<int, int> empty_hash_map;
  EXPECT_TRUE(empty_hash_map.Empty());

  const HashMap<int, int> hash_map{{1, 1}, {2, 4}, {3, 9}};
  EXPECT_FALSE(hash_map.Empty());
}

TEST(HashMapTest, Size) {
  HashMap<int, int> hash_map;
  EXPECT_EQ(hash_map.Size(), 0);

  hash_map.Insert({1, 1});
  EXPECT_EQ(hash_map.Size(), 1);
}

// Modifiers

TEST(HashMapTest, Clear) {
  HashMap<int, int> hash_map;
  hash_map.Reserve(10);
  hash_map = {{1, 1}, {2, 4}, {3, 9}};

  EXPECT_EQ(hash_map.Size(), 3);
  EXPECT_EQ(hash_map.BucketCount(), 10);

  hash_map.Clear();
  EXPECT_EQ(hash_map.Size(), 0);
  EXPECT_EQ(hash_map.BucketCount(), 10);
}

TEST(HashMapTest, Insert_Value) {
  HashMap<int, int> hash_map;

  auto result{hash_map.Insert({1, 1})};
  EXPECT_EQ(*result.first, (Pair{1, 1}));
  EXPECT_EQ(result.second, true);
  EXPECT_EQ(hash_map.Size(), 1);

  result = hash_map.Insert({2, 4});
  EXPECT_EQ(*result.first, (Pair{2, 4}));
  EXPECT_EQ(result.second, true);
  EXPECT_EQ(hash_map.Size(), 2);

  result = hash_map.Insert({3, 9});
  EXPECT_EQ(*result.first, (Pair{3, 9}));
  EXPECT_EQ(result.second, true);
  EXPECT_EQ(hash_map.Size(), 3);

  result = hash_map.Insert({1, 0});
  EXPECT_EQ(*result.first, (Pair{1, 1}));
  EXPECT_EQ(result.second, false);
  EXPECT_EQ(hash_map.Size(), 3);

  result = hash_map.Insert({2, 0});
  EXPECT_EQ(*result.first, (Pair{2, 4}));
  EXPECT_EQ(result.second, false);
  EXPECT_EQ(hash_map.Size(), 3);

  result = hash_map.Insert({3, 0});
  EXPECT_EQ(*result.first, (Pair{3, 9}));
  EXPECT_EQ(result.second, false);
  EXPECT_EQ(hash_map.Size(), 3);
}

TEST(HashMapTest, Insert_Range) {
  const std::initializer_list<std::pair<const int, int>> source{
      {1, 1}, {2, 4}, {3, 9}, {4, 16}, {5, 25}, {6, 36}};
  HashMap<int, int> hash_map;

  hash_map.Insert(source.begin() + 0, source.begin() + 2);
  EXPECT_EQ(hash_map.Size(), 2);
  EXPECT_EQ(hash_map.At(1), *(source.begin() + 0));
  EXPECT_EQ(hash_map.At(2), *(source.begin() + 1));

  hash_map.Insert(source.begin() + 2, source.begin() + 4);
  EXPECT_EQ(hash_map.Size(), 4);
  EXPECT_EQ(hash_map.At(3), *(source.begin() + 2));
  EXPECT_EQ(hash_map.At(4), *(source.begin() + 3));

  hash_map.Insert(source.begin() + 4, source.begin() + 6);
  EXPECT_EQ(hash_map.Size(), 6);
  EXPECT_EQ(hash_map.At(5), *(source.begin() + 4));
  EXPECT_EQ(hash_map.At(6), *(source.begin() + 5));

  hash_map.Insert(source.begin(), source.begin() + 6);
  EXPECT_EQ(hash_map.Size(), 6);
}

TEST(HashMapTest, Insert_InitializerList) {
  const std::initializer_list<std::pair<const int, int>> list{
      {1, 1}, {2, 4}, {3, 9}};
  HashMap<int, int> hash_map;

  hash_map.Insert(list);
  EXPECT_EQ(hash_map.Size(), 3);
  EXPECT_EQ(hash_map.At(1), *(list.begin() + 0));
  EXPECT_EQ(hash_map.At(2), *(list.begin() + 1));
  EXPECT_EQ(hash_map.At(3), *(list.begin() + 2));

  hash_map.Insert(list);
  EXPECT_EQ(hash_map.Size(), 3);
}

TEST(HashMapTest, InsertOrAssign) {
  HashMap<int, int> hash_map;

  auto result{hash_map.InsertOrAssign(1, 1)};
  EXPECT_EQ(*result.first, (Pair{1, 1}));
  EXPECT_EQ(result.second, true);
  EXPECT_EQ(hash_map.Size(), 1);

  result = hash_map.InsertOrAssign(2, 4);
  EXPECT_EQ(*result.first, (Pair{2, 4}));
  EXPECT_EQ(result.second, true);
  EXPECT_EQ(hash_map.Size(), 2);

  result = hash_map.InsertOrAssign(3, 9);
  EXPECT_EQ(*result.first, (Pair{3, 9}));
  EXPECT_EQ(result.second, true);
  EXPECT_EQ(hash_map.Size(), 3);

  result = hash_map.InsertOrAssign(1, -1);
  EXPECT_EQ(*result.first, (Pair{1, -1}));
  EXPECT_EQ(result.second, false);
  EXPECT_EQ(hash_map.Size(), 3);

  result = hash_map.InsertOrAssign(2, -2);
  EXPECT_EQ(*result.first, (Pair{2, -2}));
  EXPECT_EQ(result.second, false);
  EXPECT_EQ(hash_map.Size(), 3);

  result = hash_map.InsertOrAssign(3, -3);
  EXPECT_EQ(*result.first, (Pair{3, -3}));
  EXPECT_EQ(result.second, false);
  EXPECT_EQ(hash_map.Size(), 3);
}

TEST(HashMapTest, Erase_Element) {
  HashMap<int, int> hash_map{{1, 1}, {2, 4}, {3, 9}};
  auto it{hash_map.end()};
  auto next{hash_map.end()};

  it = hash_map.Find(1);
  next = std::next(it);
  EXPECT_EQ(hash_map.Erase(it), next);
  EXPECT_EQ(hash_map.Size(), 2);

  it = hash_map.Find(2);
  next = std::next(it);
  EXPECT_EQ(hash_map.Erase(it), next);
  EXPECT_EQ(hash_map.Size(), 1);

  it = hash_map.Find(3);
  next = std::next(it);
  EXPECT_EQ(hash_map.Erase(it), next);
  EXPECT_EQ(hash_map.Size(), 0);
}

TEST(HashMapTest, Erase_Range) {
  HashMap<int, int> hash_map{{1, 1}, {2, 4}, {3, 9}};
  auto next{hash_map.end()};

  next = At(hash_map, 2);
  EXPECT_EQ(hash_map.Erase(At(hash_map, 0), At(hash_map, 2)), next);
  EXPECT_EQ(hash_map.Size(), 1);

  next = At(hash_map, 1);
  EXPECT_EQ(hash_map.Erase(At(hash_map, 0), At(hash_map, 1)), next);
  EXPECT_EQ(hash_map.Size(), 0);
}

TEST(HashMapTest, Erase_Key) {
  HashMap<int, int> hash_map{{1, 1}, {2, 4}, {3, 9}};

  EXPECT_EQ(hash_map.Erase(1), 1);
  EXPECT_EQ(hash_map.Size(), 2);

  EXPECT_EQ(hash_map.Erase(2), 1);
  EXPECT_EQ(hash_map.Size(), 1);

  EXPECT_EQ(hash_map.Erase(3), 1);
  EXPECT_EQ(hash_map.Size(), 0);

  EXPECT_EQ(hash_map.Erase(1), 0);
  EXPECT_EQ(hash_map.Size(), 0);

  EXPECT_EQ(hash_map.Erase(2), 0);
  EXPECT_EQ(hash_map.Size(), 0);

  EXPECT_EQ(hash_map.Erase(3), 0);
  EXPECT_EQ(hash_map.Size(), 0);
}

TEST(HashMapTest, Swap) {
  HashMap<int, int> a{{1, 1}, {2, 4}, {3, 9}};
  HashMap<int, int> b{{4, 16}, {5, 25}, {6, 36}};
  const HashMap<int, int> expected_a{b};
  const HashMap<int, int> expected_b{a};

  a.Swap(b);
  EXPECT_EQ(a, expected_a);
  EXPECT_EQ(b, expected_b);
}

// Lookup

TEST(HashMapTest, At) {
  HashMap<int, int> hash_map{{1, 1}, {2, 4}, {3, 9}};
  EXPECT_THROW(hash_map.At(5), std::out_of_range);
  EXPECT_EQ(hash_map.At(1), (Pair{1, 1}));

  hash_map.At(2).second = -2;
  EXPECT_EQ(hash_map.At(2), (Pair{2, -2}));
}

TEST(HashMapTest, At_Const) {
  const HashMap<int, int> hash_map{{1, 1}, {2, 4}, {3, 9}};
  EXPECT_THROW(hash_map.At(5), std::out_of_range);
  EXPECT_EQ(hash_map.At(1), (Pair{1, 1}));
}

TEST(HashMapTest, SubscriptOperator) {
  HashMap<int, int> hash_map{{1, 1}, {2, 4}, {3, 9}};
  EXPECT_EQ(hash_map[1], (Pair{1, 1}));

  hash_map[1].second = -1;
  EXPECT_EQ(hash_map[1], (Pair{1, -1}));

  hash_map[4].second = 16;
  EXPECT_EQ(hash_map[4], (Pair{4, 16}));
  EXPECT_EQ(hash_map.Size(), 4);
}

TEST(HashMapTest, Count) {
  const HashMap<int, int> hash_map{{1, 1}, {2, 4}, {3, 9}};
  EXPECT_EQ(hash_map.Count(1), 1);
  EXPECT_EQ(hash_map.Count(4), 0);
}

TEST(HashMapTest, Find) {
  HashMap<int, int> hash_map{{1, 1}, {2, 4}, {3, 9}};
  EXPECT_EQ(*hash_map.Find(1), (Pair{1, 1}));

  hash_map.Find(1)->second = -1;
  EXPECT_EQ(*hash_map.Find(1), (Pair{1, -1}));

  EXPECT_EQ(hash_map.Find(4), hash_map.end());
}

TEST(HashMapTest, Find_Const) {
  const HashMap<int, int> hash_map{{1, 1}, {2, 4}, {3, 9}};
  EXPECT_EQ(*hash_map.Find(1), (Pair{1, 1}));
  EXPECT_EQ(hash_map.Find(4), hash_map.cend());
}

TEST(HashMapTest, Contains) {
  const HashMap<int, int> hash_map{{1, 1}, {2, 4}, {3, 9}};
  EXPECT_EQ(hash_map.Contains(1), true);
  EXPECT_EQ(hash_map.Contains(4), false);
}

TEST(HashMapTest, EqualRange) {
  HashMap<int, int> hash_map{{1, 1}, {2, 4}, {3, 9}};

  auto result{hash_map.EqualRange(1)};
  EXPECT_EQ(*result.first, (Pair{1, 1}));
  EXPECT_EQ(result.second, std::next(result.first));

  result.first->second = -1;
  EXPECT_EQ(hash_map.At(1), (Pair{1, -1}));

  result = hash_map.EqualRange(4);
  EXPECT_EQ(result.first, hash_map.end());
  EXPECT_EQ(result.second, hash_map.end());
}

TEST(HashMapTest, EqualRange_Const) {
  const HashMap<int, int> hash_map{{1, 1}, {2, 4}, {3, 9}};

  auto result{hash_map.EqualRange(1)};
  EXPECT_EQ(*result.first, (Pair{1, 1}));
  EXPECT_EQ(result.second, std::next(result.first));

  result = hash_map.EqualRange(4);
  EXPECT_EQ(result.first, hash_map.cend());
  EXPECT_EQ(result.second, hash_map.cend());
}

// Bucket interface

TEST(HashMapTest, Begin_Bucket) {
  HashMap<int, int> hash_map{{1, 1}};
  const std::size_t bucket{hash_map.Bucket(1)};

  auto it{hash_map.begin(bucket)};
  EXPECT_EQ(*it, (Pair{1, 1}));
  EXPECT_NE(it, hash_map.end(bucket));

  it->second = 10;
  EXPECT_EQ(hash_map.At(1), (Pair{1, 10}));

  ++it;
  EXPECT_EQ(it, hash_map.end(bucket));
}

TEST(HashMapTest, Begin_Bucket_Const) {
  const HashMap<int, int> hash_map{{1, 1}};
  const std::size_t bucket{hash_map.Bucket(1)};

  auto it{hash_map.begin(bucket)};
  EXPECT_EQ(*it, (Pair{1, 1}));
  EXPECT_NE(it, hash_map.end(bucket));

  ++it;
  EXPECT_EQ(it, hash_map.end(bucket));
}

TEST(HashMapTest, Cbegin_Bucket) {
  const HashMap<int, int> hash_map{{1, 1}};
  const std::size_t bucket{hash_map.Bucket(1)};

  auto it{hash_map.cbegin(bucket)};
  EXPECT_EQ(*it, (Pair{1, 1}));
  EXPECT_NE(it, hash_map.cend(bucket));

  ++it;
  EXPECT_EQ(it, hash_map.cend(bucket));
}

TEST(HashMapTest, End_Bucket) {
  HashMap<int, int> hash_map{{1, 1}};
  const std::size_t bucket{hash_map.Bucket(1)};

  EXPECT_NE(hash_map.end(bucket), hash_map.begin(bucket));
  EXPECT_EQ(hash_map.end(bucket), ++hash_map.begin(bucket));
}

TEST(HashMapTest, End_Bucket_Const) {
  const HashMap<int, int> hash_map{{1, 1}};
  const std::size_t bucket{hash_map.Bucket(1)};

  EXPECT_NE(hash_map.end(bucket), hash_map.begin(bucket));
  EXPECT_EQ(hash_map.end(bucket), ++hash_map.begin(bucket));
}

TEST(HashMapTest, Cend_Bucket) {
  const HashMap<int, int> hash_map{{1, 1}};
  const std::size_t bucket{hash_map.Bucket(1)};

  EXPECT_NE(hash_map.cend(bucket), hash_map.cbegin(bucket));
  EXPECT_EQ(hash_map.cend(bucket), ++hash_map.cbegin(bucket));
}

TEST(HashMapTest, BucketCount) {
  HashMap<int, int> hash_map;
  EXPECT_EQ(hash_map.BucketCount(), 0);

  hash_map.Reserve(5);
  EXPECT_EQ(hash_map.BucketCount(), 5);
}

TEST(HashMapTest, BucketSize) {
  const HashMap<int, int> hash_map{{1, 1}};
  const std::size_t bucket{hash_map.Bucket(1)};
  EXPECT_EQ(hash_map.BucketSize(bucket), 1);
}

TEST(HashMapTest, Bucket) {
  const HashMap<int, int> hash_map{{1, 1}};
  EXPECT_EQ(hash_map.Bucket(1), 0);
}

// Hash policy

TEST(HashMapTest, LoadFactor) {
  HashMap<int, int> hash_map;
  EXPECT_EQ(hash_map.LoadFactor(), 0);

  hash_map.Insert({1, 1});
  EXPECT_EQ(hash_map.LoadFactor(), 1);
}

TEST(HashMapTest, MaxLoadFactor) {
  HashMap<int, int> hash_map;
  EXPECT_EQ(hash_map.MaxLoadFactor(), 1);

  hash_map.MaxLoadFactor(2);
  EXPECT_EQ(hash_map.MaxLoadFactor(), 2);
}

TEST(HashMapTest, MaxLoadFactor_Const) {
  const HashMap<int, int> hash_map;
  EXPECT_EQ(hash_map.MaxLoadFactor(), 1);
}

TEST(HashMapTest, Rehash) {
  HashMap<int, int> hash_map{{1, 1}};
  EXPECT_EQ(hash_map.BucketCount(), 1);

  hash_map.MaxLoadFactor(0.25);
  hash_map.Rehash(0);
  EXPECT_EQ(hash_map.BucketCount(), 4);
}

TEST(HashMapTest, Reserve) {
  HashMap<int, int> hash_map;
  EXPECT_EQ(hash_map.BucketCount(), 0);

  hash_map.Reserve(10);
  EXPECT_EQ(hash_map.BucketCount(), 10);

  hash_map.Reserve(5);
  EXPECT_EQ(hash_map.BucketCount(), 5);
}

// Comparison operators

TEST(HashMapTest, EqualOperator) {
  const HashMap<int, int> a{{1, 1}, {2, 4}, {3, 9}};
  const HashMap<int, int> b{{1, 1}, {2, 4}, {3, 9}};
  EXPECT_EQ(a, b);
}

TEST(HashMapTest, NotEqualOperator) {
  const HashMap<int, int> a{{1, 1}, {2, 4}, {3, 9}};
  const HashMap<int, int> b{{4, 16}, {5, 25}, {6, 36}};
  EXPECT_NE(a, b);
}