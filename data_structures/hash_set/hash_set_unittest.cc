#include "hash_set.h"

#include <gtest/gtest.h>

#include <stdexcept>
#include <utility>

template <class K>
typename HashSet<K>::iterator At(HashSet<K>& hash_set, std::size_t index) {
  auto it{hash_set.begin()};
  std::advance(it, index);
  return it;
}

// Constructors

TEST(HashSetTest, Constructor) {
  const HashSet<int> hash_set;
  EXPECT_EQ(hash_set.Size(), 0);
  EXPECT_EQ(hash_set.BucketCount(), 0);
}

TEST(HashSetTest, CopyConstructor) {
  const HashSet<int> hash_set{1, 2, 3};

  const HashSet<int> copy{hash_set};
  EXPECT_EQ(copy, hash_set);
}

TEST(HashSetTest, MoveConstructor) {
  HashSet<int> hash_set{1, 2, 3};

  const HashSet<int> moved_hash_map{std::move(hash_set)};
  EXPECT_EQ(moved_hash_map.Size(), 3);
  EXPECT_EQ(hash_set.Size(), 0);
}

TEST(HashSetTest, InitializerListConstructor) {
  const HashSet<int> hash_set{1, 2, 3};
  EXPECT_EQ(hash_set.Size(), 3);
  EXPECT_TRUE(hash_set.Contains(1));
  EXPECT_TRUE(hash_set.Contains(2));
  EXPECT_TRUE(hash_set.Contains(3));
}

// Assignments

TEST(HashSetTest, CopyAssignment) {
  const HashSet<int> hash_set{1, 2, 3};
  HashSet<int> copy;

  copy = hash_set;
  EXPECT_EQ(copy, hash_set);
}

TEST(HashSetTest, MoveAssignment) {
  HashSet<int> hash_set{1, 2, 3};
  HashSet<int> moved_hash_map;

  moved_hash_map = std::move(hash_set);
  EXPECT_EQ(moved_hash_map.Size(), 3);
  EXPECT_EQ(hash_set.Size(), 0);
}

TEST(HashSetTest, InitializerListAssigment) {
  HashSet<int> hash_set;

  hash_set = {1, 2, 3};
  EXPECT_EQ(hash_set.Size(), 3);
  EXPECT_TRUE(hash_set.Contains(1));
  EXPECT_TRUE(hash_set.Contains(2));
  EXPECT_TRUE(hash_set.Contains(3));
}

// Iterators

TEST(HashSetTest, Begin) {
  HashSet<int> hash_set;
  EXPECT_EQ(hash_set.begin(), hash_set.end());

  hash_set = {1, 2};
  auto it{hash_set.begin()};
  EXPECT_EQ(*it, 1);
  EXPECT_NE(it, hash_set.end());

  ++it;
  EXPECT_EQ(*it, 2);
  EXPECT_NE(it, hash_set.end());

  ++it;
  EXPECT_EQ(it, hash_set.end());
}

TEST(HashSetTest, Begin_Const) {
  const HashSet<int> hash_set{1, 2};

  auto it{hash_set.begin()};
  EXPECT_EQ(*it, 1);
  EXPECT_NE(it, hash_set.end());

  ++it;
  EXPECT_EQ(*it, 2);
  EXPECT_NE(it, hash_set.end());

  ++it;
  EXPECT_EQ(it, hash_set.end());
}

TEST(HashSetTest, Cbegin) {
  const HashSet<int> hash_set{1, 2};

  auto it{hash_set.cbegin()};
  EXPECT_EQ(*it, 1);
  EXPECT_NE(it, hash_set.cend());

  ++it;
  EXPECT_EQ(*it, 2);
  EXPECT_NE(it, hash_set.cend());

  ++it;
  EXPECT_EQ(it, hash_set.cend());
}

TEST(HashSetTest, End) {
  HashSet<int> hash_set{1};
  EXPECT_NE(hash_set.end(), hash_set.begin());
  EXPECT_EQ(hash_set.end(), ++hash_set.begin());
}

TEST(HashSetTest, End_Const) {
  const HashSet<int> hash_set{1};
  EXPECT_NE(hash_set.end(), hash_set.begin());
  EXPECT_EQ(hash_set.end(), ++hash_set.begin());
}

TEST(HashSetTest, Cend) {
  const HashSet<int> hash_set{1};
  EXPECT_NE(hash_set.cend(), hash_set.cbegin());
  EXPECT_EQ(hash_set.cend(), ++hash_set.cbegin());
}

// Capacity

TEST(HashSetTest, Empty) {
  const HashSet<int> empty_hash_map;
  EXPECT_TRUE(empty_hash_map.Empty());

  const HashSet<int> hash_set{1, 2, 3};
  EXPECT_FALSE(hash_set.Empty());
}

TEST(HashSetTest, Size) {
  HashSet<int> hash_set;
  EXPECT_EQ(hash_set.Size(), 0);

  hash_set.Insert(1);
  EXPECT_EQ(hash_set.Size(), 1);
}

// Modifiers

TEST(HashSetTest, Clear) {
  HashSet<int> hash_set;
  hash_set.Reserve(10);
  hash_set = {1, 2, 3};

  EXPECT_EQ(hash_set.Size(), 3);
  EXPECT_EQ(hash_set.BucketCount(), 10);

  hash_set.Clear();
  EXPECT_EQ(hash_set.Size(), 0);
  EXPECT_EQ(hash_set.BucketCount(), 10);
}

TEST(HashSetTest, Insert_Value) {
  HashSet<int> hash_set;

  auto result{hash_set.Insert(1)};
  EXPECT_EQ(*result.first, 1);
  EXPECT_EQ(result.second, true);
  EXPECT_EQ(hash_set.Size(), 1);

  result = hash_set.Insert(2);
  EXPECT_EQ(*result.first, 2);
  EXPECT_EQ(result.second, true);
  EXPECT_EQ(hash_set.Size(), 2);

  result = hash_set.Insert(3);
  EXPECT_EQ(*result.first, 3);
  EXPECT_EQ(result.second, true);
  EXPECT_EQ(hash_set.Size(), 3);

  result = hash_set.Insert(1);
  EXPECT_EQ(*result.first, 1);
  EXPECT_EQ(result.second, false);
  EXPECT_EQ(hash_set.Size(), 3);

  result = hash_set.Insert(2);
  EXPECT_EQ(*result.first, 2);
  EXPECT_EQ(result.second, false);
  EXPECT_EQ(hash_set.Size(), 3);

  result = hash_set.Insert(3);
  EXPECT_EQ(*result.first, 3);
  EXPECT_EQ(result.second, false);
  EXPECT_EQ(hash_set.Size(), 3);
}

TEST(HashSetTest, Insert_Range) {
  const std::initializer_list<int> source{1, 2, 3, 4, 5, 6};
  HashSet<int> hash_set;

  hash_set.Insert(source.begin() + 0, source.begin() + 2);
  EXPECT_EQ(hash_set.Size(), 2);
  EXPECT_TRUE(hash_set.Contains(*(source.begin() + 0)));
  EXPECT_TRUE(hash_set.Contains(*(source.begin() + 1)));

  hash_set.Insert(source.begin() + 2, source.begin() + 4);
  EXPECT_EQ(hash_set.Size(), 4);
  EXPECT_TRUE(hash_set.Contains(*(source.begin() + 2)));
  EXPECT_TRUE(hash_set.Contains(*(source.begin() + 3)));

  hash_set.Insert(source.begin() + 4, source.begin() + 6);
  EXPECT_EQ(hash_set.Size(), 6);
  EXPECT_TRUE(hash_set.Contains(*(source.begin() + 4)));
  EXPECT_TRUE(hash_set.Contains(*(source.begin() + 5)));

  hash_set.Insert(source.begin(), source.begin() + 6);
  EXPECT_EQ(hash_set.Size(), 6);
}

TEST(HashSetTest, Insert_InitializerList) {
  const std::initializer_list<int> list{1, 2, 3};
  HashSet<int> hash_set;

  hash_set.Insert(list);
  EXPECT_EQ(hash_set.Size(), 3);
  EXPECT_TRUE(hash_set.Contains(*(list.begin() + 0)));
  EXPECT_TRUE(hash_set.Contains(*(list.begin() + 1)));
  EXPECT_TRUE(hash_set.Contains(*(list.begin() + 2)));

  hash_set.Insert(list);
  EXPECT_EQ(hash_set.Size(), 3);
}

TEST(HashSetTest, Erase_Element) {
  HashSet<int> hash_set{1, 2, 3};
  auto it{hash_set.end()};
  auto next{hash_set.end()};

  it = hash_set.Find(1);
  next = std::next(it);
  EXPECT_EQ(hash_set.Erase(it), next);
  EXPECT_EQ(hash_set.Size(), 2);

  it = hash_set.Find(2);
  next = std::next(it);
  EXPECT_EQ(hash_set.Erase(it), next);
  EXPECT_EQ(hash_set.Size(), 1);

  it = hash_set.Find(3);
  next = std::next(it);
  EXPECT_EQ(hash_set.Erase(it), next);
  EXPECT_EQ(hash_set.Size(), 0);
}

TEST(HashSetTest, Erase_Range) {
  HashSet<int> hash_set{1, 2, 3};
  auto next{hash_set.end()};

  next = At(hash_set, 2);
  EXPECT_EQ(hash_set.Erase(At(hash_set, 0), At(hash_set, 2)), next);
  EXPECT_EQ(hash_set.Size(), 1);

  next = At(hash_set, 1);
  EXPECT_EQ(hash_set.Erase(At(hash_set, 0), At(hash_set, 1)), next);
  EXPECT_EQ(hash_set.Size(), 0);
}

TEST(HashSetTest, Erase_Key) {
  HashSet<int> hash_set{1, 2, 3};

  EXPECT_EQ(hash_set.Erase(1), 1);
  EXPECT_EQ(hash_set.Size(), 2);

  EXPECT_EQ(hash_set.Erase(2), 1);
  EXPECT_EQ(hash_set.Size(), 1);

  EXPECT_EQ(hash_set.Erase(3), 1);
  EXPECT_EQ(hash_set.Size(), 0);

  EXPECT_EQ(hash_set.Erase(1), 0);
  EXPECT_EQ(hash_set.Size(), 0);

  EXPECT_EQ(hash_set.Erase(2), 0);
  EXPECT_EQ(hash_set.Size(), 0);

  EXPECT_EQ(hash_set.Erase(3), 0);
  EXPECT_EQ(hash_set.Size(), 0);
}

TEST(HashSetTest, Swap) {
  HashSet<int> a{1, 2, 3};
  HashSet<int> b{4, 5, 6};
  const HashSet<int> expected_a{b};
  const HashSet<int> expected_b{a};

  a.Swap(b);
  EXPECT_EQ(a, expected_a);
  EXPECT_EQ(b, expected_b);
}

// Lookup

TEST(HashSetTest, Count) {
  const HashSet<int> hash_set{1, 2, 3};
  EXPECT_EQ(hash_set.Count(1), 1);
  EXPECT_EQ(hash_set.Count(4), 0);
}

TEST(HashSetTest, Find) {
  HashSet<int> hash_set{1, 2, 3};
  EXPECT_EQ(*hash_set.Find(1), 1);
  EXPECT_EQ(hash_set.Find(4), hash_set.end());
}

TEST(HashSetTest, Find_Const) {
  const HashSet<int> hash_set{1, 2, 3};
  EXPECT_EQ(*hash_set.Find(1), 1);
  EXPECT_EQ(hash_set.Find(4), hash_set.cend());
}

TEST(HashSetTest, Contains) {
  const HashSet<int> hash_set{1, 2, 3};
  EXPECT_TRUE(hash_set.Contains(1));
  EXPECT_FALSE(hash_set.Contains(4));
}

TEST(HashSetTest, EqualRange) {
  HashSet<int> hash_set{1, 2, 3};

  auto result{hash_set.EqualRange(1)};
  EXPECT_EQ(*result.first, 1);
  EXPECT_EQ(result.second, std::next(result.first));

  result = hash_set.EqualRange(4);
  EXPECT_EQ(result.first, hash_set.end());
  EXPECT_EQ(result.second, hash_set.end());
}

TEST(HashSetTest, EqualRange_Const) {
  const HashSet<int> hash_set{1, 2, 3};

  auto result{hash_set.EqualRange(1)};
  EXPECT_EQ(*result.first, 1);
  EXPECT_EQ(result.second, std::next(result.first));

  result = hash_set.EqualRange(4);
  EXPECT_EQ(result.first, hash_set.cend());
  EXPECT_EQ(result.second, hash_set.cend());
}

// Bucket interface

TEST(HashSetTest, Begin_Bucket) {
  HashSet<int> hash_set{1};
  const std::size_t bucket{hash_set.Bucket(1)};

  auto it{hash_set.begin(bucket)};
  EXPECT_EQ(*it, 1);
  EXPECT_NE(it, hash_set.end(bucket));

  ++it;
  EXPECT_EQ(it, hash_set.end(bucket));
}

TEST(HashSetTest, Begin_Bucket_Const) {
  const HashSet<int> hash_set{1};
  const std::size_t bucket{hash_set.Bucket(1)};

  auto it{hash_set.begin(bucket)};
  EXPECT_EQ(*it, 1);
  EXPECT_NE(it, hash_set.end(bucket));

  ++it;
  EXPECT_EQ(it, hash_set.end(bucket));
}

TEST(HashSetTest, Cbegin_Bucket) {
  const HashSet<int> hash_set{1};
  const std::size_t bucket{hash_set.Bucket(1)};

  auto it{hash_set.cbegin(bucket)};
  EXPECT_EQ(*it, 1);
  EXPECT_NE(it, hash_set.cend(bucket));

  ++it;
  EXPECT_EQ(it, hash_set.cend(bucket));
}

TEST(HashSetTest, End_Bucket) {
  HashSet<int> hash_set{1};
  const std::size_t bucket{hash_set.Bucket(1)};

  EXPECT_NE(hash_set.end(bucket), hash_set.begin(bucket));
  EXPECT_EQ(hash_set.end(bucket), ++hash_set.begin(bucket));
}

TEST(HashSetTest, End_Bucket_Const) {
  const HashSet<int> hash_set{1};
  const std::size_t bucket{hash_set.Bucket(1)};

  EXPECT_NE(hash_set.end(bucket), hash_set.begin(bucket));
  EXPECT_EQ(hash_set.end(bucket), ++hash_set.begin(bucket));
}

TEST(HashSetTest, Cend_Bucket) {
  const HashSet<int> hash_set{1};
  const std::size_t bucket{hash_set.Bucket(1)};

  EXPECT_NE(hash_set.cend(bucket), hash_set.cbegin(bucket));
  EXPECT_EQ(hash_set.cend(bucket), ++hash_set.cbegin(bucket));
}

TEST(HashSetTest, BucketCount) {
  HashSet<int> hash_set;
  EXPECT_EQ(hash_set.BucketCount(), 0);

  hash_set.Reserve(5);
  EXPECT_EQ(hash_set.BucketCount(), 5);
}

TEST(HashSetTest, BucketSize) {
  const HashSet<int> hash_set{1};
  const std::size_t bucket{hash_set.Bucket(1)};
  EXPECT_EQ(hash_set.BucketSize(bucket), 1);
}

TEST(HashSetTest, Bucket) {
  const HashSet<int> hash_set{1};
  EXPECT_EQ(hash_set.Bucket(1), 0);
}

// Hash policy

TEST(HashSetTest, LoadFactor) {
  HashSet<int> hash_set;
  EXPECT_EQ(hash_set.LoadFactor(), 0);

  hash_set.Insert(1);
  EXPECT_EQ(hash_set.LoadFactor(), 1);
}

TEST(HashSetTest, MaxLoadFactor) {
  HashSet<int> hash_set;
  EXPECT_EQ(hash_set.MaxLoadFactor(), 1);

  hash_set.MaxLoadFactor(2);
  EXPECT_EQ(hash_set.MaxLoadFactor(), 2);
}

TEST(HashSetTest, MaxLoadFactor_Const) {
  const HashSet<int> hash_set;
  EXPECT_EQ(hash_set.MaxLoadFactor(), 1);
}

TEST(HashSetTest, Rehash) {
  HashSet<int> hash_set{1};
  EXPECT_EQ(hash_set.BucketCount(), 1);

  hash_set.MaxLoadFactor(0.25);
  hash_set.Rehash(0);
  EXPECT_EQ(hash_set.BucketCount(), 4);
}

TEST(HashSetTest, Reserve) {
  HashSet<int> hash_set;
  EXPECT_EQ(hash_set.BucketCount(), 0);

  hash_set.Reserve(10);
  EXPECT_EQ(hash_set.BucketCount(), 10);

  hash_set.Reserve(5);
  EXPECT_EQ(hash_set.BucketCount(), 5);
}

// Comparison operators

TEST(HashSetTest, EqualOperator) {
  const HashSet<int> a{1, 2, 3};
  const HashSet<int> b{1, 2, 3};
  EXPECT_EQ(a, b);
}

TEST(HashSetTest, NotEqualOperator) {
  const HashSet<int> a{1, 2, 3};
  const HashSet<int> b{4, 5, 6};
  EXPECT_NE(a, b);
}