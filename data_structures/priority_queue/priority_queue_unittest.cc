#include "priority_queue.h"

#include <gtest/gtest.h>

#include <functional>
#include <initializer_list>
#include <utility>

template <class T>
using MaxPriorityQueue = PriorityQueue<T>;

template <class T>
using MinPriorityQueue = PriorityQueue<T, std::greater<T>>;

// Constructors

TEST(PriorityQueueTest, Constructor) {
  const MaxPriorityQueue<int> priority_queue;
  EXPECT_EQ(priority_queue.Size(), 0);
}

TEST(PriorityQueueTest, CopyConstructor) {
  const MaxPriorityQueue<int> priority_queue{1, 2, 3};

  const MaxPriorityQueue<int> copy{priority_queue};
  EXPECT_EQ(copy.Size(), 3);
  EXPECT_EQ(copy.Top(), 3);
}

TEST(PriorityQueueTest, MoveConstructor) {
  MaxPriorityQueue<int> priority_queue{1, 2, 3};

  const MaxPriorityQueue<int> moved_priority_queue{std::move(priority_queue)};
  EXPECT_EQ(priority_queue.Size(), 0);
  EXPECT_EQ(moved_priority_queue.Size(), 3);
  EXPECT_EQ(moved_priority_queue.Top(), 3);
}

TEST(PriorityQueueTest, InitializerListConstructor) {
  const MaxPriorityQueue<int> max_priority_queue{1, 2, 3};
  EXPECT_EQ(max_priority_queue.Size(), 3);
  EXPECT_EQ(max_priority_queue.Top(), 3);

  const MinPriorityQueue<int> min_priority_queue{1, 2, 3};
  EXPECT_EQ(min_priority_queue.Size(), 3);
  EXPECT_EQ(min_priority_queue.Top(), 1);
}

// Assignments

TEST(PriorityQueueTest, CopyAssignment) {
  const MaxPriorityQueue<int> priority_queue{1, 2, 3};
  MaxPriorityQueue<int> copy;

  copy = priority_queue;
  EXPECT_EQ(copy.Size(), 3);
  EXPECT_EQ(copy.Top(), 3);
}

TEST(PriorityQueueTest, MoveAssignment) {
  MaxPriorityQueue<int> priority_queue{1, 2, 3};
  MaxPriorityQueue<int> moved_priority_queue;

  moved_priority_queue = std::move(priority_queue);
  EXPECT_EQ(priority_queue.Size(), 0);
  EXPECT_EQ(moved_priority_queue.Size(), 3);
  EXPECT_EQ(moved_priority_queue.Top(), 3);
}

TEST(PriorityQueueTest, InitializerListAssigment) {
  MaxPriorityQueue<int> max_priority_queue;
  max_priority_queue = {1, 2, 3};

  EXPECT_EQ(max_priority_queue.Size(), 3);
  EXPECT_EQ(max_priority_queue.Top(), 3);

  MinPriorityQueue<int> min_priority_queue;
  min_priority_queue = {1, 2, 3};

  EXPECT_EQ(min_priority_queue.Size(), 3);
  EXPECT_EQ(min_priority_queue.Top(), 1);
}

// Element access

TEST(PriorityQueueTest, Top) {
  MaxPriorityQueue<int> max_priority_queue{1, 2, 3};
  EXPECT_EQ(max_priority_queue.Top(), 3);

  max_priority_queue.Push(5);
  EXPECT_EQ(max_priority_queue.Top(), 5);

  MinPriorityQueue<int> min_priority_queue{4, 5, 6};
  EXPECT_EQ(min_priority_queue.Top(), 4);

  min_priority_queue.Push(1);
  EXPECT_EQ(min_priority_queue.Top(), 1);
}

// Capacity

TEST(PriorityQueueTest, Empty) {
  const MaxPriorityQueue<int> empty_priority_queue;
  EXPECT_TRUE(empty_priority_queue.Empty());

  const MaxPriorityQueue<int> priority_queue{1, 2, 3};
  EXPECT_FALSE(priority_queue.Empty());
}

TEST(PriorityQueueTest, Size) {
  MaxPriorityQueue<int> priority_queue;
  EXPECT_EQ(priority_queue.Size(), 0);

  priority_queue.Push(1);
  EXPECT_EQ(priority_queue.Size(), 1);
}

// Modifiers

TEST(PriorityQueueTest, Push) {
  MaxPriorityQueue<int> priority_queue;

  priority_queue.Push(1);
  EXPECT_EQ(priority_queue.Size(), 1);
  EXPECT_EQ(priority_queue.Top(), 1);

  priority_queue.Push(3);
  EXPECT_EQ(priority_queue.Size(), 2);
  EXPECT_EQ(priority_queue.Top(), 3);

  priority_queue.Push(2);
  EXPECT_EQ(priority_queue.Size(), 3);
  EXPECT_EQ(priority_queue.Top(), 3);
}

TEST(PriorityQueueTest, Pop) {
  MaxPriorityQueue<int> max_priority_queue{1, 2, 3};

  max_priority_queue.Pop();
  EXPECT_EQ(max_priority_queue.Top(), 2);

  max_priority_queue.Pop();
  EXPECT_EQ(max_priority_queue.Top(), 1);

  max_priority_queue.Pop();
  EXPECT_TRUE(max_priority_queue.Empty());

  MinPriorityQueue<int> min_priority_queue{1, 2, 3};

  min_priority_queue.Pop();
  EXPECT_EQ(min_priority_queue.Top(), 2);

  min_priority_queue.Pop();
  EXPECT_EQ(min_priority_queue.Top(), 3);

  min_priority_queue.Pop();
  EXPECT_TRUE(min_priority_queue.Empty());
}

TEST(PriorityQueueTest, Swap) {
  MaxPriorityQueue<int> a{1};
  MaxPriorityQueue<int> b{2};

  a.Swap(b);
  EXPECT_EQ(a.Top(), 2);
  EXPECT_EQ(b.Top(), 1);
}