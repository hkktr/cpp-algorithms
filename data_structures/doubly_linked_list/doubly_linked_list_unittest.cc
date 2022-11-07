#include "doubly_linked_list.h"

#include <gtest/gtest.h>

#include <cstddef>
#include <iterator>
#include <utility>

template <class T>
typename DoublyLinkedList<T>::const_iterator At(const DoublyLinkedList<T>& list,
                                                std::size_t index) {
  auto it{list.cbegin()};
  std::advance(it, index);
  return it;
}

// Constructors

TEST(DoublyLinkedListTest, DefaultConstructor) {
  const DoublyLinkedList<int> list;
  EXPECT_EQ(list.Size(), 0);
}

TEST(DoublyLinkedListTest, CopyConstructor) {
  const DoublyLinkedList<int> list{1, 2, 3};

  const DoublyLinkedList<int> list_copy(list);
  EXPECT_EQ(list, list_copy);
}

TEST(DoublyLinkedListTest, MoveConstructor) {
  DoublyLinkedList<int> list{1, 2, 3};

  const DoublyLinkedList<int> moved_list(std::move(list));
  EXPECT_EQ(list.Size(), 0);
  EXPECT_EQ(moved_list.Size(), 3);
}

TEST(DoublyLinkedListTest, InitializerListConstructor) {
  const DoublyLinkedList<int> list{1, 2, 3};
  EXPECT_EQ(list.Size(), 3);
}

// Assignments

TEST(DoublyLinkedListTest, CopyAssignment) {
  const DoublyLinkedList<int> list{1, 2, 3};

  DoublyLinkedList<int> list_copy = list;
  EXPECT_EQ(list, list_copy);
}

TEST(DoublyLinkedListTest, MoveAssignment) {
  DoublyLinkedList<int> list{1, 2, 3};

  DoublyLinkedList<int> moved_list{1, 2, 3};
  moved_list = std::move(list);

  EXPECT_EQ(list.Size(), 0);
  EXPECT_EQ(moved_list.Size(), 3);
}

TEST(DoublyLinkedListTest, InitializerListAssignment) {
  DoublyLinkedList<int> list;
  list = {1, 2, 3};

  EXPECT_EQ(list.Size(), 3);
}

// Element access

TEST(DoublyLinkedListTest, Front) {
  DoublyLinkedList<int> list{1, 2, 3};
  EXPECT_EQ(list.Front(), 1);

  list.Front() = 5;
  EXPECT_EQ(list.Front(), 5);
}

TEST(DoublyLinkedListTest, Front_Const) {
  const DoublyLinkedList<int> list{1, 2, 3};
  EXPECT_EQ(list.Front(), 1);
}

TEST(DoublyLinkedListTest, Back) {
  DoublyLinkedList<int> list{1, 2, 3};
  EXPECT_EQ(list.Back(), 3);

  list.Back() = 5;
  EXPECT_EQ(list.Back(), 5);
}

TEST(DoublyLinkedListTest, Back_Const) {
  const DoublyLinkedList<int> list{1, 2, 3};
  EXPECT_EQ(list.Back(), 3);
}

// Iterators

TEST(DoublyLinkedListTest, Begin) {
  DoublyLinkedList<int> list;
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

TEST(DoublyLinkedListTest, Begin_Const) {
  const DoublyLinkedList<int> list{1, 2};

  auto it{list.begin()};
  EXPECT_EQ(*it, 1);
  EXPECT_NE(it, list.end());

  ++it;
  EXPECT_EQ(*it, 2);
  EXPECT_NE(it, list.end());

  ++it;
  EXPECT_EQ(it, list.end());
}

TEST(DoublyLinkedListTest, Cbegin) {
  const DoublyLinkedList<int> list{1, 2};

  auto it{list.cbegin()};
  EXPECT_EQ(*it, 1);
  EXPECT_NE(it, list.cend());

  ++it;
  EXPECT_EQ(*it, 2);
  EXPECT_NE(it, list.cend());

  ++it;
  EXPECT_EQ(it, list.cend());
}

TEST(DoublyLinkedListTest, End) {
  DoublyLinkedList<int> list{1};
  EXPECT_NE(list.end(), list.begin());
  EXPECT_EQ(list.end(), ++list.begin());
}

TEST(DoublyLinkedListTest, End_Const) {
  const DoublyLinkedList<int> list{1};
  EXPECT_NE(list.end(), list.begin());
  EXPECT_EQ(list.end(), ++list.begin());
}

TEST(DoublyLinkedListTest, Cend) {
  const DoublyLinkedList<int> list{1};
  EXPECT_NE(list.cend(), list.cbegin());
  EXPECT_EQ(list.cend(), ++list.cbegin());
}

TEST(DoublyLinkedListTest, Rbegin) {
  DoublyLinkedList<int> list;
  EXPECT_EQ(list.rbegin(), list.rend());

  list = {1, 2};
  auto it{list.rbegin()};
  EXPECT_EQ(*it, 2);
  EXPECT_NE(it, list.rend());

  *it = 10;
  EXPECT_EQ(list.Back(), 10);

  ++it;
  EXPECT_EQ(*it, 1);
  EXPECT_NE(it, list.rend());

  ++it;
  EXPECT_EQ(it, list.rend());
}

TEST(DoublyLinkedListTest, Rbegin_Const) {
  const DoublyLinkedList<int> list{1, 2};

  auto it{list.rbegin()};
  EXPECT_EQ(*it, 2);
  EXPECT_NE(it, list.rend());

  ++it;
  EXPECT_EQ(*it, 1);
  EXPECT_NE(it, list.rend());

  ++it;
  EXPECT_EQ(it, list.rend());
}

TEST(DoublyLinkedListTest, Crbegin) {
  const DoublyLinkedList<int> list{1, 2};

  auto it{list.crbegin()};
  EXPECT_EQ(*it, 2);
  EXPECT_NE(it, list.crend());

  ++it;
  EXPECT_EQ(*it, 1);
  EXPECT_NE(it, list.crend());

  ++it;
  EXPECT_EQ(it, list.crend());
}

TEST(DoublyLinkedListTest, Rend) {
  DoublyLinkedList<int> list{1};
  EXPECT_NE(list.rend(), list.rbegin());
  EXPECT_EQ(list.rend(), ++list.rbegin());
}

TEST(DoublyLinkedListTest, Rend_Const) {
  const DoublyLinkedList<int> list{1};
  EXPECT_NE(list.rend(), list.rbegin());
  EXPECT_EQ(list.rend(), ++list.rbegin());
}

TEST(DoublyLinkedListTest, Crend) {
  const DoublyLinkedList<int> list{1};
  EXPECT_NE(list.crend(), list.crbegin());
  EXPECT_EQ(list.crend(), ++list.crbegin());
}

// Capacity

TEST(DoublyLinkedListTest, Empty) {
  DoublyLinkedList<int> list;
  ASSERT_TRUE(list.Empty());

  list.PushFront(0);
  EXPECT_FALSE(list.Empty());
}

TEST(DoublyLinkedListTest, Size) {
  DoublyLinkedList<int> list;
  EXPECT_EQ(list.Size(), 0);

  list.PushFront(0);
  EXPECT_EQ(list.Size(), 1);
}

// Modifiers

TEST(DoublyLinkedListTest, Clear) {
  DoublyLinkedList<int> list{1};
  ASSERT_FALSE(list.Empty());

  list.Clear();
  EXPECT_TRUE(list.Empty());
}

TEST(DoublyLinkedListTest, Insert_Value) {
  DoublyLinkedList<int> list;
  auto inserted{list.end()};

  inserted = list.Insert(list.cend(), 1);
  EXPECT_EQ(*inserted, 1);
  EXPECT_EQ(list, (DoublyLinkedList{1}));

  inserted = list.Insert(At(list, 0), 2);
  EXPECT_EQ(*inserted, 2);
  EXPECT_EQ(list, (DoublyLinkedList{2, 1}));

  inserted = list.Insert(list.cend(), 3);
  EXPECT_EQ(*inserted, 3);
  EXPECT_EQ(list, (DoublyLinkedList{2, 1, 3}));

  inserted = list.Insert(At(list, 1), 4);
  EXPECT_EQ(*inserted, 4);
  EXPECT_EQ(list, (DoublyLinkedList{2, 4, 1, 3}));

  inserted = list.Insert(At(list, 0), 5);
  EXPECT_EQ(*inserted, 5);
  EXPECT_EQ(list, (DoublyLinkedList{5, 2, 4, 1, 3}));

  inserted = list.Insert(At(list, 3), 6);
  EXPECT_EQ(*inserted, 6);
  EXPECT_EQ(list, (DoublyLinkedList{5, 2, 4, 6, 1, 3}));

  inserted = list.Insert(list.cend(), 7);
  EXPECT_EQ(*inserted, 7);
  EXPECT_EQ(list, (DoublyLinkedList{5, 2, 4, 6, 1, 3, 7}));
}

TEST(DoublyLinkedListTest, Insert_ValueCount) {
  DoublyLinkedList<int> list;
  auto inserted{list.end()};

  inserted = list.Insert(list.cend(), 1, 1);
  EXPECT_EQ(*inserted, 1);
  EXPECT_EQ(list, (DoublyLinkedList{1}));

  inserted = list.Insert(At(list, 0), 2, 2);
  EXPECT_EQ(*inserted, 2);
  EXPECT_EQ(list, (DoublyLinkedList{2, 2, 1}));

  inserted = list.Insert(list.cend(), 3, 3);
  EXPECT_EQ(*inserted, 3);
  EXPECT_EQ(list, (DoublyLinkedList{2, 2, 1, 3, 3, 3}));

  inserted = list.Insert(At(list, 2), 4, 4);
  EXPECT_EQ(*inserted, 4);
  EXPECT_EQ(list, (DoublyLinkedList{2, 2, 4, 4, 4, 4, 1, 3, 3, 3}));

  inserted = list.Insert(At(list, 0), 5, 5);
  EXPECT_EQ(*inserted, 5);
  EXPECT_EQ(list,
            (DoublyLinkedList{5, 5, 5, 5, 5, 2, 2, 4, 4, 4, 4, 1, 3, 3, 3}));

  inserted = list.Insert(At(list, 11), 6, 6);
  EXPECT_EQ(*inserted, 6);
  EXPECT_EQ(list, (DoublyLinkedList{5, 5, 5, 5, 5, 2, 2, 4, 4, 4, 4,
                                    6, 6, 6, 6, 6, 6, 1, 3, 3, 3}));

  inserted = list.Insert(list.cend(), 7, 7);
  EXPECT_EQ(*inserted, 7);
  EXPECT_EQ(list, (DoublyLinkedList{5, 5, 5, 5, 5, 2, 2, 4, 4, 4, 4, 6, 6, 6,
                                    6, 6, 6, 1, 3, 3, 3, 7, 7, 7, 7, 7, 7, 7}));
}

TEST(DoublyLinkedListTest, Insert_Range) {
  const DoublyLinkedList<int> source{1, 11, 111, 2,  22, 33, 3,  44, 4,
                                     5, 55, 666, 66, 6,  7,  77, 777};
  DoublyLinkedList<int> list;
  auto inserted{list.end()};

  inserted = list.Insert(list.cend(), At(source, 0), At(source, 3));
  EXPECT_EQ(*inserted, 111);
  EXPECT_EQ(list, (DoublyLinkedList{1, 11, 111}));

  inserted = list.Insert(At(list, 0), At(source, 3), At(source, 5));
  EXPECT_EQ(*inserted, 22);
  EXPECT_EQ(list, (DoublyLinkedList{2, 22, 1, 11, 111}));

  inserted = list.Insert(list.cend(), At(source, 5), At(source, 7));
  EXPECT_EQ(*inserted, 3);
  EXPECT_EQ(list, (DoublyLinkedList{2, 22, 1, 11, 111, 33, 3}));

  inserted = list.Insert(At(list, 2), At(source, 7), At(source, 9));
  EXPECT_EQ(*inserted, 4);
  EXPECT_EQ(list, (DoublyLinkedList{2, 22, 44, 4, 1, 11, 111, 33, 3}));

  inserted = list.Insert(At(list, 7), At(source, 9), At(source, 11));
  EXPECT_EQ(*inserted, 55);
  EXPECT_EQ(list, (DoublyLinkedList{2, 22, 44, 4, 1, 11, 111, 5, 55, 33, 3}));

  inserted = list.Insert(At(list, 0), At(source, 11), At(source, 14));
  EXPECT_EQ(*inserted, 6);
  EXPECT_EQ(list, (DoublyLinkedList{666, 66, 6, 2, 22, 44, 4, 1, 11, 111, 5, 55,
                                    33, 3}));

  inserted = list.Insert(list.cend(), At(source, 14), At(source, 17));
  EXPECT_EQ(*inserted, 777);
  EXPECT_EQ(list, (DoublyLinkedList{666, 66, 6, 2, 22, 44, 4, 1, 11, 111, 5, 55,
                                    33, 3, 7, 77, 777}));
}

TEST(DoublyLinkedListTest, Insert_InitializerList) {
  DoublyLinkedList<int> list;
  auto inserted{list.end()};

  inserted = list.Insert(list.cend(), {1, 11, 111});
  EXPECT_EQ(*inserted, 111);
  EXPECT_EQ(list, (DoublyLinkedList{1, 11, 111}));

  inserted = list.Insert(At(list, 0), {2, 22});
  EXPECT_EQ(*inserted, 22);
  EXPECT_EQ(list, (DoublyLinkedList{2, 22, 1, 11, 111}));

  inserted = list.Insert(list.cend(), {33, 3});
  EXPECT_EQ(*inserted, 3);
  EXPECT_EQ(list, (DoublyLinkedList{2, 22, 1, 11, 111, 33, 3}));

  inserted = list.Insert(At(list, 2), {44, 4});
  EXPECT_EQ(*inserted, 4);
  EXPECT_EQ(list, (DoublyLinkedList{2, 22, 44, 4, 1, 11, 111, 33, 3}));

  inserted = list.Insert(At(list, 7), {5, 55});
  EXPECT_EQ(*inserted, 55);
  EXPECT_EQ(list, (DoublyLinkedList{2, 22, 44, 4, 1, 11, 111, 5, 55, 33, 3}));

  inserted = list.Insert(At(list, 0), {666, 66, 6});
  EXPECT_EQ(*inserted, 6);
  EXPECT_EQ(list, (DoublyLinkedList{666, 66, 6, 2, 22, 44, 4, 1, 11, 111, 5, 55,
                                    33, 3}));

  inserted = list.Insert(list.cend(), {7, 77, 777});
  EXPECT_EQ(*inserted, 777);
  EXPECT_EQ(list, (DoublyLinkedList{666, 66, 6, 2, 22, 44, 4, 1, 11, 111, 5, 55,
                                    33, 3, 7, 77, 777}));
}

TEST(DoublyLinkedListTest, Erase_Element) {
  DoublyLinkedList<int> list{0, 1, 2, 3, 4, 5};
  auto next{list.end()};

  next = list.Erase(At(list, 0));
  EXPECT_EQ(*next, 1);
  EXPECT_EQ(list, (DoublyLinkedList{1, 2, 3, 4, 5}));

  next = list.Erase(At(list, 1));
  EXPECT_EQ(*next, 3);
  EXPECT_EQ(list, (DoublyLinkedList{1, 3, 4, 5}));

  next = list.Erase(At(list, 2));
  EXPECT_EQ(*next, 5);
  EXPECT_EQ(list, (DoublyLinkedList{1, 3, 5}));

  next = list.Erase(At(list, 2));
  EXPECT_EQ(next, list.end());
  EXPECT_EQ(list, (DoublyLinkedList{1, 3}));

  next = list.Erase(At(list, 1));
  EXPECT_EQ(next, list.end());
  EXPECT_EQ(list, (DoublyLinkedList{1}));

  next = list.Erase(At(list, 0));
  EXPECT_EQ(next, list.end());
  EXPECT_TRUE(list.Empty());
}

TEST(DoublyLinkedListTest, Erase_Range) {
  DoublyLinkedList<int> list{0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
  auto next{list.end()};

  next = list.Erase(At(list, 0), At(list, 3));
  EXPECT_EQ(*next, 3);
  EXPECT_EQ(list, (DoublyLinkedList{3, 4, 5, 6, 7, 8, 9}));

  next = list.Erase(At(list, 2), At(list, 4));
  EXPECT_EQ(*next, 7);
  EXPECT_EQ(list, (DoublyLinkedList{3, 4, 7, 8, 9}));

  next = list.Erase(At(list, 1), At(list, 4));
  EXPECT_EQ(*next, 9);
  EXPECT_EQ(list, (DoublyLinkedList{3, 9}));

  next = list.Erase(At(list, 0), list.cend());
  EXPECT_EQ(next, list.end());
  EXPECT_TRUE(list.Empty());
}

TEST(DoublyLinkedListTest, PushBack) {
  DoublyLinkedList<int> list;

  list.PushBack(1);
  EXPECT_EQ(list.Back(), 1);
  EXPECT_EQ(list.Size(), 1);

  list.PushBack(2);
  EXPECT_EQ(list.Back(), 2);
  EXPECT_EQ(list.Size(), 2);
}

TEST(DoublyLinkedListTest, PopBack) {
  DoublyLinkedList<int> list{1, 2};
  EXPECT_EQ(list.Back(), 2);

  list.PopBack();
  EXPECT_EQ(list.Back(), 1);

  list.PopBack();
  EXPECT_TRUE(list.Empty());
}

TEST(DoublyLinkedListTest, PushFront) {
  DoublyLinkedList<int> list;

  list.PushFront(1);
  EXPECT_EQ(list.Front(), 1);
  EXPECT_EQ(list.Size(), 1);

  list.PushFront(2);
  EXPECT_EQ(list.Front(), 2);
  EXPECT_EQ(list.Size(), 2);
}

TEST(DoublyLinkedListTest, PopFront) {
  DoublyLinkedList<int> list{1, 2};
  EXPECT_EQ(list.Front(), 1);

  list.PopFront();
  EXPECT_EQ(list.Front(), 2);

  list.PopFront();
  EXPECT_TRUE(list.Empty());
}

TEST(DoublyLinkedListTest, Resize) {
  DoublyLinkedList<int> list;

  list.Resize(2);
  EXPECT_EQ(list, (DoublyLinkedList{0, 0}));

  list.Resize(5, 1);
  EXPECT_EQ(list, (DoublyLinkedList{0, 0, 1, 1, 1}));

  list.Resize(3);
  EXPECT_EQ(list, (DoublyLinkedList{0, 0, 1}));
}

TEST(DoublyLinkedListTest, Swap) {
  DoublyLinkedList<int> a{1};
  DoublyLinkedList<int> b{2, 3};

  a.Swap(b);
  EXPECT_EQ(a.Size(), 2);
  EXPECT_EQ(b.Size(), 1);
}

// Operations

TEST(DoublyLinkedListTest, Splice_List) {
  DoublyLinkedList<int> list;

  DoublyLinkedList<int> a{};
  list.Splice(At(list, 0), a);

  EXPECT_EQ(list, (DoublyLinkedList<int>{}));
  EXPECT_TRUE(a.Empty());

  DoublyLinkedList<int> b{1};
  list.Splice(At(list, 0), b);

  EXPECT_EQ(list, (DoublyLinkedList<int>{1}));
  EXPECT_TRUE(b.Empty());

  DoublyLinkedList<int> c{3, 2};
  list.Splice(At(list, 0), c);

  EXPECT_EQ(list, (DoublyLinkedList<int>{3, 2, 1}));
  EXPECT_TRUE(c.Empty());

  DoublyLinkedList<int> d{5};
  list.Splice(list.cend(), d);

  EXPECT_EQ(list, (DoublyLinkedList<int>{3, 2, 1, 5}));
  EXPECT_TRUE(d.Empty());

  DoublyLinkedList<int> e{4};
  list.Splice(At(list, 3), e);

  EXPECT_EQ(list, (DoublyLinkedList<int>{3, 2, 1, 4, 5}));
  EXPECT_TRUE(e.Empty());
}

TEST(DoublyLinkedListTest, Splice_Element) {
  DoublyLinkedList<int> list;

  DoublyLinkedList<int> a{1};
  list.Splice(At(list, 0), a, At(a, 0));

  EXPECT_EQ(list, (DoublyLinkedList{1}));
  EXPECT_TRUE(a.Empty());

  DoublyLinkedList<int> b{2, 1};
  list.Splice(At(list, 0), b, At(b, 0));

  EXPECT_EQ(list, (DoublyLinkedList{2, 1}));
  EXPECT_EQ(b, (DoublyLinkedList{1}));

  DoublyLinkedList<int> c{1, 2, 3};
  list.Splice(list.cend(), c, At(c, 2));

  EXPECT_EQ(list, (DoublyLinkedList{2, 1, 3}));
  EXPECT_EQ(c, (DoublyLinkedList{1, 2}));

  DoublyLinkedList<int> d{1, 4, 2};
  list.Splice(At(list, 1), d, At(d, 1));

  EXPECT_EQ(list, (DoublyLinkedList{2, 4, 1, 3}));
  EXPECT_EQ(d, (DoublyLinkedList{1, 2}));

  list.Splice(At(list, 0), list, At(list, 2));
  EXPECT_EQ(list, (DoublyLinkedList{1, 2, 4, 3}));

  list.Splice(list.cend(), list, At(list, 2));
  EXPECT_EQ(list, (DoublyLinkedList{1, 2, 3, 4}));
}

TEST(DoublyLinkedListTest, Splice_Range) {
  DoublyLinkedList<int> list;

  DoublyLinkedList<int> a{1, 2};
  list.Splice(At(list, 0), a, At(a, 0), At(a, 1));

  EXPECT_EQ(list, (DoublyLinkedList{1}));
  EXPECT_EQ(a, (DoublyLinkedList{2}));

  DoublyLinkedList<int> b{2, 3, 4};
  list.Splice(At(list, 0), b, At(b, 0), b.cend());

  EXPECT_EQ(list, (DoublyLinkedList{2, 3, 4, 1}));
  EXPECT_TRUE(b.Empty());

  DoublyLinkedList<int> c{1, 5, 6, 2};
  list.Splice(list.cend(), c, At(c, 1), At(c, 3));

  EXPECT_EQ(list, (DoublyLinkedList{2, 3, 4, 1, 5, 6}));
  EXPECT_EQ(c, (DoublyLinkedList{1, 2}));

  DoublyLinkedList<int> d{3, 7, 8};
  list.Splice(list.cend(), d, At(d, 1), d.cend());

  EXPECT_EQ(list, (DoublyLinkedList{2, 3, 4, 1, 5, 6, 7, 8}));
  EXPECT_EQ(d, (DoublyLinkedList{3}));

  DoublyLinkedList<int> e{1, 9, 10, 2};
  list.Splice(At(list, 7), e, At(e, 1), At(e, 3));

  EXPECT_EQ(list, (DoublyLinkedList{2, 3, 4, 1, 5, 6, 7, 9, 10, 8}));
  EXPECT_EQ(e, (DoublyLinkedList{1, 2}));

  list.Splice(At(list, 4), list, At(list, 0), At(list, 3));
  EXPECT_EQ(list, (DoublyLinkedList{1, 2, 3, 4, 5, 6, 7, 9, 10, 8}));

  list.Splice(list.cend(), list, At(list, 7), At(list, 3));
  EXPECT_EQ(list, (DoublyLinkedList{1, 2, 3, 4, 5, 6, 7, 9, 10, 8}));
}

TEST(DoublyLinkedListTest, Reverse) {
  DoublyLinkedList<int> list{1, 2, 3};

  list.Reverse();
  EXPECT_EQ(list, (DoublyLinkedList{3, 2, 1}));
}

// Comparison operators

TEST(DoublyLinkedListTest, EqualOperator) {
  const DoublyLinkedList<int> a{1, 2, 3};
  const DoublyLinkedList<int> b{1, 2, 3};
  EXPECT_EQ(a, b);
}

TEST(DoublyLinkedListTest, NotEqualOperator) {
  const DoublyLinkedList<int> a{1, 2, 3};
  const DoublyLinkedList<int> b{4, 5, 6};
  EXPECT_NE(a, b);
}

TEST(DoublyLinkedListTest, LessOperator) {
  DoublyLinkedList<int> a{4, 5};
  DoublyLinkedList<int> b{1, 2, 3};
  EXPECT_LT(a, b);

  a = {1, 2, 3};
  b = {4, 5, 6};
  EXPECT_LT(a, b);
}

TEST(DoublyLinkedListTest, LessEqualOperator) {
  DoublyLinkedList<int> a{1, 2};
  DoublyLinkedList<int> b{4, 5, 6};
  EXPECT_LE(a, b);

  a = {1, 2, 3};
  b = {4, 5, 6};
  EXPECT_LE(a, b);

  a = b = {1, 2, 3};
  EXPECT_LE(a, b);
}

TEST(DoublyLinkedListTest, GreaterOperator) {
  DoublyLinkedList<int> a{1, 2, 3};
  DoublyLinkedList<int> b{4, 5};
  EXPECT_GT(a, b);

  a = {4, 5, 6};
  b = {1, 2, 3};
  EXPECT_GT(a, b);
}

TEST(DoublyLinkedListTest, GreaterEqualOperator) {
  DoublyLinkedList<int> a{1, 2, 3};
  DoublyLinkedList<int> b{4, 5};
  EXPECT_GE(a, b);

  a = {4, 5, 6};
  b = {1, 2, 3};
  EXPECT_GE(a, b);

  a = b = {1, 2, 3};
  EXPECT_GE(a, b);
}