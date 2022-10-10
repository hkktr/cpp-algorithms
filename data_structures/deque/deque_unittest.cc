#include "deque.h"

#include <gtest/gtest.h>

#include <stdexcept>
#include <utility>

// Constructors

TEST(DequeTest, Constructor) {
  const Deque<int> deque;
  EXPECT_EQ(deque.Size(), 0);
}

TEST(DequeTest, CopyConstructor) {
  const Deque<int> deque{1, 2, 3};

  const Deque<int> copy{deque};
  EXPECT_EQ(copy, deque);
}

TEST(DequeTest, MoveConstructor) {
  Deque<int> deque{1, 2, 3};

  const Deque<int> moved_deque{std::move(deque)};
  EXPECT_EQ(moved_deque.Size(), 3);
  EXPECT_EQ(deque.Size(), 0);
}

TEST(DequeTest, InitializerListConstructor) {
  const Deque<int> deque{1, 2, 3};
  EXPECT_EQ(deque.Size(), 3);
  EXPECT_EQ(deque[0], 1);
  EXPECT_EQ(deque[1], 2);
  EXPECT_EQ(deque[2], 3);
}

// Assignments

TEST(DequeTest, CopyAssignment) {
  const Deque<int> deque{1, 2, 3};
  Deque<int> copy;

  copy = deque;
  EXPECT_EQ(copy, deque);
}

TEST(DequeTest, MoveAssignment) {
  Deque<int> deque{1, 2, 3};
  Deque<int> moved_deque;

  moved_deque = std::move(deque);
  EXPECT_EQ(moved_deque.Size(), 3);
  EXPECT_EQ(deque.Size(), 0);
}

TEST(DequeTest, InitializerListAssignment) {
  Deque<int> deque;

  deque = {1, 2, 3};
  EXPECT_EQ(deque.Size(), 3);
  EXPECT_EQ(deque[0], 1);
  EXPECT_EQ(deque[1], 2);
  EXPECT_EQ(deque[2], 3);
}

// Element access

TEST(DequeTest, At) {
  Deque<int> deque{1, 2, 3};
  EXPECT_THROW(deque.At(5), std::out_of_range);
  EXPECT_EQ(deque.At(1), 2);

  deque.At(2) = 5;
  EXPECT_EQ(deque.At(2), 5);
}

TEST(DequeTest, At_Const) {
  const Deque<int> deque{1, 2, 3};
  EXPECT_THROW(deque.At(5), std::out_of_range);
  EXPECT_EQ(deque.At(1), 2);
}

TEST(DequeTest, SubscriptOperator) {
  Deque<int> deque{1, 2, 3};
  EXPECT_EQ(deque[1], 2);

  deque[1] = 5;
  EXPECT_EQ(deque[1], 5);
}

TEST(DequeTest, SubscriptOperator_Const) {
  const Deque<int> deque{1, 2, 3};
  EXPECT_EQ(deque[1], 2);
}

TEST(DequeTest, Front) {
  Deque<int> deque{1, 2, 3};
  EXPECT_EQ(deque.Front(), 1);

  deque.Front() = 5;
  EXPECT_EQ(deque.Front(), 5);
}

TEST(DequeTest, Front_Const) {
  const Deque<int> deque{1, 2, 3};
  EXPECT_EQ(deque.Front(), 1);
}

TEST(DequeTest, Back) {
  Deque<int> deque{1, 2, 3};
  EXPECT_EQ(deque.Back(), 3);

  deque.Back() = 5;
  EXPECT_EQ(deque.Back(), 5);
}

TEST(DequeTest, Back_Const) {
  const Deque<int> deque{1, 2, 3};
  EXPECT_EQ(deque.Back(), 3);
}

// Iterators

TEST(DequeTest, Begin) {
  Deque<int> deque;
  EXPECT_EQ(deque.begin(), deque.end());

  deque = {1, 2};
  auto it{deque.begin()};
  EXPECT_EQ(*it, 1);
  EXPECT_NE(it, deque.end());

  *it = 10;
  EXPECT_EQ(deque.Front(), 10);

  ++it;
  EXPECT_EQ(*it, 2);
  EXPECT_NE(it, deque.end());

  ++it;
  EXPECT_EQ(it, deque.end());
}

TEST(DequeTest, Begin_Const) {
  const Deque<int> deque{1, 2};

  auto it{deque.begin()};
  EXPECT_EQ(*it, 1);
  EXPECT_NE(it, deque.end());

  ++it;
  EXPECT_EQ(*it, 2);
  EXPECT_NE(it, deque.end());

  ++it;
  EXPECT_EQ(it, deque.end());
}

TEST(DequeTest, Cbegin) {
  const Deque<int> deque{1, 2};

  auto it{deque.cbegin()};
  EXPECT_EQ(*it, 1);
  EXPECT_NE(it, deque.cend());

  ++it;
  EXPECT_EQ(*it, 2);
  EXPECT_NE(it, deque.cend());

  ++it;
  EXPECT_EQ(it, deque.cend());
}

TEST(DequeTest, End) {
  Deque<int> deque{1};
  EXPECT_NE(deque.end(), deque.begin());
  EXPECT_EQ(deque.end(), ++deque.begin());
}

TEST(DequeTest, End_Const) {
  const Deque<int> deque{1};
  EXPECT_NE(deque.end(), deque.begin());
  EXPECT_EQ(deque.end(), ++deque.begin());
}

TEST(DequeTest, Cend) {
  const Deque<int> deque{1};
  EXPECT_NE(deque.cend(), deque.cbegin());
  EXPECT_EQ(deque.cend(), ++deque.cbegin());
}

TEST(DequeTest, Rbegin) {
  Deque<int> deque;
  EXPECT_EQ(deque.rbegin(), deque.rend());

  deque = {1, 2};
  auto it{deque.rbegin()};
  EXPECT_EQ(*it, 2);
  EXPECT_NE(it, deque.rend());

  *it = 10;
  EXPECT_EQ(deque.Back(), 10);

  ++it;
  EXPECT_EQ(*it, 1);
  EXPECT_NE(it, deque.rend());

  ++it;
  EXPECT_EQ(it, deque.rend());
}

TEST(DequeTest, Rbegin_Const) {
  const Deque<int> deque{1, 2};

  auto it{deque.rbegin()};
  EXPECT_EQ(*it, 2);
  EXPECT_NE(it, deque.rend());

  ++it;
  EXPECT_EQ(*it, 1);
  EXPECT_NE(it, deque.rend());

  ++it;
  EXPECT_EQ(it, deque.rend());
}

TEST(DequeTest, Crbegin) {
  const Deque<int> deque{1, 2};

  auto it{deque.crbegin()};
  EXPECT_EQ(*it, 2);
  EXPECT_NE(it, deque.crend());

  ++it;
  EXPECT_EQ(*it, 1);
  EXPECT_NE(it, deque.crend());

  ++it;
  EXPECT_EQ(it, deque.crend());
}

TEST(DequeTest, Rend) {
  Deque<int> deque{1};
  EXPECT_NE(deque.rend(), deque.rbegin());
  EXPECT_EQ(deque.rend(), ++deque.rbegin());
}

TEST(DequeTest, Rend_Const) {
  const Deque<int> deque{1};
  EXPECT_NE(deque.rend(), deque.rbegin());
  EXPECT_EQ(deque.rend(), ++deque.rbegin());
}

TEST(DequeTest, Crend) {
  const Deque<int> deque{1};
  EXPECT_NE(deque.crend(), deque.crbegin());
  EXPECT_EQ(deque.crend(), ++deque.crbegin());
}

// Capacity

TEST(DequeTest, Empty) {
  const Deque<int> empty_deque;
  EXPECT_TRUE(empty_deque.Empty());

  const Deque<int> deque{1, 2, 3};
  EXPECT_FALSE(deque.Empty());
}

TEST(DequeTest, Size) {
  Deque<int> deque;
  EXPECT_EQ(deque.Size(), 0);

  deque.PushBack(1);
  EXPECT_EQ(deque.Size(), 1);
}

TEST(DequeTest, ShrinkToFit) {
  Deque<int> deque;
  for (std::size_t i{1}; i <= 10; ++i) {
    deque.PushBack(i);
  }

  deque.ShrinkToFit();
  EXPECT_EQ(deque, (Deque{1, 2, 3, 4, 5, 6, 7, 8, 9, 10}));
}

// Modifiers

TEST(DequeTest, Clear) {
  Deque<int> deque{1, 2, 3};
  EXPECT_EQ(deque.Size(), 3);

  deque.Clear();
  EXPECT_EQ(deque.Size(), 0);
}

TEST(DequeTest, Insert_Value) {
  Deque<int> deque;
  auto inserted{deque.end()};

  inserted = deque.Insert(deque.cend(), 1);
  EXPECT_EQ(*inserted, 1);
  EXPECT_EQ(deque, (Deque{1}));

  inserted = deque.Insert(deque.cbegin(), 2);
  EXPECT_EQ(*inserted, 2);
  EXPECT_EQ(deque, (Deque{2, 1}));

  inserted = deque.Insert(deque.cend(), 3);
  EXPECT_EQ(*inserted, 3);
  EXPECT_EQ(deque, (Deque{2, 1, 3}));

  inserted = deque.Insert(deque.cbegin() + 1, 4);
  EXPECT_EQ(*inserted, 4);
  EXPECT_EQ(deque, (Deque{2, 4, 1, 3}));

  inserted = deque.Insert(deque.cbegin(), 5);
  EXPECT_EQ(*inserted, 5);
  EXPECT_EQ(deque, (Deque{5, 2, 4, 1, 3}));

  inserted = deque.Insert(deque.cbegin() + 3, 6);
  EXPECT_EQ(*inserted, 6);
  EXPECT_EQ(deque, (Deque{5, 2, 4, 6, 1, 3}));

  inserted = deque.Insert(deque.cend(), 7);
  EXPECT_EQ(*inserted, 7);
  EXPECT_EQ(deque, (Deque{5, 2, 4, 6, 1, 3, 7}));
}

TEST(DequeTest, Insert_ValueCount) {
  Deque<int> deque;
  auto inserted{deque.end()};

  inserted = deque.Insert(deque.cbegin(), 1, 1);
  EXPECT_EQ(*inserted, 1);
  EXPECT_EQ(deque, (Deque{1}));

  inserted = deque.Insert(deque.cbegin(), 2, 2);
  EXPECT_EQ(*inserted, 2);
  EXPECT_EQ(deque, (Deque{2, 2, 1}));

  inserted = deque.Insert(deque.cend(), 3, 3);
  EXPECT_EQ(*inserted, 3);
  EXPECT_EQ(deque, (Deque{2, 2, 1, 3, 3, 3}));

  inserted = deque.Insert(deque.cbegin() + 2, 4, 4);
  EXPECT_EQ(*inserted, 4);
  EXPECT_EQ(deque, (Deque{2, 2, 4, 4, 4, 4, 1, 3, 3, 3}));

  inserted = deque.Insert(deque.cbegin(), 5, 5);
  EXPECT_EQ(*inserted, 5);
  EXPECT_EQ(deque, (Deque{5, 5, 5, 5, 5, 2, 2, 4, 4, 4, 4, 1, 3, 3, 3}));

  inserted = deque.Insert(deque.cbegin() + 11, 6, 6);
  EXPECT_EQ(*inserted, 6);
  EXPECT_EQ(deque, (Deque{5, 5, 5, 5, 5, 2, 2, 4, 4, 4, 4,
                          6, 6, 6, 6, 6, 6, 1, 3, 3, 3}));

  inserted = deque.Insert(deque.cend(), 7, 7);
  EXPECT_EQ(*inserted, 7);
  EXPECT_EQ(deque, (Deque{5, 5, 5, 5, 5, 2, 2, 4, 4, 4, 4, 6, 6, 6,
                          6, 6, 6, 1, 3, 3, 3, 7, 7, 7, 7, 7, 7, 7}));
}

TEST(DequeTest, Insert_Range) {
  const Deque<int> source{1, 11, 111, 2,  22, 33, 3,  44, 4,
                          5, 55, 666, 66, 6,  7,  77, 777};
  Deque<int> deque;
  auto inserted{deque.end()};

  inserted = deque.Insert(deque.cend(), source.cbegin(), source.cbegin() + 3);
  EXPECT_EQ(*inserted, 1);
  EXPECT_EQ(deque, (Deque{1, 11, 111}));

  inserted =
      deque.Insert(deque.cbegin(), source.cbegin() + 3, source.cbegin() + 5);
  EXPECT_EQ(*inserted, 2);
  EXPECT_EQ(deque, (Deque{2, 22, 1, 11, 111}));

  inserted =
      deque.Insert(deque.cend(), source.cbegin() + 5, source.cbegin() + 7);
  EXPECT_EQ(*inserted, 33);
  EXPECT_EQ(deque, (Deque{2, 22, 1, 11, 111, 33, 3}));

  inserted = deque.Insert(deque.cbegin() + 2, source.cbegin() + 7,
                          source.cbegin() + 9);
  EXPECT_EQ(*inserted, 44);
  EXPECT_EQ(deque, (Deque{2, 22, 44, 4, 1, 11, 111, 33, 3}));

  inserted = deque.Insert(deque.cbegin() + 7, source.cbegin() + 9,
                          source.cbegin() + 11);
  EXPECT_EQ(*inserted, 5);
  EXPECT_EQ(deque, (Deque{2, 22, 44, 4, 1, 11, 111, 5, 55, 33, 3}));

  inserted =
      deque.Insert(deque.cbegin(), source.cbegin() + 11, source.cbegin() + 14);
  EXPECT_EQ(*inserted, 666);
  EXPECT_EQ(deque, (Deque{666, 66, 6, 2, 22, 44, 4, 1, 11, 111, 5, 55, 33, 3}));

  inserted = deque.Insert(deque.cend(), source.cbegin() + 14, source.cend());
  EXPECT_EQ(*inserted, 7);
  EXPECT_EQ(deque, (Deque{666, 66, 6, 2, 22, 44, 4, 1, 11, 111, 5, 55, 33, 3, 7,
                          77, 777}));
}

TEST(DequeTest, Insert_InitializerList) {
  Deque<int> deque;
  auto inserted{deque.end()};

  inserted = deque.Insert(deque.cend(), {1, 11, 111});
  EXPECT_EQ(*inserted, 1);
  EXPECT_EQ(deque, (Deque{1, 11, 111}));

  inserted = deque.Insert(deque.cbegin(), {2, 22});
  EXPECT_EQ(*inserted, 2);
  EXPECT_EQ(deque, (Deque{2, 22, 1, 11, 111}));

  inserted = deque.Insert(deque.cend(), {33, 3});
  EXPECT_EQ(*inserted, 33);
  EXPECT_EQ(deque, (Deque{2, 22, 1, 11, 111, 33, 3}));

  inserted = deque.Insert(deque.cbegin() + 2, {44, 4});
  EXPECT_EQ(*inserted, 44);
  EXPECT_EQ(deque, (Deque{2, 22, 44, 4, 1, 11, 111, 33, 3}));

  inserted = deque.Insert(deque.cbegin() + 7, {5, 55});
  EXPECT_EQ(*inserted, 5);
  EXPECT_EQ(deque, (Deque{2, 22, 44, 4, 1, 11, 111, 5, 55, 33, 3}));

  inserted = deque.Insert(deque.cbegin(), {666, 66, 6});
  EXPECT_EQ(*inserted, 666);
  EXPECT_EQ(deque, (Deque{666, 66, 6, 2, 22, 44, 4, 1, 11, 111, 5, 55, 33, 3}));

  inserted = deque.Insert(deque.cend(), {7, 77, 777});
  EXPECT_EQ(*inserted, 7);
  EXPECT_EQ(deque, (Deque{666, 66, 6, 2, 22, 44, 4, 1, 11, 111, 5, 55, 33, 3, 7,
                          77, 777}));
}

TEST(DequeTest, Erase_Element) {
  Deque<int> deque{0, 1, 2, 3, 4, 5};
  auto next{deque.end()};

  next = deque.Erase(deque.cbegin());
  EXPECT_EQ(*next, 1);
  EXPECT_EQ(deque, (Deque{1, 2, 3, 4, 5}));

  next = deque.Erase(deque.cbegin() + 1);
  EXPECT_EQ(*next, 3);
  EXPECT_EQ(deque, (Deque{1, 3, 4, 5}));

  next = deque.Erase(deque.cbegin() + 2);
  EXPECT_EQ(*next, 5);
  EXPECT_EQ(deque, (Deque{1, 3, 5}));

  next = deque.Erase(deque.cend() - 1);
  EXPECT_EQ(next, deque.end());
  EXPECT_EQ(deque, (Deque{1, 3}));
}

TEST(DequeTest, Erase_Range) {
  Deque<int> deque{0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
  auto next{deque.end()};

  next = deque.Erase(deque.cbegin(), deque.cbegin() + 3);
  EXPECT_EQ(*next, 3);
  EXPECT_EQ(deque, (Deque{3, 4, 5, 6, 7, 8, 9}));

  next = deque.Erase(deque.cbegin() + 2, deque.cbegin() + 4);
  EXPECT_EQ(*next, 7);
  EXPECT_EQ(deque, (Deque{3, 4, 7, 8, 9}));

  next = deque.Erase(deque.cbegin() + 1, deque.cbegin() + 4);
  EXPECT_EQ(*next, 9);
  EXPECT_EQ(deque, (Deque{3, 9}));

  next = deque.Erase(deque.cbegin(), deque.cend());
  EXPECT_EQ(next, deque.end());
  EXPECT_TRUE(deque.Empty());
}

TEST(DequeTest, PushBack) {
  Deque<int> deque;

  deque.PushBack(1);
  EXPECT_EQ(deque.Size(), 1);
  EXPECT_EQ(deque.Back(), 1);
}

TEST(DequeTest, PopBack) {
  Deque<int> deque{1, 2, 3};

  deque.PopBack();
  EXPECT_EQ(deque.Size(), 2);
  EXPECT_EQ(deque.Back(), 2);
}

TEST(DequeTest, PushFront) {
  Deque<int> deque;

  deque.PushFront(1);
  EXPECT_EQ(deque.Size(), 1);
  EXPECT_EQ(deque.Front(), 1);
}

TEST(DequeTest, PopFront) {
  Deque<int> deque{1, 2, 3};

  deque.PopFront();
  EXPECT_EQ(deque.Size(), 2);
  EXPECT_EQ(deque.Front(), 2);
}

TEST(DequeTest, Resize) {
  Deque<int> deque;

  deque.Resize(2);
  EXPECT_EQ(deque, (Deque{0, 0}));

  deque.Resize(5, 1);
  EXPECT_EQ(deque, (Deque{0, 0, 1, 1, 1}));

  deque.Resize(3);
  EXPECT_EQ(deque, (Deque{0, 0, 1}));
}

TEST(DequeTest, Swap) {
  Deque<int> a{1, 2, 3};
  Deque<int> b{4, 5, 6};
  const Deque<int> expected_a{b};
  const Deque<int> expected_b{a};

  a.Swap(b);
  EXPECT_EQ(a, expected_a);
  EXPECT_EQ(b, expected_b);
}

// Comparison operators

TEST(DequeTest, EqualOperator) {
  const Deque<int> a{1, 2, 3};
  const Deque<int> b{1, 2, 3};
  EXPECT_EQ(a, b);
}

TEST(DequeTest, NotEqualOperator) {
  const Deque<int> a{1, 2, 3};
  const Deque<int> b{4, 5, 6};
  EXPECT_NE(a, b);
}

TEST(DequeTest, LessOperator) {
  const Deque<int> a{1, 2, 3};
  const Deque<int> b{4, 5, 6};
  EXPECT_LT(a, b);
}

TEST(DequeTest, LessEqualOperator) {
  Deque<int> a{1, 2, 3};
  Deque<int> b{4, 5, 6};
  EXPECT_LE(a, b);

  a = b = {1, 2, 3};
  EXPECT_LE(a, b);
}

TEST(DequeTest, GreaterOperator) {
  const Deque<int> a{4, 5, 6};
  const Deque<int> b{1, 2, 3};
  EXPECT_GT(a, b);
}

TEST(DequeTest, GreaterEqualOperator) {
  Deque<int> a{4, 5, 6};
  Deque<int> b{1, 2, 3};
  EXPECT_GE(a, b);

  a = b = {1, 2, 3};
  EXPECT_GE(a, b);
}
