#include "binary_heap.h"

#include <gtest/gtest.h>

#include <functional>
#include <initializer_list>
#include <utility>

template <class T>
using MaxBinaryHeap = BinaryHeap<T>;

template <class T>
using MinBinaryHeap = BinaryHeap<T, std::greater<T>>;

// Constructors

TEST(BinaryHeapTest, Constructor) {
  const MaxBinaryHeap<int> binary_heap;
  EXPECT_EQ(binary_heap.Capacity(), 0);
  EXPECT_EQ(binary_heap.Size(), 0);
}

TEST(BinaryHeapTest, CopyConstructor) {
  const MaxBinaryHeap<int> binary_heap{1, 2, 3};

  const MaxBinaryHeap<int> copy{binary_heap};
  EXPECT_EQ(copy, binary_heap);
  EXPECT_NE(copy.Data(), binary_heap.Data());
}

TEST(BinaryHeapTest, MoveConstructor) {
  MaxBinaryHeap<int> binary_heap{1, 2, 3};

  const MaxBinaryHeap<int> moved_binary_heap{std::move(binary_heap)};
  EXPECT_EQ(moved_binary_heap.Capacity(), 3);
  EXPECT_EQ(moved_binary_heap.Size(), 3);
  EXPECT_EQ(binary_heap.Capacity(), 0);
  EXPECT_EQ(binary_heap.Size(), 0);
  EXPECT_NE(moved_binary_heap.Data(), binary_heap.Data());
}

TEST(BinaryHeapTest, InitializerListConstructor) {
  const MaxBinaryHeap<int> max_binary_heap{1, 2, 3};
  EXPECT_EQ(max_binary_heap.Capacity(), 3);
  EXPECT_EQ(max_binary_heap.Size(), 3);
  EXPECT_EQ(max_binary_heap.Top(), 3);

  const MinBinaryHeap<int> min_binary_heap{1, 2, 3};
  EXPECT_EQ(min_binary_heap.Capacity(), 3);
  EXPECT_EQ(min_binary_heap.Size(), 3);
  EXPECT_EQ(min_binary_heap.Top(), 1);
}

// Assignments

TEST(BinaryHeapTest, CopyAssignment) {
  const MaxBinaryHeap<int> binary_heap{1, 2, 3};
  MaxBinaryHeap<int> copy;

  copy = binary_heap;
  EXPECT_EQ(copy, binary_heap);
  EXPECT_NE(copy.Data(), binary_heap.Data());
}

TEST(BinaryHeapTest, MoveAssignment) {
  MaxBinaryHeap<int> binary_heap{1, 2, 3};
  MaxBinaryHeap<int> moved_binary_heap;

  moved_binary_heap = std::move(binary_heap);
  EXPECT_EQ(moved_binary_heap.Capacity(), 3);
  EXPECT_EQ(moved_binary_heap.Size(), 3);
  EXPECT_EQ(binary_heap.Capacity(), 0);
  EXPECT_EQ(binary_heap.Size(), 0);
  EXPECT_NE(moved_binary_heap.Data(), binary_heap.Data());
}

TEST(BinaryHeapTest, InitializerListAssigment) {
  MaxBinaryHeap<int> max_binary_heap;
  max_binary_heap = {1, 2, 3};

  EXPECT_EQ(max_binary_heap.Capacity(), 3);
  EXPECT_EQ(max_binary_heap.Size(), 3);
  EXPECT_EQ(max_binary_heap.Top(), 3);

  MinBinaryHeap<int> min_binary_heap;
  min_binary_heap = {1, 2, 3};

  EXPECT_EQ(min_binary_heap.Capacity(), 3);
  EXPECT_EQ(min_binary_heap.Size(), 3);
  EXPECT_EQ(min_binary_heap.Top(), 1);
}

// Element access

TEST(BinaryHeapTest, Top) {
  MaxBinaryHeap<int> max_binary_heap{1, 2, 3};
  EXPECT_EQ(max_binary_heap.Top(), 3);

  max_binary_heap.Insert(5);
  EXPECT_EQ(max_binary_heap.Top(), 5);

  MinBinaryHeap<int> min_binary_heap{4, 5, 6};
  EXPECT_EQ(min_binary_heap.Top(), 4);

  min_binary_heap.Insert(1);
  EXPECT_EQ(min_binary_heap.Top(), 1);
}

TEST(BinaryHeapTest, Data) {
  MaxBinaryHeap<int> binary_heap{1, 2, 3};

  int* const data{binary_heap.Data()};
  EXPECT_EQ(*data, 3);
}

TEST(BinaryHeapTest, Data_Const) {
  const MaxBinaryHeap<int> binary_heap{1, 2, 3};

  const int* const data{binary_heap.Data()};
  EXPECT_EQ(*data, 3);
}

// Iterators

TEST(BinaryHeapTest, Cbegin) {
  const MaxBinaryHeap<int> binary_heap{1, 2};

  auto it{binary_heap.cbegin()};
  EXPECT_EQ(*it, 2);
  EXPECT_NE(it, binary_heap.cend());

  ++it;
  EXPECT_EQ(*it, 1);
  EXPECT_NE(it, binary_heap.cend());

  ++it;
  EXPECT_EQ(it, binary_heap.cend());
}

TEST(BinaryHeapTest, Cend) {
  const MaxBinaryHeap<int> binary_heap{1};
  EXPECT_NE(binary_heap.cend(), binary_heap.cbegin());
  EXPECT_EQ(binary_heap.cend(), ++binary_heap.cbegin());
}

// Capacity

TEST(BinaryHeapTest, Empty) {
  const MaxBinaryHeap<int> empty_binary_heap;
  EXPECT_TRUE(empty_binary_heap.Empty());

  const MaxBinaryHeap<int> binary_heap{1, 2, 3};
  EXPECT_FALSE(binary_heap.Empty());
}

TEST(BinaryHeapTest, Size) {
  MaxBinaryHeap<int> binary_heap;
  EXPECT_EQ(binary_heap.Size(), 0);

  binary_heap.Insert(1);
  EXPECT_EQ(binary_heap.Size(), 1);
}

TEST(BinaryHeapTest, Reserve) {
  MaxBinaryHeap<int> binary_heap;

  binary_heap.Reserve(10);
  EXPECT_EQ(binary_heap.Capacity(), 10);

  binary_heap.Reserve(5);
  EXPECT_EQ(binary_heap.Capacity(), 10);
}

TEST(BinaryHeapTest, Capacity) {
  MaxBinaryHeap<int> binary_heap;
  EXPECT_EQ(binary_heap.Capacity(), 0);

  binary_heap.Insert(1);
  EXPECT_EQ(binary_heap.Capacity(), 1);

  binary_heap.Insert(2);
  EXPECT_EQ(binary_heap.Capacity(), 2);

  binary_heap.Insert(3);
  EXPECT_EQ(binary_heap.Capacity(), 4);
}

TEST(BinaryHeapTest, ShrinkToFit) {
  MaxBinaryHeap<int> binary_heap;
  binary_heap.Reserve(10);
  binary_heap = {1, 2, 3};

  EXPECT_EQ(binary_heap.Size(), 3);
  EXPECT_EQ(binary_heap.Capacity(), 10);

  binary_heap.ShrinkToFit();
  EXPECT_EQ(binary_heap.Capacity(), binary_heap.Size());
}

// Modifiers

TEST(BinaryHeapTest, Clear) {
  MaxBinaryHeap<int> binary_heap;
  binary_heap.Reserve(10);
  binary_heap = {1, 2, 3};

  EXPECT_EQ(binary_heap.Size(), 3);
  EXPECT_EQ(binary_heap.Capacity(), 10);

  binary_heap.Clear();
  EXPECT_EQ(binary_heap.Size(), 0);
  EXPECT_EQ(binary_heap.Capacity(), 10);
}

TEST(BinaryHeapTest, Insert_Value) {
  MaxBinaryHeap<int> binary_heap;

  binary_heap.Insert(1);
  EXPECT_EQ(binary_heap.Size(), 1);
  EXPECT_EQ(binary_heap.Top(), 1);

  binary_heap.Insert(3);
  EXPECT_EQ(binary_heap.Size(), 2);
  EXPECT_EQ(binary_heap.Top(), 3);

  binary_heap.Insert(2);
  EXPECT_EQ(binary_heap.Size(), 3);
  EXPECT_EQ(binary_heap.Top(), 3);
}

TEST(BinaryHeapTest, Insert_Range) {
  const std::initializer_list<int> source{3, 2, 1, 7, 9, 8, 4, 5, 6};
  MaxBinaryHeap<int> binary_heap;

  binary_heap.Insert(source.begin() + 0, source.begin() + 3);
  EXPECT_EQ(binary_heap.Size(), 3);
  EXPECT_EQ(binary_heap.Top(), 3);

  binary_heap.Insert(source.begin() + 3, source.begin() + 6);
  EXPECT_EQ(binary_heap.Size(), 6);
  EXPECT_EQ(binary_heap.Top(), 9);

  binary_heap.Insert(source.begin() + 6, source.begin() + 9);
  EXPECT_EQ(binary_heap.Size(), 9);
  EXPECT_EQ(binary_heap.Top(), 9);
}

TEST(BinaryHeapTest, Insert_InitializerList) {
  MaxBinaryHeap<int> binary_heap;

  binary_heap.Insert({3, 2, 1});
  EXPECT_EQ(binary_heap.Size(), 3);
  EXPECT_EQ(binary_heap.Top(), 3);

  binary_heap.Insert({7, 9, 8});
  EXPECT_EQ(binary_heap.Size(), 6);
  EXPECT_EQ(binary_heap.Top(), 9);

  binary_heap.Insert({4, 5, 6});
  EXPECT_EQ(binary_heap.Size(), 9);
  EXPECT_EQ(binary_heap.Top(), 9);
}

TEST(BinaryHeapTest, Erase_Element) {
  MaxBinaryHeap<int> binary_heap{1, 2, 3};

  binary_heap.Erase(3);
  EXPECT_EQ(binary_heap.Size(), 2);
  EXPECT_EQ(binary_heap.Top(), 2);

  binary_heap.Erase(1);
  EXPECT_EQ(binary_heap.Size(), 1);
  EXPECT_EQ(binary_heap.Top(), 2);

  binary_heap.Erase(2);
  EXPECT_TRUE(binary_heap.Empty());
}

TEST(BinaryHeapTest, Erase_Range) {
  const std::initializer_list<int> source{7, 9, 8, 3, 2, 1, 4, 5, 6};
  MaxBinaryHeap<int> binary_heap{1, 2, 3, 4, 5, 6, 7, 8, 9};

  binary_heap.Erase(source.begin() + 0, source.begin() + 3);
  EXPECT_EQ(binary_heap.Size(), 6);
  EXPECT_EQ(binary_heap.Top(), 6);

  binary_heap.Erase(source.begin() + 3, source.begin() + 6);
  EXPECT_EQ(binary_heap.Size(), 3);
  EXPECT_EQ(binary_heap.Top(), 6);

  binary_heap.Erase(source.begin() + 6, source.begin() + 9);
  EXPECT_TRUE(binary_heap.Empty());
}

TEST(BinaryHeapTest, Erase_InitializerList) {
  MaxBinaryHeap<int> binary_heap{1, 2, 3, 4, 5, 6, 7, 8, 9};

  binary_heap.Erase({7, 9, 8});
  EXPECT_EQ(binary_heap.Size(), 6);
  EXPECT_EQ(binary_heap.Top(), 6);

  binary_heap.Erase({3, 2, 1});
  EXPECT_EQ(binary_heap.Size(), 3);
  EXPECT_EQ(binary_heap.Top(), 6);

  binary_heap.Erase({4, 5, 6});
  EXPECT_TRUE(binary_heap.Empty());
}

TEST(BinaryHeapTest, Pop) {
  MaxBinaryHeap<int> max_binary_heap{1, 2, 3};

  max_binary_heap.Pop();
  EXPECT_EQ(max_binary_heap.Top(), 2);

  max_binary_heap.Pop();
  EXPECT_EQ(max_binary_heap.Top(), 1);

  max_binary_heap.Pop();
  EXPECT_TRUE(max_binary_heap.Empty());

  MinBinaryHeap<int> min_binary_heap{1, 2, 3};

  min_binary_heap.Pop();
  EXPECT_EQ(min_binary_heap.Top(), 2);

  min_binary_heap.Pop();
  EXPECT_EQ(min_binary_heap.Top(), 3);

  min_binary_heap.Pop();
  EXPECT_TRUE(min_binary_heap.Empty());
}

TEST(BinaryHeapTest, Swap) {
  MaxBinaryHeap<int> a{1, 2, 3};
  MaxBinaryHeap<int> b{4, 5, 6};
  const MaxBinaryHeap<int> expected_a{b};
  const MaxBinaryHeap<int> expected_b{a};

  a.Swap(b);
  EXPECT_EQ(a, expected_a);
  EXPECT_EQ(b, expected_b);
}

// Comparison operators

TEST(BinaryHeapTest, EqualOperator) {
  const MaxBinaryHeap<int> a{1, 2, 3};
  const MaxBinaryHeap<int> b{1, 2, 3};
  EXPECT_EQ(a, b);
}

TEST(BinaryHeapTest, NotEqualOperator) {
  const MaxBinaryHeap<int> a{1, 2, 3};
  const MaxBinaryHeap<int> b{4, 5, 6};
  EXPECT_NE(a, b);
}