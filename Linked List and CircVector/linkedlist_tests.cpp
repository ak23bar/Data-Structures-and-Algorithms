#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <stdexcept>


#include "linkedlist.h"

using namespace std;
using namespace testing;

// LL CORE TESTS
TEST(LinkedListCore, DefaultConstructor) {
  LinkedList<int> list;
  EXPECT_TRUE(list.empty());
  EXPECT_EQ(list.size(), 0);
}

// Test push_front and size
TEST(LinkedListCore, PushFront) {
  LinkedList<int> list;
  list.push_front(1);
  EXPECT_FALSE(list.empty());
  EXPECT_EQ(list.size(), 1);
  EXPECT_EQ(list.at(0), 1);

  list.push_front(2);
  EXPECT_EQ(list.size(), 2);
  EXPECT_EQ(list.at(0), 2);
  EXPECT_EQ(list.at(1), 1);
}

// Test push_back and to_string
TEST(LinkedListCore, PushBack) {
  LinkedList<int> list;
  list.push_back(1);
  EXPECT_FALSE(list.empty());
  EXPECT_EQ(list.size(), 1);
  EXPECT_EQ(list.at(0), 1);

  list.push_back(2);
  EXPECT_EQ(list.size(), 2);
  EXPECT_EQ(list.at(0), 1);
  EXPECT_EQ(list.at(1), 2);

  EXPECT_EQ(list.to_string(), "[1, 2]");
}

// Test pop_front
TEST(LinkedListCore, PopFront) {
  LinkedList<int> list;
  list.push_back(1);
  list.push_back(2);
  list.push_back(3);

  EXPECT_EQ(list.pop_front(), 1);
  EXPECT_EQ(list.size(), 2);
  EXPECT_EQ(list.at(0), 2);

  EXPECT_EQ(list.pop_front(), 2);
  EXPECT_EQ(list.size(), 1);
  EXPECT_EQ(list.at(0), 3);

  EXPECT_EQ(list.pop_front(), 3);
  EXPECT_TRUE(list.empty());
}

// Test pop_back
TEST(LinkedListCore, PopBack) {
  LinkedList<int> list;
  list.push_back(1);
  list.push_back(2);
  list.push_back(3);

  EXPECT_EQ(list.pop_back(), 3);
  EXPECT_EQ(list.size(), 2);
  EXPECT_EQ(list.at(1), 2);

  EXPECT_EQ(list.pop_back(), 2);
  EXPECT_EQ(list.size(), 1);
  EXPECT_EQ(list.at(0), 1);

  EXPECT_EQ(list.pop_back(), 1);
  EXPECT_TRUE(list.empty());
}

// Test at() and handling invalid indices
TEST(LinkedListCore, AtAndInvalidIndices) {
  LinkedList<int> list;
  list.push_back(10);
  list.push_back(20);

  EXPECT_EQ(list.at(0), 10);
  EXPECT_EQ(list.at(1), 20);

  // Check for out-of-range access 
  EXPECT_THROW(list.at(2), std::exception);
  EXPECT_THROW(list.at(100), std::exception);

  // Test empty list
  LinkedList<int> empty_list;
  EXPECT_THROW(empty_list.at(0), std::exception);
}

// Test copy constructor
TEST(LinkedListCore, CopyConstructor) {
  LinkedList<int> list1;
  list1.push_back(1);
  list1.push_back(2);
  list1.push_back(3);

  // Create a copy
  LinkedList<int> list2(list1);

  // Verify values were copied
  EXPECT_EQ(list2.size(), 3);
  EXPECT_EQ(list2.at(0), 1);
  EXPECT_EQ(list2.at(1), 2);
  EXPECT_EQ(list2.at(2), 3);

  // Verify deep copy
  list1.push_back(4);
  EXPECT_EQ(list1.size(), 4);
  EXPECT_EQ(list2.size(), 3);

  // test empty list copy
  LinkedList<int> empty_list;
  LinkedList<int> empty_copy(empty_list);
  EXPECT_TRUE(empty_copy.empty());
}

// Test assignment operator
TEST(LinkedListCore, AssignmentOperator) {
  LinkedList<int> list1;
  list1.push_back(1);
  list1.push_back(2);

  LinkedList<int> list2;
  list2.push_back(10);
  list2.push_back(20);
  list2.push_back(30);

  // Assign list1 = list2
  list1 = list2;

  // Verify values were copied
  EXPECT_EQ(list1.size(), 3);
  EXPECT_EQ(list1.at(0), 10);
  EXPECT_EQ(list1.at(1), 20);
  EXPECT_EQ(list1.at(2), 30);

  // Verify deep copy
  list2.pop_back();
  EXPECT_EQ(list2.size(), 2);
  EXPECT_EQ(list1.size(), 3);

  // Test self-assignment

  EXPECT_EQ(list1.size(), 3);
  EXPECT_EQ(list1.at(0), 10);

  // Test assignment of empty list
  LinkedList<int> empty_list;
  list1 = empty_list;
  EXPECT_TRUE(list1.empty());
}

// Test clear
TEST(LinkedListCore, Clear) {
  LinkedList<int> list;
  list.push_back(1);
  list.push_back(2);

  list.clear();
  EXPECT_TRUE(list.empty());
  EXPECT_EQ(list.size(), 0);

  // Test that it adds elements after clearing
  list.push_back(3);
  EXPECT_EQ(list.size(), 1);
  EXPECT_EQ(list.at(0), 3);
}

// Test find
TEST(LinkedListCore, Find) {
  LinkedList<int> list;
  list.push_back(10);
  list.push_back(20);
  list.push_back(30);
  list.push_back(20);  // Duplicate val

  EXPECT_EQ(list.find(10), 0);
  EXPECT_EQ(list.find(20), 1);  // Find 1st occurrence
  EXPECT_EQ(list.find(30), 2);
  // For not found values, check that it returns a val
  size_t not_found_result = list.find(40);
  EXPECT_TRUE(not_found_result >= list.size());
}

// Test remove_at
TEST(LinkedListCore, RemoveAt) {
  LinkedList<int> list;
  list.push_back(10);
  list.push_back(20);
  list.push_back(30);
  list.push_back(40);

  // Remove from middle
  list.remove_at(1);
  EXPECT_EQ(list.size(), 3);
  EXPECT_EQ(list.at(0), 10);
  EXPECT_EQ(list.at(1), 30);
  EXPECT_EQ(list.at(2), 40);

  // Remove from front (hence use of pop_front)
  list.remove_at(0);
  EXPECT_EQ(list.size(), 2);
  EXPECT_EQ(list.at(0), 30);

  // Remove from end
  list.remove_at(1);
  EXPECT_EQ(list.size(), 1);
  EXPECT_EQ(list.at(0), 30);

  // Test invalid idx
  EXPECT_THROW(list.remove_at(1), std::exception);
  EXPECT_THROW(list.remove_at(100), std::exception);
}


// Test insert_after
TEST(LinkedListCore, InsertAfter) {
  LinkedList<int> list;
  list.push_back(10);
  list.push_back(30);

  // Insert in middle
  list.insert_after(0, 20);
  EXPECT_EQ(list.size(), 3);
  EXPECT_EQ(list.at(0), 10);
  EXPECT_EQ(list.at(1), 20);
  EXPECT_EQ(list.at(2), 30);

  // Insert at end
  list.insert_after(2, 40);
  EXPECT_EQ(list.size(), 4);
  EXPECT_EQ(list.at(3), 40);

  // Test invalid idx
  EXPECT_THROW(list.insert_after(4, 50), std::exception);
  EXPECT_THROW(list.insert_after(100, 60), std::exception);
}

TEST(LinkedListCore, InsertAfterInvalidIndex) {
  LinkedList<int> list;
  list.push_back(10);

  // Valid insert
  list.insert_after(0, 20);
  EXPECT_EQ(list.size(), 2);
  EXPECT_EQ(list.at(1), 20);

  // Test invalid idx
  EXPECT_THROW(list.insert_after(2, 30), std::exception);
  EXPECT_THROW(list.insert_after(100, 40), std::exception);

  // Test on empty list
  LinkedList<int> empty_list;
  EXPECT_THROW(empty_list.insert_after(0, 10), std::exception);
}

// Tests for exceptions when accessing invalid idxs
TEST(LinkedListCore, AtExceptions) {
  LinkedList<int> list;
  list.push_back(1);
  list.push_back(2);

  // Test valid access
  EXPECT_EQ(list.at(0), 1);
  EXPECT_EQ(list.at(1), 2);

  // Test invalid access
  EXPECT_THROW(list.at(2), std::exception);
  EXPECT_THROW(list.at(100), std::exception);

  // Test empty list
  LinkedList<int> empty_list;
  EXPECT_THROW(empty_list.at(0), std::exception);
}



// Test remove_every_other
TEST(LinkedListCore, RemoveEveryOther) {
  // Test with odd num of elements
  LinkedList<int> list1;
  list1.push_back(0);
  list1.push_back(1);
  list1.push_back(2);
  list1.push_back(3);
  list1.push_back(4);

  list1.remove_every_other();
  EXPECT_EQ(list1.size(), 3);
  EXPECT_EQ(list1.at(0), 0);
  EXPECT_EQ(list1.at(1), 2);
  EXPECT_EQ(list1.at(2), 4);

  // Test with even num of elements
  LinkedList<int> list2;
  list2.push_back(0);
  list2.push_back(1);
  list2.push_back(2);
  list2.push_back(3);

  list2.remove_every_other();
  EXPECT_EQ(list2.size(), 2);
  EXPECT_EQ(list2.at(0), 0);
  EXPECT_EQ(list2.at(1), 2);

  // Test with single element
  LinkedList<int> list3;
  list3.push_back(0);

  list3.remove_every_other();
  EXPECT_EQ(list3.size(), 1);
  EXPECT_EQ(list3.at(0), 0);

  // Test with empty list
  LinkedList<int> list4;
  list4.remove_every_other();
  EXPECT_TRUE(list4.empty());
}

// Tests for exceptions when calling pop on empty list
TEST(LinkedListCore, PopFromEmptyList) {
  LinkedList<int> list;

  // Test pop from empty list
  EXPECT_THROW(list.pop_front(), std::exception);
  EXPECT_THROW(list.pop_back(), std::exception);
}

// Test pop_back edge case with one element
TEST(LinkedListCore, PopBackSingleElement) {
  LinkedList<int> list;
  list.push_back(42);

  EXPECT_EQ(list.size(), 1);
  EXPECT_EQ(list.pop_back(), 42);
  EXPECT_TRUE(list.empty());
  EXPECT_EQ(list.size(), 0);
}

// Test push_back on an empty list (case in buggy)
TEST(LinkedListCore, PushBackOnEmptyList) {
  LinkedList<int> list;
  list.push_back(42);

  EXPECT_EQ(list.size(), 1);
  EXPECT_EQ(list.at(0), 42);
}

// Test for proper memory cleanup in clear()
TEST(LinkedListCore, ClearMemoryCleanup) {
  LinkedList<int> list;
  for (int i = 0; i < 10; i++) {
    list.push_back(i);
  }

  list.clear();
  EXPECT_TRUE(list.empty());

  // add more elements to ensure everything was properly cleaned
  for (int i = 0; i < 5; i++) {
    list.push_back(i * 10);
  }

  EXPECT_EQ(list.size(), 5);
  for (size_t i = 0; i < list.size(); i++) {
    EXPECT_EQ(list.at(i), static_cast<int>(i) * 10);
  }
}

// LL Augmented Tests

// Tests copy constructor, operator=, to_string, find, remove_at

TEST(LinkedListAugmented, CopyConstructorEmpty) {
  LinkedList<int> empty;
  LinkedList<int> copied(empty);
  
  EXPECT_TRUE(copied.empty());
  EXPECT_EQ(copied.size(), 0);
}

TEST(LinkedListAugmented, CopyConstructorSingleElement) {
  LinkedList<int> original;
  original.push_back(42);
  
  LinkedList<int> copied(original);
  EXPECT_EQ(copied.size(), 1);
  EXPECT_EQ(copied.at(0), 42);
}

TEST(LinkedListAugmented, CopyConstructorMultipleElements) {
  LinkedList<int> original;
  for (int i = 0; i < 5; i++) {
    original.push_back(i);
  }
  
  LinkedList<int> copied(original);
  EXPECT_EQ(copied.size(), 5);
  for (size_t i = 0; i < 5; i++) {
    EXPECT_EQ(copied.at(i), i);
  }
  
  // Verify deep copy - modifying original should not affect copy tho
  original.at(0) = 100;
  EXPECT_EQ(copied.at(0), 0);
}

TEST(LinkedListAugmented, AssignmentOperatorEmpty) {
  LinkedList<int> list1;
  list1.push_back(10);
  
  LinkedList<int> empty;
  list1 = empty;
  
  EXPECT_TRUE(list1.empty());
}

TEST(LinkedListAugmented, AssignmentOperatorNonEmpty) {
  LinkedList<int> list1;
  list1.push_back(10);
  list1.push_back(20);
  
  LinkedList<int> list2;
  list2.push_back(30);
  list2.push_back(40);
  list2.push_back(50);
  
  list1 = list2;
  
  EXPECT_EQ(list1.size(), 3);
  EXPECT_EQ(list1.at(0), 30);
  EXPECT_EQ(list1.at(1), 40);
  EXPECT_EQ(list1.at(2), 50);
  
  // Verify deep copy
  list2.at(0) = 99;
  EXPECT_EQ(list1.at(0), 30);
}

TEST(LinkedListAugmented, AssignmentOperatorSelfAssignment) {
  LinkedList<int> list;
  list.push_back(10);
  list.push_back(20);
  
  // Create a ref to the same list
  LinkedList<int>& listRef = list;
  
  // Self-assignment thru ref
  list = listRef;
  
  // Should still have the same data
  EXPECT_EQ(list.size(), 2);
  EXPECT_EQ(list.at(0), 10);
  EXPECT_EQ(list.at(1), 20);
}

TEST(LinkedListAugmented, ToStringEmpty) {
  LinkedList<int> list;
  EXPECT_EQ(list.to_string(), "[]");
}

TEST(LinkedListAugmented, ToStringSingleElement) {
  LinkedList<int> list;
  list.push_back(42);
  EXPECT_EQ(list.to_string(), "[42]");
}

TEST(LinkedListAugmented, ToStringMultipleElements) {
  LinkedList<int> list;
  list.push_back(10);
  list.push_back(20);
  list.push_back(30);
  
  EXPECT_EQ(list.to_string(), "[10, 20, 30]");
}

TEST(LinkedListAugmented, FindElementExists) {
  LinkedList<int> list;
  list.push_back(10);
  list.push_back(20);
  list.push_back(30);
  list.push_back(20); // Duplicate
  
  EXPECT_EQ(list.find(10), 0);
  EXPECT_EQ(list.find(20), 1); // First occurrence
  EXPECT_EQ(list.find(30), 2);
}

TEST(LinkedListAugmented, FindElementDoesNotExist) {
  LinkedList<int> list;
  list.push_back(10);
  list.push_back(20);

  EXPECT_EQ(list.find(999), -1);
}

TEST(LinkedListAugmented, FindEmptyList) {
  LinkedList<int> list;

  EXPECT_EQ(list.find(10), -1);  
}


TEST(LinkedListAugmented, RemoveAtFront) {
  LinkedList<int> list;
  list.push_back(10);
  list.push_back(20);
  list.push_back(30);
  
  list.remove_at(0);
  EXPECT_EQ(list.size(), 2);
  EXPECT_EQ(list.at(0), 20);
  EXPECT_EQ(list.at(1), 30);
}

TEST(LinkedListAugmented, RemoveAtMiddle) {
  LinkedList<int> list;
  list.push_back(10);
  list.push_back(20);
  list.push_back(30);
  
  list.remove_at(1);
  EXPECT_EQ(list.size(), 2);
  EXPECT_EQ(list.at(0), 10);
  EXPECT_EQ(list.at(1), 30);
}

TEST(LinkedListAugmented, RemoveAtLast) {
  LinkedList<int> list;
  list.push_back(10);
  list.push_back(20);
  list.push_back(30);
  
  list.remove_at(2);
  EXPECT_EQ(list.size(), 2);
  EXPECT_EQ(list.at(0), 10);
  EXPECT_EQ(list.at(1), 20);
}

TEST(LinkedListAugmented, RemoveAtSingleElement) {
  LinkedList<int> list;
  list.push_back(10);
  
  list.remove_at(0);
  EXPECT_TRUE(list.empty());
}

TEST(LinkedListAugmented, RemoveAtInvalidIndex) {
  LinkedList<int> list;
  list.push_back(10);
  
  EXPECT_THROW(list.remove_at(1), out_of_range);
  EXPECT_THROW(list.remove_at(100), out_of_range);
}

// LL EXTRAS (100 pts)
TEST(LinkedListExtras, InsertAfterFront) {
  LinkedList<int> list;
  list.push_back(10);
  list.push_back(30);
  
  list.insert_after(0, 20);
  EXPECT_EQ(list.size(), 3);
  EXPECT_EQ(list.at(0), 10);
  EXPECT_EQ(list.at(1), 20);
  EXPECT_EQ(list.at(2), 30);
}

TEST(LinkedListExtras, InsertAfterLast) {
  LinkedList<int> list;
  list.push_back(10);
  list.push_back(20);
  
  list.insert_after(1, 30);
  EXPECT_EQ(list.size(), 3);
  EXPECT_EQ(list.at(0), 10);
  EXPECT_EQ(list.at(1), 20);
  EXPECT_EQ(list.at(2), 30);
}

TEST(LinkedListExtras, InsertAfterMiddle) {
  LinkedList<int> list;
  list.push_back(10);
  list.push_back(20);
  list.push_back(40);
  
  list.insert_after(1, 30);
  EXPECT_EQ(list.size(), 4);
  EXPECT_EQ(list.at(0), 10);
  EXPECT_EQ(list.at(1), 20);
  EXPECT_EQ(list.at(2), 30);
  EXPECT_EQ(list.at(3), 40);
}

TEST(LinkedListExtras, InsertAfterSingleElement) {
  LinkedList<int> list;
  list.push_back(10);
  
  list.insert_after(0, 20);
  EXPECT_EQ(list.size(), 2);
  EXPECT_EQ(list.at(0), 10);
  EXPECT_EQ(list.at(1), 20);
}

TEST(LinkedListExtras, InsertAfterInvalidIndex) {
  LinkedList<int> list;
  list.push_back(10);
  
  EXPECT_THROW(list.insert_after(1, 20), out_of_range);
  EXPECT_THROW(list.insert_after(100, 20), out_of_range);
}

TEST(LinkedListExtras, InsertAfterEmptyList) {
  LinkedList<int> list;
  
  EXPECT_THROW(list.insert_after(0, 10), out_of_range);
}

TEST(LinkedListExtras, RemoveEveryOtherEmpty) {
  LinkedList<int> list;
  list.remove_every_other();
  EXPECT_TRUE(list.empty());
}

TEST(LinkedListExtras, RemoveEveryOtherSingleElement) {
  LinkedList<int> list;
  list.push_back(10);
  
  list.remove_every_other();
  EXPECT_EQ(list.size(), 1);
  EXPECT_EQ(list.at(0), 10);
}

TEST(LinkedListExtras, RemoveEveryOtherTwoElements) {
  LinkedList<int> list;
  list.push_back(10);
  list.push_back(20);
  
  list.remove_every_other();
  EXPECT_EQ(list.size(), 1);
  EXPECT_EQ(list.at(0), 10);
}

TEST(LinkedListExtras, RemoveEveryOtherOddSize) {
  LinkedList<int> list;
  list.push_back(0);
  list.push_back(1);
  list.push_back(2);
  list.push_back(3);
  list.push_back(4);
  
  list.remove_every_other();
  EXPECT_EQ(list.size(), 3);
  EXPECT_EQ(list.at(0), 0);
  EXPECT_EQ(list.at(1), 2);
  EXPECT_EQ(list.at(2), 4);
}

TEST(LinkedListExtras, RemoveEveryOtherEvenSize) {
  LinkedList<int> list;
  list.push_back(0);
  list.push_back(1);
  list.push_back(2);
  list.push_back(3);
  
  list.remove_every_other();
  EXPECT_EQ(list.size(), 2);
  EXPECT_EQ(list.at(0), 0);
  EXPECT_EQ(list.at(1), 2);
}

//All tests passed for LL :)