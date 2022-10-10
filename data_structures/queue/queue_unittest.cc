#include "queue.h"

#include <gtest/gtest.h>

#include <utility>

// Constructors

TEST(QueueTest, Constructor) {
  const Queue<int> queue;
  EXPECT_EQ(queue.Size(), 0);
}

TEST(QueueTest, CopyConstructor) {
  const Queue<int> queue{1, 2, 3};

  const Queue<int> copy{queue};
  EXPECT_EQ(copy, queue);
}

TEST(QueueTest, MoveConstructor) {
  Queue<int> queue{1, 2, 3};

  const Queue<int> moved_queue{std::move(queue)};
  EXPECT_EQ(moved_queue.Size(), 3);
  EXPECT_EQ(queue.Size(), 0);
}

TEST(QueueTest, InitializerListConstructor) {
  Queue<int> queue{1, 2, 3};
  EXPECT_EQ(queue.Size(), 3);
  EXPECT_EQ(queue.Front(), 1);

  queue.Pop();
  EXPECT_EQ(queue.Front(), 2);

  queue.Pop();
  EXPECT_EQ(queue.Front(), 3);

  queue.Pop();
  EXPECT_TRUE(queue.Empty());
}

// Assignments

TEST(QueueTest, CopyAssignment) {
  const Queue<int> queue{1, 2, 3};
  Queue<int> copy;

  copy = queue;
  EXPECT_EQ(copy, queue);
}

TEST(QueueTest, MoveAssignment) {
  Queue<int> queue{1, 2, 3};
  Queue<int> moved_queue;

  moved_queue = std::move(queue);
  EXPECT_EQ(moved_queue.Size(), 3);
  EXPECT_EQ(queue.Size(), 0);
}

TEST(QueueTest, InitializerListAssignment) {
  Queue<int> queue;

  queue = {1, 2, 3};
  EXPECT_EQ(queue.Size(), 3);
  EXPECT_EQ(queue.Front(), 1);

  queue.Pop();
  EXPECT_EQ(queue.Front(), 2);

  queue.Pop();
  EXPECT_EQ(queue.Front(), 3);

  queue.Pop();
  EXPECT_TRUE(queue.Empty());
}

// Element access

TEST(QueueTest, Front) {
  Queue<int> queue{1, 2, 3};
  EXPECT_EQ(queue.Front(), 1);

  queue.Front() = 5;
  EXPECT_EQ(queue.Front(), 5);
}

TEST(QueueTest, Front_Const) {
  const Queue<int> queue{1, 2, 3};
  EXPECT_EQ(queue.Front(), 1);
}

TEST(QueueTest, Back) {
  Queue<int> queue{1, 2, 3};
  EXPECT_EQ(queue.Back(), 3);

  queue.Back() = 5;
  EXPECT_EQ(queue.Back(), 5);
}

TEST(QueueTest, Back_Const) {
  const Queue<int> queue{1, 2, 3};
  EXPECT_EQ(queue.Back(), 3);
}

// Iterators

TEST(QueueTest, Begin) {
  Queue<int> queue;
  EXPECT_EQ(queue.begin(), queue.end());

  queue = {1, 2};
  auto it{queue.begin()};
  EXPECT_EQ(*it, 1);
  EXPECT_NE(it, queue.end());

  *it = 10;
  EXPECT_EQ(queue.Front(), 10);

  ++it;
  EXPECT_EQ(*it, 2);
  EXPECT_NE(it, queue.end());

  ++it;
  EXPECT_EQ(it, queue.end());
}

TEST(QueueTest, Begin_Const) {
  const Queue<int> queue{1, 2};

  auto it{queue.begin()};
  EXPECT_EQ(*it, 1);
  EXPECT_NE(it, queue.end());

  ++it;
  EXPECT_EQ(*it, 2);
  EXPECT_NE(it, queue.end());

  ++it;
  EXPECT_EQ(it, queue.end());
}

TEST(QueueTest, Cbegin) {
  const Queue<int> queue{1, 2};

  auto it{queue.cbegin()};
  EXPECT_EQ(*it, 1);
  EXPECT_NE(it, queue.cend());

  ++it;
  EXPECT_EQ(*it, 2);
  EXPECT_NE(it, queue.cend());

  ++it;
  EXPECT_EQ(it, queue.cend());
}

TEST(QueueTest, End) {
  Queue<int> queue{1};
  EXPECT_NE(queue.end(), queue.begin());
  EXPECT_EQ(queue.end(), ++queue.begin());
}

TEST(QueueTest, End_Const) {
  const Queue<int> queue{1};
  EXPECT_NE(queue.end(), queue.begin());
  EXPECT_EQ(queue.end(), ++queue.begin());
}

TEST(QueueTest, Cend) {
  const Queue<int> queue{1};
  EXPECT_NE(queue.cend(), queue.cbegin());
  EXPECT_EQ(queue.cend(), ++queue.cbegin());
}

TEST(QueueTest, Rbegin) {
  Queue<int> queue;
  EXPECT_EQ(queue.rbegin(), queue.rend());

  queue = {1, 2};
  auto it{queue.rbegin()};
  EXPECT_EQ(*it, 2);
  EXPECT_NE(it, queue.rend());

  *it = 10;
  EXPECT_EQ(queue.Back(), 10);

  ++it;
  EXPECT_EQ(*it, 1);
  EXPECT_NE(it, queue.rend());

  ++it;
  EXPECT_EQ(it, queue.rend());
}

TEST(QueueTest, Rbegin_Const) {
  const Queue<int> queue{1, 2};

  auto it{queue.rbegin()};
  EXPECT_EQ(*it, 2);
  EXPECT_NE(it, queue.rend());

  ++it;
  EXPECT_EQ(*it, 1);
  EXPECT_NE(it, queue.rend());

  ++it;
  EXPECT_EQ(it, queue.rend());
}

TEST(QueueTest, Crbegin) {
  const Queue<int> queue{1, 2};

  auto it{queue.crbegin()};
  EXPECT_EQ(*it, 2);
  EXPECT_NE(it, queue.crend());

  ++it;
  EXPECT_EQ(*it, 1);
  EXPECT_NE(it, queue.crend());

  ++it;
  EXPECT_EQ(it, queue.crend());
}

TEST(QueueTest, Rend) {
  Queue<int> queue{1};
  EXPECT_NE(queue.rend(), queue.rbegin());
  EXPECT_EQ(queue.rend(), ++queue.rbegin());
}

TEST(QueueTest, Rend_Const) {
  const Queue<int> queue{1};
  EXPECT_NE(queue.rend(), queue.rbegin());
  EXPECT_EQ(queue.rend(), ++queue.rbegin());
}

TEST(QueueTest, Crend) {
  const Queue<int> queue{1};
  EXPECT_NE(queue.crend(), queue.crbegin());
  EXPECT_EQ(queue.crend(), ++queue.crbegin());
}

// Capacity

TEST(QueueTest, Empty) {
  const Queue<int> empty_queue;
  EXPECT_TRUE(empty_queue.Empty());

  const Queue<int> queue{1, 2, 3};
  EXPECT_FALSE(queue.Empty());
}

TEST(QueueTest, Size) {
  Queue<int> queue;
  EXPECT_EQ(queue.Size(), 0);

  queue.Push(1);
  EXPECT_EQ(queue.Size(), 1);
}

// Modifiers

TEST(QueueTest, Push) {
  Queue<int> queue;

  queue.Push(1);
  EXPECT_EQ(queue.Size(), 1);
  EXPECT_EQ(queue.Front(), 1);
  EXPECT_EQ(queue.Back(), 1);
}

TEST(QueueTest, Pop) {
  Queue<int> queue{1, 2, 3};

  queue.Pop();
  EXPECT_EQ(queue.Size(), 2);
  EXPECT_EQ(queue.Front(), 2);
}

TEST(QueueTest, Swap) {
  Queue<int> a{1, 2, 3};
  Queue<int> b{4, 5, 6};
  const Queue<int> expected_a{b};
  const Queue<int> expected_b{a};

  a.Swap(b);
  EXPECT_EQ(a, expected_a);
  EXPECT_EQ(b, expected_b);
}

// Comparison operators

TEST(QueueTest, EqualOperator) {
  const Queue<int> a{1, 2, 3};
  const Queue<int> b{1, 2, 3};
  EXPECT_EQ(a, b);
}

TEST(QueueTest, NotEqualOperator) {
  const Queue<int> a{1, 2, 3};
  const Queue<int> b{4, 5, 6};
  EXPECT_NE(a, b);
}

TEST(QueueTest, LessOperator) {
  const Queue<int> a{1, 2, 3};
  const Queue<int> b{4, 5, 6};
  EXPECT_LT(a, b);
}

TEST(QueueTest, LessEqualOperator) {
  Queue<int> a{1, 2, 3};
  Queue<int> b{4, 5, 6};
  EXPECT_LE(a, b);

  a = b = {1, 2, 3};
  EXPECT_LE(a, b);
}

TEST(QueueTest, GreaterOperator) {
  const Queue<int> a{4, 5, 6};
  const Queue<int> b{1, 2, 3};
  EXPECT_GT(a, b);
}

TEST(QueueTest, GreaterEqualOperator) {
  Queue<int> a{4, 5, 6};
  Queue<int> b{1, 2, 3};
  EXPECT_GE(a, b);

  a = b = {1, 2, 3};
  EXPECT_GE(a, b);
}
