#include "singly_linked_list.h"

#include <gtest/gtest.h>

#include <cstddef>
#include <iterator>
#include <utility>

template <class T>
typename SinglyLinkedList<T>::const_iterator At(const SinglyLinkedList<T>& list,
                                                std::size_t index) {
  auto it{list.cbegin()};
  std::advance(it, index);
  return it;
}

// Constructors

TEST(SinglyLinkedListTest, DefaultConstructor) {
  const SinglyLinkedList<int> list;
  EXPECT_EQ(list.Size(), 0);
}

TEST(SinglyLinkedListTest, CopyConstructor) {
  const SinglyLinkedList<int> list{1, 2, 3};

  const SinglyLinkedList<int> list_copy(list);
  EXPECT_EQ(list, list_copy);
}

TEST(SinglyLinkedListTest, MoveConstructor) {
  SinglyLinkedList<int> list{1, 2, 3};

  const SinglyLinkedList<int> moved_list(std::move(list));
  EXPECT_EQ(list.Size(), 0);
  EXPECT_EQ(moved_list.Size(), 3);
}

TEST(SinglyLinkedListTest, InitializerListConstructor) {
  const SinglyLinkedList<int> list{1, 2, 3};
  EXPECT_EQ(list.Size(), 3);
}

// Assignments

TEST(SinglyLinkedListTest, CopyAssignment) {
  const SinglyLinkedList<int> list{1, 2, 3};

  SinglyLinkedList<int> list_copy = list;
  EXPECT_EQ(list, list_copy);
}

TEST(SinglyLinkedListTest, MoveAssignment) {
  SinglyLinkedList<int> list{1, 2, 3};

  SinglyLinkedList<int> moved_list{1, 2, 3};
  moved_list = std::move(list);

  EXPECT_EQ(list.Size(), 0);
  EXPECT_EQ(moved_list.Size(), 3);
}

TEST(SinglyLinkedListTest, InitializerListAssignment) {
  SinglyLinkedList<int> list;
  list = {1, 2, 3};

  EXPECT_EQ(list.Size(), 3);
}

// Element access

TEST(SinglyLinkedListTest, Front) {
  SinglyLinkedList<int> list{1, 2, 3};
  EXPECT_EQ(list.Front(), 1);

  list.Front() = 5;
  EXPECT_EQ(list.Front(), 5);
}

TEST(SinglyLinkedListTest, Front_Const) {
  const SinglyLinkedList<int> list{1, 2, 3};
  EXPECT_EQ(list.Front(), 1);
}

// Iterators

TEST(SinglyLinkedListTest, BeforeBegin) {
  SinglyLinkedList<int> list{1};

  auto it{list.before_begin()};
  EXPECT_NE(it, list.begin());

  ++it;
  EXPECT_EQ(*it, 1);
  EXPECT_EQ(it, list.begin());

  *it = 10;
  EXPECT_EQ(list.Front(), 10);

  ++it;
  EXPECT_EQ(it, list.end());
}

TEST(SinglyLinkedListTest, BeforeBegin_Const) {
  const SinglyLinkedList<int> list{1};

  auto it{list.before_begin()};
  EXPECT_NE(it, list.begin());

  ++it;
  EXPECT_EQ(*it, 1);
  EXPECT_EQ(it, list.begin());

  ++it;
  EXPECT_EQ(it, list.end());
}

TEST(SinglyLinkedListTest, CBeforeBegin) {
  const SinglyLinkedList<int> list{1};

  auto it{list.cbefore_begin()};
  EXPECT_NE(it, list.cbegin());

  ++it;
  EXPECT_EQ(*it, 1);
  EXPECT_EQ(it, list.cbegin());

  ++it;
  EXPECT_EQ(it, list.cend());
}

TEST(SinglyLinkedListTest, Begin) {
  SinglyLinkedList<int> list;
  EXPECT_EQ(list.begin(), list.end());

  list = {1, 2};
  auto it{list.begin()};
  EXPECT_EQ(*it, 1);
  EXPECT_NE(it, list.end());

  *it = 10;
  EXPECT_EQ(list.Front(), 10);

  ++it;
  EXPECT_EQ(*it, 2);
  EXPECT_NE(it, list.end());

  ++it;
  EXPECT_EQ(it, list.end());
}

TEST(SinglyLinkedListTest, Begin_Const) {
  const SinglyLinkedList<int> list{1, 2};

  auto it{list.begin()};
  EXPECT_EQ(*it, 1);
  EXPECT_NE(it, list.end());

  ++it;
  EXPECT_EQ(*it, 2);
  EXPECT_NE(it, list.end());

  ++it;
  EXPECT_EQ(it, list.end());
}

TEST(SinglyLinkedListTest, Cbegin) {
  const SinglyLinkedList<int> list{1, 2};

  auto it{list.cbegin()};
  EXPECT_EQ(*it, 1);
  EXPECT_NE(it, list.cend());

  ++it;
  EXPECT_EQ(*it, 2);
  EXPECT_NE(it, list.cend());

  ++it;
  EXPECT_EQ(it, list.cend());
}

TEST(SinglyLinkedListTest, End) {
  SinglyLinkedList<int> list{1};
  EXPECT_NE(list.end(), list.begin());
  EXPECT_EQ(list.end(), ++list.begin());
}

TEST(SinglyLinkedListTest, End_Const) {
  const SinglyLinkedList<int> list{1};
  EXPECT_NE(list.end(), list.begin());
  EXPECT_EQ(list.end(), ++list.begin());
}

TEST(SinglyLinkedListTest, Cend) {
  const SinglyLinkedList<int> list{1};
  EXPECT_NE(list.cend(), list.cbegin());
  EXPECT_EQ(list.cend(), ++list.cbegin());
}

// Capacity

TEST(SinglyLinkedListTest, Empty) {
  SinglyLinkedList<int> list;
  EXPECT_TRUE(list.Empty());

  list.PushFront(0);
  EXPECT_FALSE(list.Empty());
}

TEST(SinglyLinkedListTest, Size) {
  SinglyLinkedList<int> list;
  EXPECT_EQ(list.Size(), 0);

  list.PushFront(0);
  EXPECT_EQ(list.Size(), 1);
}

// Modifiers

TEST(SinglyLinkedListTest, Clear) {
  SinglyLinkedList<int> list{1};
  EXPECT_FALSE(list.Empty());

  list.Clear();
  EXPECT_TRUE(list.Empty());
}

TEST(SinglyLinkedListTest, InsertAfter_Value) {
  SinglyLinkedList<int> list;
  auto inserted{list.end()};

  inserted = list.InsertAfter(list.cbefore_begin(), 1);
  EXPECT_EQ(*inserted, 1);
  EXPECT_EQ(list, (SinglyLinkedList{1}));

  inserted = list.InsertAfter(list.cbefore_begin(), 2);
  EXPECT_EQ(*inserted, 2);
  EXPECT_EQ(list, (SinglyLinkedList{2, 1}));

  inserted = list.InsertAfter(At(list, 1), 3);
  EXPECT_EQ(*inserted, 3);
  EXPECT_EQ(list, (SinglyLinkedList{2, 1, 3}));

  inserted = list.InsertAfter(At(list, 0), 4);
  EXPECT_EQ(*inserted, 4);
  EXPECT_EQ(list, (SinglyLinkedList{2, 4, 1, 3}));

  inserted = list.InsertAfter(list.cbefore_begin(), 5);
  EXPECT_EQ(*inserted, 5);
  EXPECT_EQ(list, (SinglyLinkedList{5, 2, 4, 1, 3}));

  inserted = list.InsertAfter(At(list, 2), 6);
  EXPECT_EQ(*inserted, 6);
  EXPECT_EQ(list, (SinglyLinkedList{5, 2, 4, 6, 1, 3}));

  inserted = list.InsertAfter(At(list, 5), 7);
  EXPECT_EQ(*inserted, 7);
  EXPECT_EQ(list, (SinglyLinkedList{5, 2, 4, 6, 1, 3, 7}));
}

TEST(SinglyLinkedListTest, InsertAfter_ValueCount) {
  SinglyLinkedList<int> list;
  auto inserted{list.end()};

  inserted = list.InsertAfter(list.cbefore_begin(), 1, 1);
  EXPECT_EQ(*inserted, 1);
  EXPECT_EQ(list, (SinglyLinkedList{1}));

  inserted = list.InsertAfter(list.cbefore_begin(), 2, 2);
  EXPECT_EQ(*inserted, 2);
  EXPECT_EQ(list, (SinglyLinkedList{2, 2, 1}));

  inserted = list.InsertAfter(At(list, 2), 3, 3);
  EXPECT_EQ(*inserted, 3);
  EXPECT_EQ(list, (SinglyLinkedList{2, 2, 1, 3, 3, 3}));

  inserted = list.InsertAfter(At(list, 1), 4, 4);
  EXPECT_EQ(*inserted, 4);
  EXPECT_EQ(list, (SinglyLinkedList{2, 2, 4, 4, 4, 4, 1, 3, 3, 3}));

  inserted = list.InsertAfter(list.cbefore_begin(), 5, 5);
  EXPECT_EQ(*inserted, 5);
  EXPECT_EQ(list,
            (SinglyLinkedList{5, 5, 5, 5, 5, 2, 2, 4, 4, 4, 4, 1, 3, 3, 3}));

  inserted = list.InsertAfter(At(list, 10), 6, 6);
  EXPECT_EQ(*inserted, 6);
  EXPECT_EQ(list, (SinglyLinkedList{5, 5, 5, 5, 5, 2, 2, 4, 4, 4, 4,
                                    6, 6, 6, 6, 6, 6, 1, 3, 3, 3}));

  inserted = list.InsertAfter(At(list, 20), 7, 7);
  EXPECT_EQ(*inserted, 7);
  EXPECT_EQ(list, (SinglyLinkedList{5, 5, 5, 5, 5, 2, 2, 4, 4, 4, 4, 6, 6, 6,
                                    6, 6, 6, 1, 3, 3, 3, 7, 7, 7, 7, 7, 7, 7}));
}

TEST(SinglyLinkedListTest, InsertAfter_Range) {
  const SinglyLinkedList<int> source{1, 11, 111, 2,  22, 33, 3,  44, 4,
                                     5, 55, 666, 66, 6,  7,  77, 777};
  SinglyLinkedList<int> list;
  auto inserted{list.end()};

  inserted =
      list.InsertAfter(list.cbefore_begin(), At(source, 0), At(source, 3));
  EXPECT_EQ(*inserted, 111);
  EXPECT_EQ(list, (SinglyLinkedList{1, 11, 111}));

  inserted =
      list.InsertAfter(list.cbefore_begin(), At(source, 3), At(source, 5));
  EXPECT_EQ(*inserted, 22);
  EXPECT_EQ(list, (SinglyLinkedList{2, 22, 1, 11, 111}));

  inserted = list.InsertAfter(At(list, 4), At(source, 5), At(source, 7));
  EXPECT_EQ(*inserted, 3);
  EXPECT_EQ(list, (SinglyLinkedList{2, 22, 1, 11, 111, 33, 3}));

  inserted = list.InsertAfter(At(list, 1), At(source, 7), At(source, 9));
  EXPECT_EQ(*inserted, 4);
  EXPECT_EQ(list, (SinglyLinkedList{2, 22, 44, 4, 1, 11, 111, 33, 3}));

  inserted = list.InsertAfter(At(list, 6), At(source, 9), At(source, 11));
  EXPECT_EQ(*inserted, 55);
  EXPECT_EQ(list, (SinglyLinkedList{2, 22, 44, 4, 1, 11, 111, 5, 55, 33, 3}));

  inserted =
      list.InsertAfter(list.cbefore_begin(), At(source, 11), At(source, 14));
  EXPECT_EQ(*inserted, 6);
  EXPECT_EQ(list, (SinglyLinkedList{666, 66, 6, 2, 22, 44, 4, 1, 11, 111, 5, 55,
                                    33, 3}));

  inserted = list.InsertAfter(At(list, 13), At(source, 14), At(source, 17));
  EXPECT_EQ(*inserted, 777);
  EXPECT_EQ(list, (SinglyLinkedList{666, 66, 6, 2, 22, 44, 4, 1, 11, 111, 5, 55,
                                    33, 3, 7, 77, 777}));
}

TEST(SinglyLinkedListTest, InsertAfter_InitializerList) {
  SinglyLinkedList<int> list;
  auto inserted{list.end()};

  inserted = list.InsertAfter(list.cbefore_begin(), {1, 11, 111});
  EXPECT_EQ(*inserted, 111);
  EXPECT_EQ(list, (SinglyLinkedList{1, 11, 111}));

  inserted = list.InsertAfter(list.cbefore_begin(), {2, 22});
  EXPECT_EQ(*inserted, 22);
  EXPECT_EQ(list, (SinglyLinkedList{2, 22, 1, 11, 111}));

  inserted = list.InsertAfter(At(list, 4), {33, 3});
  EXPECT_EQ(*inserted, 3);
  EXPECT_EQ(list, (SinglyLinkedList{2, 22, 1, 11, 111, 33, 3}));

  inserted = list.InsertAfter(At(list, 1), {44, 4});
  EXPECT_EQ(*inserted, 4);
  EXPECT_EQ(list, (SinglyLinkedList{2, 22, 44, 4, 1, 11, 111, 33, 3}));

  inserted = list.InsertAfter(At(list, 6), {5, 55});
  EXPECT_EQ(*inserted, 55);
  EXPECT_EQ(list, (SinglyLinkedList{2, 22, 44, 4, 1, 11, 111, 5, 55, 33, 3}));

  inserted = list.InsertAfter(list.cbefore_begin(), {666, 66, 6});
  EXPECT_EQ(*inserted, 6);
  EXPECT_EQ(list, (SinglyLinkedList{666, 66, 6, 2, 22, 44, 4, 1, 11, 111, 5, 55,
                                    33, 3}));

  inserted = list.InsertAfter(At(list, 13), {7, 77, 777});
  EXPECT_EQ(*inserted, 777);
  EXPECT_EQ(list, (SinglyLinkedList{666, 66, 6, 2, 22, 44, 4, 1, 11, 111, 5, 55,
                                    33, 3, 7, 77, 777}));
}

TEST(SinglyLinkedListTest, EraseAfter_Element) {
  SinglyLinkedList<int> list{0, 1, 2, 3, 4, 5};
  auto next{list.end()};

  next = list.EraseAfter(list.cbefore_begin());
  EXPECT_EQ(*next, 1);
  EXPECT_EQ(list, (SinglyLinkedList{1, 2, 3, 4, 5}));

  next = list.EraseAfter(At(list, 0));
  EXPECT_EQ(*next, 3);
  EXPECT_EQ(list, (SinglyLinkedList{1, 3, 4, 5}));

  next = list.EraseAfter(At(list, 1));
  EXPECT_EQ(*next, 5);
  EXPECT_EQ(list, (SinglyLinkedList{1, 3, 5}));

  next = list.EraseAfter(At(list, 1));
  EXPECT_EQ(next, list.end());
  EXPECT_EQ(list, (SinglyLinkedList{1, 3}));

  next = list.EraseAfter(At(list, 0));
  EXPECT_EQ(next, list.end());
  EXPECT_EQ(list, (SinglyLinkedList{1}));

  next = list.EraseAfter(list.cbefore_begin());
  EXPECT_EQ(next, list.end());
  EXPECT_TRUE(list.Empty());
}

TEST(SinglyLinkedListTest, EraseAfter_Range) {
  SinglyLinkedList<int> list{0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
  auto next{list.end()};

  next = list.EraseAfter(list.cbefore_begin(), At(list, 3));
  EXPECT_EQ(*next, 3);
  EXPECT_EQ(list, (SinglyLinkedList{3, 4, 5, 6, 7, 8, 9}));

  next = list.EraseAfter(At(list, 1), At(list, 4));
  EXPECT_EQ(*next, 7);
  EXPECT_EQ(list, (SinglyLinkedList{3, 4, 7, 8, 9}));

  next = list.EraseAfter(At(list, 0), At(list, 4));
  EXPECT_EQ(*next, 9);
  EXPECT_EQ(list, (SinglyLinkedList{3, 9}));

  next = list.EraseAfter(list.cbefore_begin(), list.cend());
  EXPECT_EQ(next, list.end());
  EXPECT_TRUE(list.Empty());
}

TEST(SinglyLinkedListTest, PushFront) {
  SinglyLinkedList<int> list;

  list.PushFront(1);
  EXPECT_EQ(list.Front(), 1);
  EXPECT_EQ(list.Size(), 1);

  list.PushFront(2);
  EXPECT_EQ(list.Front(), 2);
  EXPECT_EQ(list.Size(), 2);
}

TEST(SinglyLinkedListTest, PopFront) {
  SinglyLinkedList<int> list{1, 2};
  EXPECT_EQ(list.Front(), 1);

  list.PopFront();
  EXPECT_EQ(list.Front(), 2);

  list.PopFront();
  EXPECT_TRUE(list.Empty());
}

TEST(SinglyLinkedListTest, Resize) {
  SinglyLinkedList<int> list;

  list.Resize(2);
  EXPECT_EQ(list, (SinglyLinkedList{0, 0}));

  list.Resize(5, 1);
  EXPECT_EQ(list, (SinglyLinkedList{0, 0, 1, 1, 1}));

  list.Resize(3);
  EXPECT_EQ(list, (SinglyLinkedList{0, 0, 1}));
}

TEST(SinglyLinkedListTest, Swap) {
  SinglyLinkedList<int> a{1};
  SinglyLinkedList<int> b{2, 3};

  a.Swap(b);
  EXPECT_EQ(a.Size(), 2);
  EXPECT_EQ(b.Size(), 1);
}

// Operations

TEST(SinglyLinkedListTest, SpliceAfter_List) {
  SinglyLinkedList<int> list;

  SinglyLinkedList<int> a{};
  list.SpliceAfter(list.cbefore_begin(), a);

  EXPECT_EQ(list, (SinglyLinkedList<int>{}));
  EXPECT_TRUE(a.Empty());

  SinglyLinkedList<int> b{1};
  list.SpliceAfter(list.cbefore_begin(), b);

  EXPECT_EQ(list, (SinglyLinkedList<int>{1}));
  EXPECT_TRUE(b.Empty());

  SinglyLinkedList<int> c{3, 2};
  list.SpliceAfter(list.cbefore_begin(), c);

  EXPECT_EQ(list, (SinglyLinkedList<int>{3, 2, 1}));
  EXPECT_TRUE(c.Empty());

  SinglyLinkedList<int> d{5};
  list.SpliceAfter(At(list, 2), d);

  EXPECT_EQ(list, (SinglyLinkedList<int>{3, 2, 1, 5}));
  EXPECT_TRUE(d.Empty());

  SinglyLinkedList<int> e{4};
  list.SpliceAfter(At(list, 2), e);

  EXPECT_EQ(list, (SinglyLinkedList<int>{3, 2, 1, 4, 5}));
  EXPECT_TRUE(e.Empty());
}

TEST(SinglyLinkedListTest, SpliceAfter_Element) {
  SinglyLinkedList<int> list;

  SinglyLinkedList<int> a{1};
  list.SpliceAfter(list.cbefore_begin(), a, a.cbefore_begin());

  EXPECT_EQ(list, (SinglyLinkedList{1}));
  EXPECT_TRUE(a.Empty());

  SinglyLinkedList<int> b{2, 1};
  list.SpliceAfter(list.cbefore_begin(), b, b.cbefore_begin());

  EXPECT_EQ(list, (SinglyLinkedList{2, 1}));
  EXPECT_EQ(b, (SinglyLinkedList{1}));

  SinglyLinkedList<int> c{1, 2, 3};
  list.SpliceAfter(At(list, 1), c, At(c, 1));

  EXPECT_EQ(list, (SinglyLinkedList{2, 1, 3}));
  EXPECT_EQ(c, (SinglyLinkedList{1, 2}));

  SinglyLinkedList<int> d{1, 4, 2};
  list.SpliceAfter(At(list, 0), d, At(d, 0));

  EXPECT_EQ(list, (SinglyLinkedList{2, 4, 1, 3}));
  EXPECT_EQ(d, (SinglyLinkedList{1, 2}));
}

TEST(SinglyLinkedListTest, SpliceAfter_Range) {
  SinglyLinkedList<int> list;

  SinglyLinkedList<int> a{};
  list.SpliceAfter(list.cbefore_begin(), a, a.cbefore_begin(), At(a, 0));

  EXPECT_EQ(list, (SinglyLinkedList<int>{}));
  EXPECT_TRUE(a.Empty());

  SinglyLinkedList<int> b{1, 2};
  list.SpliceAfter(list.cbefore_begin(), b, b.cbefore_begin(), At(b, 1));

  EXPECT_EQ(list, (SinglyLinkedList{1}));
  EXPECT_EQ(b, (SinglyLinkedList{2}));

  SinglyLinkedList<int> c{2, 3, 4};
  list.SpliceAfter(list.cbefore_begin(), c, c.cbefore_begin(), c.cend());

  EXPECT_EQ(list, (SinglyLinkedList{2, 3, 4, 1}));
  EXPECT_TRUE(c.Empty());

  SinglyLinkedList<int> d{1, 5, 6, 2};
  list.SpliceAfter(At(list, 3), d, At(d, 0), At(d, 3));

  EXPECT_EQ(list, (SinglyLinkedList{2, 3, 4, 1, 5, 6}));
  EXPECT_EQ(d, (SinglyLinkedList{1, 2}));

  SinglyLinkedList<int> e{3, 7, 8};
  list.SpliceAfter(At(list, 5), e, At(e, 0), e.cend());

  EXPECT_EQ(list, (SinglyLinkedList{2, 3, 4, 1, 5, 6, 7, 8}));
  EXPECT_EQ(e, (SinglyLinkedList{3}));
}

TEST(SinglyLinkedListTest, Reverse) {
  SinglyLinkedList<int> list{1, 2, 3};

  list.Reverse();
  EXPECT_EQ(list, (SinglyLinkedList{3, 2, 1}));
}

// Comparison operators

TEST(SinglyLinkedListTest, EqualOperator) {
  const SinglyLinkedList<int> a{1, 2, 3};
  const SinglyLinkedList<int> b{1, 2, 3};
  EXPECT_EQ(a, b);
}

TEST(SinglyLinkedListTest, NotEqualOperator) {
  const SinglyLinkedList<int> a{1, 2, 3};
  const SinglyLinkedList<int> b{4, 5, 6};
  EXPECT_NE(a, b);
}

TEST(SinglyLinkedListTest, LessOperator) {
  SinglyLinkedList<int> a{4, 5};
  SinglyLinkedList<int> b{1, 2, 3};
  EXPECT_LT(a, b);

  a = {1, 2, 3};
  b = {4, 5, 6};
  EXPECT_LT(a, b);
}

TEST(SinglyLinkedListTest, LessEqualOperator) {
  SinglyLinkedList<int> a{1, 2};
  SinglyLinkedList<int> b{4, 5, 6};
  EXPECT_LE(a, b);

  a = {1, 2, 3};
  b = {4, 5, 6};
  EXPECT_LE(a, b);

  a = b = {1, 2, 3};
  EXPECT_LE(a, b);
}

TEST(SinglyLinkedListTest, GreaterOperator) {
  SinglyLinkedList<int> a{1, 2, 3};
  SinglyLinkedList<int> b{4, 5};
  EXPECT_GT(a, b);

  a = {4, 5, 6};
  b = {1, 2, 3};
  EXPECT_GT(a, b);
}

TEST(SinglyLinkedListTest, GreaterEqualOperator) {
  SinglyLinkedList<int> a{1, 2, 3};
  SinglyLinkedList<int> b{4, 5};
  EXPECT_GE(a, b);

  a = {4, 5, 6};
  b = {1, 2, 3};
  EXPECT_GE(a, b);

  a = b = {1, 2, 3};
  EXPECT_GE(a, b);
}