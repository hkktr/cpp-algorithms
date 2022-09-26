#include "dynamic_array.h"

#include <gtest/gtest.h>

#include <stdexcept>
#include <utility>

// Constructors

TEST(DynamicArrayTest, Constructor) {
  const DynamicArray<int> dynamic_array;
  EXPECT_EQ(dynamic_array.Capacity(), 0);
  EXPECT_EQ(dynamic_array.Size(), 0);
}

TEST(DynamicArrayTest, CopyConstructor) {
  const DynamicArray<int> dynamic_array{1, 2, 3};

  const DynamicArray<int> copy{dynamic_array};
  EXPECT_EQ(copy, dynamic_array);
  EXPECT_NE(copy.Data(), dynamic_array.Data());
}

TEST(DynamicArrayTest, MoveConstructor) {
  DynamicArray<int> dynamic_array{1, 2, 3};

  const DynamicArray<int> moved_array{std::move(dynamic_array)};
  EXPECT_EQ(moved_array.Capacity(), 3);
  EXPECT_EQ(moved_array.Size(), 3);
  EXPECT_EQ(dynamic_array.Capacity(), 0);
  EXPECT_EQ(dynamic_array.Size(), 0);
  EXPECT_NE(moved_array.Data(), dynamic_array.Data());
}

TEST(DynamicArrayTest, InitializerListConstructor) {
  const DynamicArray<int> dynamic_array{1, 2, 3};
  EXPECT_EQ(dynamic_array.Capacity(), 3);
  EXPECT_EQ(dynamic_array.Size(), 3);
  EXPECT_EQ(dynamic_array[0], 1);
  EXPECT_EQ(dynamic_array[1], 2);
  EXPECT_EQ(dynamic_array[2], 3);
}

// Assignments

TEST(DynamicArrayTest, CopyAssignment) {
  const DynamicArray<int> dynamic_array{1, 2, 3};
  DynamicArray<int> copy;

  copy = dynamic_array;
  EXPECT_EQ(copy, dynamic_array);
  EXPECT_NE(copy.Data(), dynamic_array.Data());
}

TEST(DynamicArrayTest, MoveAssignment) {
  DynamicArray<int> dynamic_array{1, 2, 3};
  DynamicArray<int> moved_array;

  moved_array = std::move(dynamic_array);
  EXPECT_EQ(moved_array.Capacity(), 3);
  EXPECT_EQ(moved_array.Size(), 3);
  EXPECT_EQ(dynamic_array.Capacity(), 0);
  EXPECT_EQ(dynamic_array.Size(), 0);
  EXPECT_NE(moved_array.Data(), dynamic_array.Data());
}

TEST(DynamicArrayTest, InitializerListAssigment) {
  DynamicArray<int> dynamic_array;

  dynamic_array = {1, 2, 3};
  EXPECT_EQ(dynamic_array.Capacity(), 3);
  EXPECT_EQ(dynamic_array.Size(), 3);
  EXPECT_EQ(dynamic_array[0], 1);
  EXPECT_EQ(dynamic_array[1], 2);
  EXPECT_EQ(dynamic_array[2], 3);
}

// Element access

TEST(DynamicArrayTest, At) {
  DynamicArray<int> dynamic_array{1, 2, 3};
  EXPECT_THROW(dynamic_array.At(5), std::out_of_range);
  EXPECT_EQ(dynamic_array.At(1), 2);

  dynamic_array.At(2) = 5;
  EXPECT_EQ(dynamic_array.At(2), 5);
}

TEST(DynamicArrayTest, At_Const) {
  const DynamicArray<int> dynamic_array{1, 2, 3};
  EXPECT_THROW(dynamic_array.At(5), std::out_of_range);
  EXPECT_EQ(dynamic_array.At(1), 2);
}

TEST(DynamicArrayTest, SubscriptOperator) {
  DynamicArray<int> dynamic_array{1, 2, 3};
  EXPECT_EQ(dynamic_array[1], 2);

  dynamic_array[1] = 5;
  EXPECT_EQ(dynamic_array[1], 5);
}

TEST(DynamicArrayTest, SubscriptOperator_Const) {
  const DynamicArray<int> dynamic_array{1, 2, 3};
  EXPECT_EQ(dynamic_array[1], 2);
}

TEST(DynamicArrayTest, Front) {
  DynamicArray<int> dynamic_array{1, 2, 3};
  EXPECT_EQ(dynamic_array.Front(), 1);

  dynamic_array.Front() = 5;
  EXPECT_EQ(dynamic_array.Front(), 5);
}

TEST(DynamicArrayTest, Front_Const) {
  const DynamicArray<int> dynamic_array{1, 2, 3};
  EXPECT_EQ(dynamic_array.Front(), 1);
}

TEST(DynamicArrayTest, Back) {
  DynamicArray<int> dynamic_array{1, 2, 3};
  EXPECT_EQ(dynamic_array.Back(), 3);

  dynamic_array.Back() = 5;
  EXPECT_EQ(dynamic_array.Back(), 5);
}

TEST(DynamicArrayTest, Back_Const) {
  const DynamicArray<int> dynamic_array{1, 2, 3};
  EXPECT_EQ(dynamic_array.Back(), 3);
}

TEST(DynamicArrayTest, Data) {
  DynamicArray<int> dynamic_array{1, 2, 3};

  int* const data{dynamic_array.Data()};
  EXPECT_EQ(*data, 1);

  dynamic_array.Front() = 5;
  EXPECT_EQ(*data, 5);
}

TEST(DynamicArrayTest, Data_Const) {
  const DynamicArray<int> dynamic_array{1, 2, 3};

  const int* const data{dynamic_array.Data()};
  EXPECT_EQ(*data, 1);
}

// Iterators

TEST(DynamicArrayTest, Begin) {
  DynamicArray<int> dynamic_array;
  EXPECT_EQ(dynamic_array.begin(), dynamic_array.end());

  dynamic_array = {1, 2};
  auto it{dynamic_array.begin()};
  EXPECT_EQ(*it, 1);
  EXPECT_NE(it, dynamic_array.end());

  *it = 10;
  EXPECT_EQ(dynamic_array.Front(), 10);

  ++it;
  EXPECT_EQ(*it, 2);
  EXPECT_NE(it, dynamic_array.end());

  ++it;
  EXPECT_EQ(it, dynamic_array.end());
}

TEST(DynamicArrayTest, Begin_Const) {
  const DynamicArray<int> dynamic_array{1, 2};

  auto it{dynamic_array.begin()};
  EXPECT_EQ(*it, 1);
  EXPECT_NE(it, dynamic_array.end());

  ++it;
  EXPECT_EQ(*it, 2);
  EXPECT_NE(it, dynamic_array.end());

  ++it;
  EXPECT_EQ(it, dynamic_array.end());
}

TEST(DynamicArrayTest, Cbegin) {
  const DynamicArray<int> dynamic_array{1, 2};

  auto it{dynamic_array.cbegin()};
  EXPECT_EQ(*it, 1);
  EXPECT_NE(it, dynamic_array.cend());

  ++it;
  EXPECT_EQ(*it, 2);
  EXPECT_NE(it, dynamic_array.cend());

  ++it;
  EXPECT_EQ(it, dynamic_array.cend());
}

TEST(DynamicArrayTest, End) {
  DynamicArray<int> dynamic_array{1};
  EXPECT_NE(dynamic_array.end(), dynamic_array.begin());
  EXPECT_EQ(dynamic_array.end(), ++dynamic_array.begin());
}

TEST(DynamicArrayTest, End_Const) {
  const DynamicArray<int> dynamic_array{1};
  EXPECT_NE(dynamic_array.end(), dynamic_array.begin());
  EXPECT_EQ(dynamic_array.end(), ++dynamic_array.begin());
}

TEST(DynamicArrayTest, Cend) {
  const DynamicArray<int> dynamic_array{1};
  EXPECT_NE(dynamic_array.cend(), dynamic_array.cbegin());
  EXPECT_EQ(dynamic_array.cend(), ++dynamic_array.cbegin());
}

TEST(DynamicArrayTest, Rbegin) {
  DynamicArray<int> dynamic_array;
  EXPECT_EQ(dynamic_array.rbegin(), dynamic_array.rend());

  dynamic_array = {1, 2};
  auto it{dynamic_array.rbegin()};
  EXPECT_EQ(*it, 2);
  EXPECT_NE(it, dynamic_array.rend());

  *it = 10;
  EXPECT_EQ(dynamic_array.Back(), 10);

  ++it;
  EXPECT_EQ(*it, 1);
  EXPECT_NE(it, dynamic_array.rend());

  ++it;
  EXPECT_EQ(it, dynamic_array.rend());
}

TEST(DynamicArrayTest, Rbegin_Const) {
  const DynamicArray<int> dynamic_array{1, 2};

  auto it{dynamic_array.rbegin()};
  EXPECT_EQ(*it, 2);
  EXPECT_NE(it, dynamic_array.rend());

  ++it;
  EXPECT_EQ(*it, 1);
  EXPECT_NE(it, dynamic_array.rend());

  ++it;
  EXPECT_EQ(it, dynamic_array.rend());
}

TEST(DynamicArrayTest, Crbegin) {
  const DynamicArray<int> dynamic_array{1, 2};

  auto it{dynamic_array.crbegin()};
  EXPECT_EQ(*it, 2);
  EXPECT_NE(it, dynamic_array.crend());

  ++it;
  EXPECT_EQ(*it, 1);
  EXPECT_NE(it, dynamic_array.crend());

  ++it;
  EXPECT_EQ(it, dynamic_array.crend());
}

TEST(DynamicArrayTest, Rend) {
  DynamicArray<int> dynamic_array{1};
  EXPECT_NE(dynamic_array.rend(), dynamic_array.rbegin());
  EXPECT_EQ(dynamic_array.rend(), ++dynamic_array.rbegin());
}

TEST(DynamicArrayTest, Rend_Const) {
  const DynamicArray<int> dynamic_array{1};
  EXPECT_NE(dynamic_array.rend(), dynamic_array.rbegin());
  EXPECT_EQ(dynamic_array.rend(), ++dynamic_array.rbegin());
}

TEST(DynamicArrayTest, Crend) {
  const DynamicArray<int> dynamic_array{1};
  EXPECT_NE(dynamic_array.crend(), dynamic_array.crbegin());
  EXPECT_EQ(dynamic_array.crend(), ++dynamic_array.crbegin());
}

// Capacity

TEST(DynamicArrayTest, Empty) {
  const DynamicArray<int> empty_dynamic_array;
  EXPECT_TRUE(empty_dynamic_array.Empty());

  const DynamicArray<int> dynamic_array{1, 2, 3};
  EXPECT_FALSE(dynamic_array.Empty());
}

TEST(DynamicArrayTest, Size) {
  DynamicArray<int> dynamic_array;
  EXPECT_EQ(dynamic_array.Size(), 0);

  dynamic_array.PushBack(1);
  EXPECT_EQ(dynamic_array.Size(), 1);
}

TEST(DynamicArrayTest, Reserve) {
  DynamicArray<int> dynamic_array;

  dynamic_array.Reserve(10);
  EXPECT_EQ(dynamic_array.Capacity(), 10);

  dynamic_array.Reserve(5);
  EXPECT_EQ(dynamic_array.Capacity(), 10);
}

TEST(DynamicArrayTest, Capacity) {
  DynamicArray<int> dynamic_array;
  EXPECT_EQ(dynamic_array.Capacity(), 0);

  dynamic_array.PushBack(1);
  EXPECT_EQ(dynamic_array.Capacity(), 1);

  dynamic_array.PushBack(2);
  EXPECT_EQ(dynamic_array.Capacity(), 2);

  dynamic_array.PushBack(3);
  EXPECT_EQ(dynamic_array.Capacity(), 4);
}

TEST(DynamicArrayTest, ShrinkToFit) {
  DynamicArray<int> dynamic_array;
  dynamic_array.Reserve(10);
  dynamic_array = {1, 2, 3};

  EXPECT_EQ(dynamic_array.Size(), 3);
  EXPECT_EQ(dynamic_array.Capacity(), 10);

  dynamic_array.ShrinkToFit();
  EXPECT_EQ(dynamic_array.Capacity(), dynamic_array.Size());
}

// Modifiers

TEST(DynamicArrayTest, Clear) {
  DynamicArray<int> dynamic_array;
  dynamic_array.Reserve(10);
  dynamic_array = {1, 2, 3};

  EXPECT_EQ(dynamic_array.Size(), 3);
  EXPECT_EQ(dynamic_array.Capacity(), 10);

  dynamic_array.Clear();
  EXPECT_EQ(dynamic_array.Size(), 0);
  EXPECT_EQ(dynamic_array.Capacity(), 10);
}

TEST(DynamicArrayTest, Insert_Value) {
  DynamicArray<int> dynamic_array;
  auto inserted{dynamic_array.end()};

  inserted = dynamic_array.Insert(dynamic_array.cend(), 1);
  EXPECT_EQ(*inserted, 1);
  EXPECT_EQ(dynamic_array, (DynamicArray{1}));

  dynamic_array.ShrinkToFit();
  inserted = dynamic_array.Insert(dynamic_array.cbegin(), 2);
  EXPECT_EQ(*inserted, 2);
  EXPECT_EQ(dynamic_array, (DynamicArray{2, 1}));

  dynamic_array.ShrinkToFit();
  inserted = dynamic_array.Insert(dynamic_array.cend(), 3);
  EXPECT_EQ(*inserted, 3);
  EXPECT_EQ(dynamic_array, (DynamicArray{2, 1, 3}));

  dynamic_array.Reserve(7);

  inserted = dynamic_array.Insert(dynamic_array.cbegin() + 1, 4);
  EXPECT_EQ(*inserted, 4);
  EXPECT_EQ(dynamic_array, (DynamicArray{2, 4, 1, 3}));

  inserted = dynamic_array.Insert(dynamic_array.cbegin(), 5);
  EXPECT_EQ(*inserted, 5);
  EXPECT_EQ(dynamic_array, (DynamicArray{5, 2, 4, 1, 3}));

  inserted = dynamic_array.Insert(dynamic_array.cbegin() + 3, 6);
  EXPECT_EQ(*inserted, 6);
  EXPECT_EQ(dynamic_array, (DynamicArray{5, 2, 4, 6, 1, 3}));

  inserted = dynamic_array.Insert(dynamic_array.cend(), 7);
  EXPECT_EQ(*inserted, 7);
  EXPECT_EQ(dynamic_array, (DynamicArray{5, 2, 4, 6, 1, 3, 7}));
}

TEST(DynamicArrayTest, Insert_ValueCount) {
  DynamicArray<int> dynamic_array;
  auto inserted{dynamic_array.end()};

  inserted = dynamic_array.Insert(dynamic_array.cend(), 1, 1);
  EXPECT_EQ(*inserted, 1);
  EXPECT_EQ(dynamic_array, (DynamicArray{1}));

  dynamic_array.ShrinkToFit();
  inserted = dynamic_array.Insert(dynamic_array.cbegin(), 2, 2);
  EXPECT_EQ(*inserted, 2);
  EXPECT_EQ(dynamic_array, (DynamicArray{2, 2, 1}));

  dynamic_array.ShrinkToFit();
  inserted = dynamic_array.Insert(dynamic_array.cend(), 3, 3);
  EXPECT_EQ(*inserted, 3);
  EXPECT_EQ(dynamic_array, (DynamicArray{2, 2, 1, 3, 3, 3}));

  dynamic_array.Reserve(28);

  inserted = dynamic_array.Insert(dynamic_array.cbegin() + 2, 4, 4);
  EXPECT_EQ(*inserted, 4);
  EXPECT_EQ(dynamic_array, (DynamicArray{2, 2, 4, 4, 4, 4, 1, 3, 3, 3}));

  inserted = dynamic_array.Insert(dynamic_array.cbegin(), 5, 5);
  EXPECT_EQ(*inserted, 5);
  EXPECT_EQ(dynamic_array,
            (DynamicArray{5, 5, 5, 5, 5, 2, 2, 4, 4, 4, 4, 1, 3, 3, 3}));

  inserted = dynamic_array.Insert(dynamic_array.cbegin() + 11, 6, 6);
  EXPECT_EQ(*inserted, 6);
  EXPECT_EQ(dynamic_array, (DynamicArray{5, 5, 5, 5, 5, 2, 2, 4, 4, 4, 4,
                                         6, 6, 6, 6, 6, 6, 1, 3, 3, 3}));

  inserted = dynamic_array.Insert(dynamic_array.cend(), 7, 7);
  EXPECT_EQ(*inserted, 7);
  EXPECT_EQ(dynamic_array,
            (DynamicArray{5, 5, 5, 5, 5, 2, 2, 4, 4, 4, 4, 6, 6, 6,
                          6, 6, 6, 1, 3, 3, 3, 7, 7, 7, 7, 7, 7, 7}));
}

TEST(DynamicArrayTest, Insert_Range) {
  const DynamicArray<int> source{1, 11, 111, 2,  22, 33, 3,  44, 4,
                                 5, 55, 666, 66, 6,  7,  77, 777};
  DynamicArray<int> dynamic_array;
  auto inserted{dynamic_array.end()};

  inserted = dynamic_array.Insert(dynamic_array.cend(), source.cbegin() + 0,
                                  source.cbegin() + 3);
  EXPECT_EQ(*inserted, 1);
  EXPECT_EQ(dynamic_array, (DynamicArray{1, 11, 111}));

  dynamic_array.ShrinkToFit();
  inserted = dynamic_array.Insert(dynamic_array.cbegin() + 0,
                                  source.cbegin() + 3, source.cbegin() + 5);
  EXPECT_EQ(*inserted, 2);
  EXPECT_EQ(dynamic_array, (DynamicArray{2, 22, 1, 11, 111}));

  dynamic_array.ShrinkToFit();
  inserted = dynamic_array.Insert(dynamic_array.cend(), source.cbegin() + 5,
                                  source.cbegin() + 7);
  EXPECT_EQ(*inserted, 33);
  EXPECT_EQ(dynamic_array, (DynamicArray{2, 22, 1, 11, 111, 33, 3}));

  dynamic_array.ShrinkToFit();
  inserted = dynamic_array.Insert(dynamic_array.cbegin() + 2,
                                  source.cbegin() + 7, source.cbegin() + 9);
  EXPECT_EQ(*inserted, 44);
  EXPECT_EQ(dynamic_array, (DynamicArray{2, 22, 44, 4, 1, 11, 111, 33, 3}));

  dynamic_array.Reserve(17);

  inserted = dynamic_array.Insert(dynamic_array.cbegin() + 7,
                                  source.cbegin() + 9, source.cbegin() + 11);
  EXPECT_EQ(*inserted, 5);
  EXPECT_EQ(dynamic_array,
            (DynamicArray{2, 22, 44, 4, 1, 11, 111, 5, 55, 33, 3}));

  inserted = dynamic_array.Insert(dynamic_array.cbegin() + 0,
                                  source.cbegin() + 11, source.cbegin() + 14);
  EXPECT_EQ(*inserted, 666);
  EXPECT_EQ(dynamic_array,
            (DynamicArray{666, 66, 6, 2, 22, 44, 4, 1, 11, 111, 5, 55, 33, 3}));

  inserted = dynamic_array.Insert(dynamic_array.cend(), source.cbegin() + 14,
                                  source.cbegin() + 17);
  EXPECT_EQ(*inserted, 7);
  EXPECT_EQ(dynamic_array, (DynamicArray{666, 66, 6, 2, 22, 44, 4, 1, 11, 111,
                                         5, 55, 33, 3, 7, 77, 777}));
}

TEST(DynamicArrayTest, Insert_InitializerList) {
  DynamicArray<int> dynamic_array;
  auto inserted{dynamic_array.end()};

  inserted = dynamic_array.Insert(dynamic_array.cend(), {1, 11, 111});
  EXPECT_EQ(*inserted, 1);
  EXPECT_EQ(dynamic_array, (DynamicArray{1, 11, 111}));

  dynamic_array.ShrinkToFit();
  inserted = dynamic_array.Insert(dynamic_array.cbegin() + 0, {2, 22});
  EXPECT_EQ(*inserted, 2);
  EXPECT_EQ(dynamic_array, (DynamicArray{2, 22, 1, 11, 111}));

  dynamic_array.ShrinkToFit();
  inserted = dynamic_array.Insert(dynamic_array.cend(), {33, 3});
  EXPECT_EQ(*inserted, 33);
  EXPECT_EQ(dynamic_array, (DynamicArray{2, 22, 1, 11, 111, 33, 3}));

  dynamic_array.ShrinkToFit();
  inserted = dynamic_array.Insert(dynamic_array.cbegin() + 2, {44, 4});
  EXPECT_EQ(*inserted, 44);
  EXPECT_EQ(dynamic_array, (DynamicArray{2, 22, 44, 4, 1, 11, 111, 33, 3}));

  dynamic_array.Reserve(17);

  inserted = dynamic_array.Insert(dynamic_array.cbegin() + 7, {5, 55});
  EXPECT_EQ(*inserted, 5);
  EXPECT_EQ(dynamic_array,
            (DynamicArray{2, 22, 44, 4, 1, 11, 111, 5, 55, 33, 3}));

  inserted = dynamic_array.Insert(dynamic_array.cbegin() + 0, {666, 66, 6});
  EXPECT_EQ(*inserted, 666);
  EXPECT_EQ(dynamic_array,
            (DynamicArray{666, 66, 6, 2, 22, 44, 4, 1, 11, 111, 5, 55, 33, 3}));

  inserted = dynamic_array.Insert(dynamic_array.cend(), {7, 77, 777});
  EXPECT_EQ(*inserted, 7);
  EXPECT_EQ(dynamic_array, (DynamicArray{666, 66, 6, 2, 22, 44, 4, 1, 11, 111,
                                         5, 55, 33, 3, 7, 77, 777}));
}

TEST(DynamicArrayTest, Erase_Element) {
  DynamicArray<int> dynamic_array{0, 1, 2, 3, 4, 5};
  auto next{dynamic_array.end()};

  next = dynamic_array.Erase(dynamic_array.cbegin() + 0);
  EXPECT_EQ(*next, 1);
  EXPECT_EQ(dynamic_array, (DynamicArray{1, 2, 3, 4, 5}));

  next = dynamic_array.Erase(dynamic_array.cbegin() + 1);
  EXPECT_EQ(*next, 3);
  EXPECT_EQ(dynamic_array, (DynamicArray{1, 3, 4, 5}));

  next = dynamic_array.Erase(dynamic_array.cbegin() + 2);
  EXPECT_EQ(*next, 5);
  EXPECT_EQ(dynamic_array, (DynamicArray{1, 3, 5}));

  next = dynamic_array.Erase(dynamic_array.cbegin() + 2);
  EXPECT_EQ(next, dynamic_array.end());
  EXPECT_EQ(dynamic_array, (DynamicArray{1, 3}));

  next = dynamic_array.Erase(dynamic_array.cbegin() + 1);
  EXPECT_EQ(next, dynamic_array.end());
  EXPECT_EQ(dynamic_array, (DynamicArray{1}));

  next = dynamic_array.Erase(dynamic_array.cbegin() + 0);
  EXPECT_EQ(next, dynamic_array.end());
  EXPECT_TRUE(dynamic_array.Empty());
}

TEST(DynamicArrayTest, Erase_Range) {
  DynamicArray<int> dynamic_array{0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
  auto next{dynamic_array.end()};

  next = dynamic_array.Erase(dynamic_array.cbegin() + 0,
                             dynamic_array.cbegin() + 3);
  EXPECT_EQ(*next, 3);
  EXPECT_EQ(dynamic_array, (DynamicArray{3, 4, 5, 6, 7, 8, 9}));

  next = dynamic_array.Erase(dynamic_array.cbegin() + 2,
                             dynamic_array.cbegin() + 4);
  EXPECT_EQ(*next, 7);
  EXPECT_EQ(dynamic_array, (DynamicArray{3, 4, 7, 8, 9}));

  next = dynamic_array.Erase(dynamic_array.cbegin() + 1,
                             dynamic_array.cbegin() + 4);
  EXPECT_EQ(*next, 9);
  EXPECT_EQ(dynamic_array, (DynamicArray{3, 9}));

  next = dynamic_array.Erase(dynamic_array.cbegin() + 0, dynamic_array.cend());
  EXPECT_EQ(next, dynamic_array.end());
  EXPECT_TRUE(dynamic_array.Empty());
}

TEST(DynamicArrayTest, PushBack) {
  DynamicArray<int> dynamic_array;

  dynamic_array.PushBack(1);
  EXPECT_EQ(dynamic_array.Size(), 1);
  EXPECT_EQ(dynamic_array.Back(), 1);
}

TEST(DynamicArrayTest, PopBack) {
  DynamicArray<int> dynamic_array{1, 2, 3};

  dynamic_array.PopBack();
  EXPECT_EQ(dynamic_array.Size(), 2);
  EXPECT_EQ(dynamic_array.Back(), 2);
}

TEST(DynamicArrayTest, Resize) {
  DynamicArray<int> dynamic_array;

  dynamic_array.Resize(2);
  EXPECT_EQ(dynamic_array.Capacity(), 2);
  EXPECT_EQ(dynamic_array, (DynamicArray{0, 0}));

  dynamic_array.Resize(5, 1);
  EXPECT_EQ(dynamic_array.Capacity(), 5);
  EXPECT_EQ(dynamic_array, (DynamicArray{0, 0, 1, 1, 1}));

  dynamic_array.Resize(3);
  EXPECT_EQ(dynamic_array.Capacity(), 5);
  EXPECT_EQ(dynamic_array, (DynamicArray{0, 0, 1}));
}

TEST(DynamicArrayTest, Swap) {
  DynamicArray<int> a{1, 2, 3};
  DynamicArray<int> b{4, 5, 6};
  const DynamicArray<int> expected_a{b};
  const DynamicArray<int> expected_b{a};

  a.Swap(b);
  EXPECT_EQ(a, expected_a);
  EXPECT_EQ(b, expected_b);
}

// Comparison operators

TEST(DynamicArrayTest, EqualOperator) {
  const DynamicArray<int> a{1, 2, 3};
  const DynamicArray<int> b{1, 2, 3};
  EXPECT_EQ(a, b);
}

TEST(DynamicArrayTest, NotEqualOperator) {
  const DynamicArray<int> a{1, 2, 3};
  const DynamicArray<int> b{4, 5, 6};
  EXPECT_NE(a, b);
}

TEST(DynamicArrayTest, LessOperator) {
  const DynamicArray<int> a{1, 2, 3};
  const DynamicArray<int> b{4, 5, 6};
  EXPECT_LT(a, b);
}

TEST(DynamicArrayTest, LessEqualOperator) {
  DynamicArray<int> a{1, 2, 3};
  DynamicArray<int> b{4, 5, 6};
  EXPECT_LE(a, b);

  a = b = {1, 2, 3};
  EXPECT_LE(a, b);
}

TEST(DynamicArrayTest, GreaterOperator) {
  const DynamicArray<int> a{4, 5, 6};
  const DynamicArray<int> b{1, 2, 3};
  EXPECT_GT(a, b);
}

TEST(DynamicArrayTest, GreaterEqualOperator) {
  DynamicArray<int> a{4, 5, 6};
  DynamicArray<int> b{1, 2, 3};
  EXPECT_GE(a, b);

  a = b = {1, 2, 3};
  EXPECT_GE(a, b);
}
