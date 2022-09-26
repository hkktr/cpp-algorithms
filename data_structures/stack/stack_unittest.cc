#include "stack.h"

#include <gtest/gtest.h>

#include <utility>

// Constructors

TEST(StackTest, Constructor) {
  const Stack<int> stack;
  EXPECT_EQ(stack.Size(), 0);
}

TEST(StackTest, CopyConstructor) {
  const Stack<int> stack{1, 2, 3};

  const Stack<int> copy{stack};
  EXPECT_EQ(copy, stack);
}

TEST(StackTest, MoveConstructor) {
  Stack<int> stack{1, 2, 3};

  const Stack<int> moved_stack{std::move(stack)};
  EXPECT_EQ(moved_stack.Size(), 3);
  EXPECT_EQ(stack.Size(), 0);
}

TEST(StackTest, InitializerListConstructor) {
  Stack<int> stack{1, 2, 3};
  EXPECT_EQ(stack.Size(), 3);
  EXPECT_EQ(stack.Top(), 3);

  stack.Pop();
  EXPECT_EQ(stack.Top(), 2);

  stack.Pop();
  EXPECT_EQ(stack.Top(), 1);

  stack.Pop();
  EXPECT_TRUE(stack.Empty());
}

// Assignments

TEST(StackTest, CopyAssignment) {
  const Stack<int> stack{1, 2, 3};
  Stack<int> copy;

  copy = stack;
  EXPECT_EQ(copy, stack);
}

TEST(StackTest, MoveAssignment) {
  Stack<int> stack{1, 2, 3};
  Stack<int> moved_stack;

  moved_stack = std::move(stack);
  EXPECT_EQ(moved_stack.Size(), 3);
  EXPECT_EQ(stack.Size(), 0);
}

TEST(StackTest, InitializerListAssignment) {
  Stack<int> stack;

  stack = {1, 2, 3};
  EXPECT_EQ(stack.Size(), 3);
  EXPECT_EQ(stack.Top(), 3);

  stack.Pop();
  EXPECT_EQ(stack.Top(), 2);

  stack.Pop();
  EXPECT_EQ(stack.Top(), 1);

  stack.Pop();
  EXPECT_TRUE(stack.Empty());
}

// Element access

TEST(StackTest, Top) {
  Stack<int> stack{1, 2, 3};
  EXPECT_EQ(stack.Top(), 3);

  stack.Top() = 5;
  EXPECT_EQ(stack.Top(), 5);
}

TEST(StackTest, Top_Const) {
  const Stack<int> stack{1, 2, 3};
  EXPECT_EQ(stack.Top(), 3);
}

// Iterators

TEST(StackTest, Begin) {
  Stack<int> stack;
  EXPECT_EQ(stack.begin(), stack.end());

  stack = {1, 2};
  auto it{stack.begin()};
  EXPECT_EQ(*it, 1);
  EXPECT_NE(it, stack.end());

  ++it;
  EXPECT_EQ(*it, 2);
  EXPECT_NE(it, stack.end());

  *it = 10;
  EXPECT_EQ(stack.Top(), 10);

  ++it;
  EXPECT_EQ(it, stack.end());
}

TEST(StackTest, Begin_Const) {
  const Stack<int> stack{1, 2};

  auto it{stack.begin()};
  EXPECT_EQ(*it, 1);
  EXPECT_NE(it, stack.end());

  ++it;
  EXPECT_EQ(*it, 2);
  EXPECT_NE(it, stack.end());

  ++it;
  EXPECT_EQ(it, stack.end());
}

TEST(StackTest, Cbegin) {
  const Stack<int> stack{1, 2};

  auto it{stack.cbegin()};
  EXPECT_EQ(*it, 1);
  EXPECT_NE(it, stack.cend());

  ++it;
  EXPECT_EQ(*it, 2);
  EXPECT_NE(it, stack.cend());

  ++it;
  EXPECT_EQ(it, stack.cend());
}

TEST(StackTest, End) {
  Stack<int> stack{1};
  EXPECT_NE(stack.end(), stack.begin());
  EXPECT_EQ(stack.end(), ++stack.begin());
}

TEST(StackTest, End_Const) {
  const Stack<int> stack{1};
  EXPECT_NE(stack.end(), stack.begin());
  EXPECT_EQ(stack.end(), ++stack.begin());
}

TEST(StackTest, Cend) {
  const Stack<int> stack{1};
  EXPECT_NE(stack.cend(), stack.cbegin());
  EXPECT_EQ(stack.cend(), ++stack.cbegin());
}

TEST(StackTest, Rbegin) {
  Stack<int> stack;
  EXPECT_EQ(stack.rbegin(), stack.rend());

  stack = {1, 2};
  auto it{stack.rbegin()};
  EXPECT_EQ(*it, 2);
  EXPECT_NE(it, stack.rend());

  *it = 10;
  EXPECT_EQ(stack.Top(), 10);

  ++it;
  EXPECT_EQ(*it, 1);
  EXPECT_NE(it, stack.rend());

  ++it;
  EXPECT_EQ(it, stack.rend());
}

TEST(StackTest, Rbegin_Const) {
  const Stack<int> stack{1, 2};

  auto it{stack.rbegin()};
  EXPECT_EQ(*it, 2);
  EXPECT_NE(it, stack.rend());

  ++it;
  EXPECT_EQ(*it, 1);
  EXPECT_NE(it, stack.rend());

  ++it;
  EXPECT_EQ(it, stack.rend());
}

TEST(StackTest, Crbegin) {
  const Stack<int> stack{1, 2};

  auto it{stack.crbegin()};
  EXPECT_EQ(*it, 2);
  EXPECT_NE(it, stack.crend());

  ++it;
  EXPECT_EQ(*it, 1);
  EXPECT_NE(it, stack.crend());

  ++it;
  EXPECT_EQ(it, stack.crend());
}

TEST(StackTest, Rend) {
  Stack<int> stack{1};
  EXPECT_NE(stack.rend(), stack.rbegin());
  EXPECT_EQ(stack.rend(), ++stack.rbegin());
}

TEST(StackTest, Rend_Const) {
  const Stack<int> stack{1};
  EXPECT_NE(stack.rend(), stack.rbegin());
  EXPECT_EQ(stack.rend(), ++stack.rbegin());
}

TEST(StackTest, Crend) {
  const Stack<int> stack{1};
  EXPECT_NE(stack.crend(), stack.crbegin());
  EXPECT_EQ(stack.crend(), ++stack.crbegin());
}

// Capacity

TEST(StackTest, Empty) {
  const Stack<int> empty_stack;
  EXPECT_TRUE(empty_stack.Empty());

  const Stack<int> stack{1, 2, 3};
  EXPECT_FALSE(stack.Empty());
}

TEST(StackTest, Size) {
  Stack<int> stack;
  EXPECT_EQ(stack.Size(), 0);

  stack.Push(1);
  EXPECT_EQ(stack.Size(), 1);
}

// Modifiers

TEST(StackTest, Push) {
  Stack<int> stack;

  stack.Push(1);
  EXPECT_EQ(stack.Size(), 1);
  EXPECT_EQ(stack.Top(), 1);
}

TEST(StackTest, Pop) {
  Stack<int> stack{1, 2, 3};

  stack.Pop();
  EXPECT_EQ(stack.Size(), 2);
  EXPECT_EQ(stack.Top(), 2);
}

TEST(StackTest, Swap) {
  Stack<int> a{1, 2, 3};
  Stack<int> b{4, 5, 6};
  const Stack<int> expected_a{b};
  const Stack<int> expected_b{a};

  a.Swap(b);
  EXPECT_EQ(a, expected_a);
  EXPECT_EQ(b, expected_b);
}

// Comparison operators

TEST(StackTest, EqualOperator) {
  const Stack<int> a{1, 2, 3};
  const Stack<int> b{1, 2, 3};
  EXPECT_EQ(a, b);
}

TEST(StackTest, NotEqualOperator) {
  const Stack<int> a{1, 2, 3};
  const Stack<int> b{4, 5, 6};
  EXPECT_NE(a, b);
}

TEST(StackTest, LessOperator) {
  const Stack<int> a{1, 2, 3};
  const Stack<int> b{4, 5, 6};
  EXPECT_LT(a, b);
}

TEST(StackTest, LessEqualOperator) {
  Stack<int> a{1, 2, 3};
  Stack<int> b{4, 5, 6};
  EXPECT_LE(a, b);

  a = b = {1, 2, 3};
  EXPECT_LE(a, b);
}

TEST(StackTest, GreaterOperator) {
  const Stack<int> a{4, 5, 6};
  const Stack<int> b{1, 2, 3};
  EXPECT_GT(a, b);
}

TEST(StackTest, GreaterEqualOperator) {
  Stack<int> a{4, 5, 6};
  Stack<int> b{1, 2, 3};
  EXPECT_GE(a, b);

  a = b = {1, 2, 3};
  EXPECT_GE(a, b);
}
