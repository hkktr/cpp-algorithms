#include "array.h"

#include <gtest/gtest.h>

#include <stdexcept>

// Constructors

TEST(ArrayTest, Constructor) {
  constexpr Array<int, 3> array;
  EXPECT_EQ(array[0], int{});
  EXPECT_EQ(array[1], int{});
  EXPECT_EQ(array[2], int{});
}

TEST(ArrayTest, CopyConstructor) {
  constexpr Array<int, 3> array{1, 2, 3};
  constexpr Array<int, 3> copy{array};

  EXPECT_EQ(array, copy);
  EXPECT_NE(array.Data(), copy.Data());
}

TEST(ArrayTest, InitializerListConstructor) {
  constexpr Array<int, 3> array{1, 2, 3};
  EXPECT_EQ(array[0], 1);
  EXPECT_EQ(array[1], 2);
  EXPECT_EQ(array[2], 3);
}

// Assignments

TEST(ArrayTest, CopyAssignment) {
  constexpr Array<int, 3> array{1, 2, 3};
  Array<int, 3> copy;

  copy = array;
  EXPECT_EQ(array, copy);
  EXPECT_NE(array.Data(), copy.Data());
}

TEST(ArrayTest, InitializerListAssignment) {
  Array<int, 3> array;
  array = {1, 2, 3};

  EXPECT_EQ(array[0], 1);
  EXPECT_EQ(array[1], 2);
  EXPECT_EQ(array[2], 3);
}

// Element access

TEST(ArrayTest, At) {
  Array<int, 3> array{1, 2, 3};
  EXPECT_THROW(array.At(5), std::out_of_range);
  EXPECT_EQ(array.At(1), 2);

  array.At(2) = 5;
  EXPECT_EQ(array.At(2), 5);
}

TEST(ArrayTest, At_Const) {
  constexpr Array<int, 3> array{1, 2, 3};
  EXPECT_THROW(array.At(5), std::out_of_range);
  EXPECT_EQ(array.At(1), 2);
}

TEST(ArrayTest, SubscriptOperator) {
  Array<int, 3> array{1, 2, 3};
  EXPECT_EQ(array[1], 2);

  array[1] = 5;
  EXPECT_EQ(array[1], 5);
}

TEST(ArrayTest, SubscriptOperator_Const) {
  constexpr Array<int, 3> array{1, 2, 3};
  EXPECT_EQ(array[1], 2);
}

TEST(ArrayTest, Front) {
  Array<int, 3> array{1, 2, 3};
  EXPECT_EQ(array.Front(), 1);

  array.Front() = 5;
  EXPECT_EQ(array.Front(), 5);
}

TEST(ArrayTest, Front_Const) {
  constexpr Array<int, 3> array{1, 2, 3};
  EXPECT_EQ(array.Front(), 1);
}

TEST(ArrayTest, Back) {
  Array<int, 3> array{1, 2, 3};
  EXPECT_EQ(array.Back(), 3);

  array.Back() = 5;
  EXPECT_EQ(array.Back(), 5);
}

TEST(ArrayTest, Back_Const) {
  constexpr Array<int, 3> array{1, 2, 3};
  EXPECT_EQ(array.Back(), 3);
}

TEST(ArrayTest, Data) {
  Array<int, 3> array{1, 2, 3};

  int* const data{array.Data()};
  EXPECT_EQ(*data, 1);

  array.Front() = 5;
  EXPECT_EQ(*data, 5);
}

TEST(ArrayTest, Data_Const) {
  constexpr Array<int, 3> array{1, 2, 3};

  const int* const data{array.Data()};
  EXPECT_EQ(*data, 1);
}

// Iterators

TEST(ArrayTest, Begin) {
  Array<int, 2> array{1, 2};

  auto it{array.begin()};
  EXPECT_EQ(*it, 1);
  EXPECT_NE(it, array.end());

  *it = 10;
  EXPECT_EQ(array.Front(), 10);

  ++it;
  EXPECT_EQ(*it, 2);
  EXPECT_NE(it, array.end());

  ++it;
  EXPECT_EQ(it, array.end());
}

TEST(ArrayTest, Begin_Const) {
  const Array<int, 2> array{1, 2};

  auto it{array.begin()};
  EXPECT_EQ(*it, 1);
  EXPECT_NE(it, array.end());

  ++it;
  EXPECT_EQ(*it, 2);
  EXPECT_NE(it, array.end());

  ++it;
  EXPECT_EQ(it, array.end());
}

TEST(ArrayTest, Cbegin) {
  const Array<int, 2> array{1, 2};

  auto it{array.cbegin()};
  EXPECT_EQ(*it, 1);
  EXPECT_NE(it, array.cend());

  ++it;
  EXPECT_EQ(*it, 2);
  EXPECT_NE(it, array.cend());

  ++it;
  EXPECT_EQ(it, array.cend());
}

TEST(ArrayTest, End) {
  Array<int, 1> array{1};
  EXPECT_NE(array.end(), array.begin());
  EXPECT_EQ(array.end(), ++array.begin());
}

TEST(ArrayTest, End_Const) {
  const Array<int, 1> array{1};
  EXPECT_NE(array.end(), array.begin());
  EXPECT_EQ(array.end(), ++array.begin());
}

TEST(ArrayTest, Cend) {
  const Array<int, 1> array{1};
  EXPECT_NE(array.cend(), array.cbegin());
  EXPECT_EQ(array.cend(), ++array.cbegin());
}

TEST(ArrayTest, Rbegin) {
  Array<int, 2> array{1, 2};

  auto it{array.rbegin()};
  EXPECT_EQ(*it, 2);
  EXPECT_NE(it, array.rend());

  *it = 10;
  EXPECT_EQ(array.Back(), 10);

  ++it;
  EXPECT_EQ(*it, 1);
  EXPECT_NE(it, array.rend());

  ++it;
  EXPECT_EQ(it, array.rend());
}

TEST(ArrayTest, Rbegin_Const) {
  const Array<int, 2> array{1, 2};

  auto it{array.rbegin()};
  EXPECT_EQ(*it, 2);
  EXPECT_NE(it, array.rend());

  ++it;
  EXPECT_EQ(*it, 1);
  EXPECT_NE(it, array.rend());

  ++it;
  EXPECT_EQ(it, array.rend());
}

TEST(ArrayTest, Crbegin) {
  const Array<int, 2> array{1, 2};

  auto it{array.crbegin()};
  EXPECT_EQ(*it, 2);
  EXPECT_NE(it, array.crend());

  ++it;
  EXPECT_EQ(*it, 1);
  EXPECT_NE(it, array.crend());

  ++it;
  EXPECT_EQ(it, array.crend());
}

TEST(ArrayTest, Rend) {
  Array<int, 1> array{1};
  EXPECT_NE(array.rend(), array.rbegin());
  EXPECT_EQ(array.rend(), ++array.rbegin());
}

TEST(ArrayTest, Rend_Const) {
  const Array<int, 1> array{1};
  EXPECT_NE(array.rend(), array.rbegin());
  EXPECT_EQ(array.rend(), ++array.rbegin());
}

TEST(ArrayTest, Crend) {
  const Array<int, 1> array{1};
  EXPECT_NE(array.crend(), array.crbegin());
  EXPECT_EQ(array.crend(), ++array.crbegin());
}

// Capacity

TEST(ArrayTest, Empty) {
  const Array<int, 0> empty_array;
  EXPECT_TRUE(empty_array.Empty());

  constexpr Array<int, 3> array;
  EXPECT_FALSE(array.Empty());
}

TEST(ArrayTest, Size) {
  constexpr Array<int, 3> array;
  EXPECT_EQ(array.Size(), 3);
}

// Operations

TEST(ArrayTest, Fill) {
  Array<int, 3> array;

  array.Fill(5);
  EXPECT_EQ(array, (Array<int, 3>{5, 5, 5}));
}

TEST(ArrayTest, Swap) {
  Array<int, 3> a{1, 2, 3};
  Array<int, 3> b{4, 5, 6};
  const Array<int, 3> expected_a{b};
  const Array<int, 3> expected_b{a};

  a.Swap(b);
  EXPECT_EQ(a, expected_a);
  EXPECT_EQ(b, expected_b);
}

// Comparison operators

TEST(ArrayTest, EqualOperator) {
  constexpr Array<int, 3> a{1, 2, 3};
  constexpr Array<int, 3> b{1, 2, 3};
  EXPECT_EQ(a, b);
}

TEST(ArrayTest, NotEqualOperator) {
  constexpr Array<int, 3> a{1, 2, 3};
  constexpr Array<int, 3> b{4, 5, 6};
  EXPECT_NE(a, b);
}

TEST(ArrayTest, LessOperator) {
  constexpr Array<int, 3> a{1, 2, 3};
  constexpr Array<int, 3> b{4, 5, 6};
  EXPECT_LT(a, b);
}

TEST(ArrayTest, LessEqualOperator) {
  Array<int, 3> a{1, 2, 3};
  Array<int, 3> b{4, 5, 6};
  EXPECT_LE(a, b);

  a = b = {1, 2, 3};
  EXPECT_LE(a, b);
}

TEST(ArrayTest, GreaterOperator) {
  constexpr Array<int, 3> a{4, 5, 6};
  constexpr Array<int, 3> b{1, 2, 3};
  EXPECT_GT(a, b);
}

TEST(ArrayTest, GreaterEqualOperator) {
  Array<int, 3> a{4, 5, 6};
  Array<int, 3> b{1, 2, 3};
  EXPECT_GE(a, b);

  a = b = {1, 2, 3};
  EXPECT_GE(a, b);
}
