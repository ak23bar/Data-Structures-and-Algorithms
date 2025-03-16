#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <stdexcept>

#include "circvector.h"

using namespace std;
using namespace testing;

// CircVec CORE TESTS
TEST(CircVectorCore, DefaultConstructor) {
  CircVector<int> vec;
  EXPECT_TRUE(vec.empty());
  EXPECT_EQ(vec.size(), 0);
}

TEST(CircVectorCore, EmptyAndSize) {
  CircVector<int> vec;
  EXPECT_TRUE(vec.empty());

  vec.push_back(10);
  EXPECT_FALSE(vec.empty());
  EXPECT_EQ(vec.size(), 1);

  vec.pop_back();
  EXPECT_TRUE(vec.empty());
  EXPECT_EQ(vec.size(), 0);
}

TEST(CircVectorCore, PushBack) {
  CircVector<int> vec;
  vec.push_back(1);
  EXPECT_FALSE(vec.empty());
  EXPECT_EQ(vec.size(), 1);
  EXPECT_EQ(vec.at(0), 1);

  vec.push_back(2);
  EXPECT_EQ(vec.size(), 2);
  EXPECT_EQ(vec.at(0), 1);
  EXPECT_EQ(vec.at(1), 2);
}

TEST(CircVectorCore, PushBackForceResize) {
  CircVector<int> vec;
  // Push enough elements to force a resize
  for (int i = 0; i < 10; i++) {
    vec.push_back(i);
  }

  EXPECT_EQ(vec.size(), 10);
  for (int i = 0; i < 10; i++) {
    EXPECT_EQ(vec.at(i), i);
  }
}

TEST(CircVectorCore, PushBackWithWraparound) {
  CircVector<int> vec;

  // 1) create situation where front_idx is not at 0
  vec.push_front(3);
  vec.push_front(2);
  vec.push_front(1);

  // 2) push to back which should handle wrapping correctly
  vec.push_back(4);
  vec.push_back(5);

  EXPECT_EQ(vec.size(), 5);
  EXPECT_EQ(vec.at(0), 1);
  EXPECT_EQ(vec.at(1), 2);
  EXPECT_EQ(vec.at(2), 3);
  EXPECT_EQ(vec.at(3), 4);
  EXPECT_EQ(vec.at(4), 5);
}

TEST(CircVectorCore, PopBack) {
  CircVector<int> vec;
  vec.push_back(1);
  vec.push_back(2);
  vec.push_back(3);

  EXPECT_EQ(vec.pop_back(), 3);
  EXPECT_EQ(vec.size(), 2);
  EXPECT_EQ(vec.at(1), 2);

  EXPECT_EQ(vec.pop_back(), 2);
  EXPECT_EQ(vec.size(), 1);
  EXPECT_EQ(vec.at(0), 1);

  EXPECT_EQ(vec.pop_back(), 1);
  EXPECT_TRUE(vec.empty());
}

TEST(CircVectorCore, PopBackEmpty) {
  CircVector<int> vec;
  EXPECT_THROW(vec.pop_back(), std::exception);  
}

TEST(CircVectorCore, PopBackWithWraparound) {
  CircVector<int> vec;

  vec.push_front(2);
  vec.push_front(1);
  vec.push_back(3);
  vec.push_back(4);

  EXPECT_EQ(vec.pop_back(), 4);
  EXPECT_EQ(vec.size(), 3);
  EXPECT_EQ(vec.at(2), 3);

  EXPECT_EQ(vec.pop_back(), 3);
  EXPECT_EQ(vec.size(), 2);
}

TEST(CircVectorCore, PushFront) {
  CircVector<int> vec;
  vec.push_front(1);
  EXPECT_EQ(vec.size(), 1);
  EXPECT_EQ(vec.at(0), 1);

  vec.push_front(2);
  EXPECT_EQ(vec.size(), 2);
  EXPECT_EQ(vec.at(0), 2);
  EXPECT_EQ(vec.at(1), 1);
}

TEST(CircVectorCore, PushFrontForceResize) {
  CircVector<int> vec;
  // force a resize
  for (int i = 0; i < 10; i++) {
    vec.push_front(i);
  }

  EXPECT_EQ(vec.size(), 10);
  for (int i = 0; i < 10; i++) {
    EXPECT_EQ(vec.at(i), 9 - i);
  }
}

TEST(CircVectorCore, PushFrontAtIndexZero) {
  CircVector<int> vec;
  // situation where front_idx might be at 0
  vec.push_back(1);
  vec.pop_front();    // should make front_idx go to 1
  vec.push_front(2);  // should make front_idx go to 0
  vec.push_front(3);  // should handle index 0 correctly

  EXPECT_EQ(vec.size(), 2);
  EXPECT_EQ(vec.at(0), 3);
  EXPECT_EQ(vec.at(1), 2);
}

TEST(CircVectorCore, PopFront) {
  CircVector<int> vec;
  vec.push_front(1);
  vec.push_front(2);
  vec.push_front(3);

  EXPECT_EQ(vec.pop_front(), 3);
  EXPECT_EQ(vec.size(), 2);
  EXPECT_EQ(vec.at(0), 2);

  EXPECT_EQ(vec.pop_front(), 2);
  EXPECT_EQ(vec.size(), 1);
  EXPECT_EQ(vec.at(0), 1);

  EXPECT_EQ(vec.pop_front(), 1);
  EXPECT_TRUE(vec.empty());
}


TEST(CircVectorCore, PopFrontEmpty) {
  CircVector<int> vec;
  EXPECT_THROW(vec.pop_front(), std::exception);
}

TEST(CircVectorCore, PopFrontWithWrap) {
  CircVector<int> vec;

  vec.push_back(1);
  vec.push_back(2);
  vec.pop_front();  // moves front_idx
  vec.push_front(3);

  EXPECT_EQ(vec.pop_front(), 3);
  EXPECT_EQ(vec.size(), 1);
  EXPECT_EQ(vec.at(0), 2);
}

TEST(CircVectorCore, At) {
  CircVector<int> vec;
  vec.push_back(10);
  vec.push_back(20);

  EXPECT_EQ(vec.at(0), 10);
  EXPECT_EQ(vec.at(1), 20);

  vec.at(1) = 25;
  EXPECT_EQ(vec.at(1), 25);
}

TEST(CircVectorCore, AtWithInvalidIndices) {
  CircVector<int> vec;
  vec.push_back(10);

  // Test out of bounds access with index 1
  EXPECT_THROW(vec.at(1), std::exception);

  // Test out of bounds access with a significantly large index
  EXPECT_THROW(vec.at(100), std::exception);

  // Test empty vector access
  CircVector<int> empty_vec;
  EXPECT_THROW(empty_vec.at(0), std::exception);
}

TEST(CircVectorCore, RemoveAtInvalidIndex) {
  CircVector<int> vec;
  vec.push_back(10);
  vec.push_back(20);

  // Test out of bounds with index 2
  EXPECT_THROW(vec.remove_at(2), std::exception);

  // Test out of bounds with a significantly large index
  EXPECT_THROW(vec.remove_at(100), std::exception);

  // Test empty vector
  CircVector<int> empty_vec;
  EXPECT_THROW(empty_vec.remove_at(0), std::exception);
}

TEST(CircVectorCore, AtWithWraparound) {
  CircVector<int> vec;

  // situation with wraparound
  vec.push_front(10);
  vec.push_front(20);
  vec.push_back(30);

  EXPECT_EQ(vec.at(0), 20);
  EXPECT_EQ(vec.at(1), 10);
  EXPECT_EQ(vec.at(2), 30);
}

TEST(CircVectorCore, Clear) {
  CircVector<int> vec;
  vec.push_back(1);
  vec.push_back(2);

  vec.clear();
  EXPECT_TRUE(vec.empty());
  EXPECT_EQ(vec.size(), 0);

  //use of the vector is possible after clearing
  vec.push_back(3);
  EXPECT_EQ(vec.size(), 1);
  EXPECT_EQ(vec.at(0), 3);
}

TEST(CircVectorCore, MixedOperations) {
  CircVector<int> vec;

  vec.push_front(1);
  vec.push_back(2);
  vec.push_front(3);
  vec.push_back(4);

  EXPECT_EQ(vec.size(), 4);
  EXPECT_EQ(vec.at(0), 3);
  EXPECT_EQ(vec.at(1), 1);
  EXPECT_EQ(vec.at(2), 2);
  EXPECT_EQ(vec.at(3), 4);

  vec.clear();
  EXPECT_TRUE(vec.empty());
}

TEST(CircVectorCore, RemoveAt) {
  CircVector<int> vec;
  vec.push_back(10);
  vec.push_back(20);
  vec.push_back(30);
  vec.push_back(40);

  // Remove from middle
  vec.remove_at(1);
  EXPECT_EQ(vec.size(), 3);
  EXPECT_EQ(vec.at(0), 10);
  EXPECT_EQ(vec.at(1), 30);
  EXPECT_EQ(vec.at(2), 40);

  // Remove from front
  vec.remove_at(0);
  EXPECT_EQ(vec.size(), 2);
  EXPECT_EQ(vec.at(0), 30);
  EXPECT_EQ(vec.at(1), 40);

  // Remove from end
  vec.remove_at(1);
  EXPECT_EQ(vec.size(), 1);
  EXPECT_EQ(vec.at(0), 30);
}

TEST(CircVectorCore, PushBackResizingStressTest) {
  CircVector<int> vec;
  
  // Get initial capacity by pushing elements until we would need to resize and then testing if elements are still accessible
  const int LARGE_SIZE = 100; // Much larger than default capacity
  
  for (int i = 0; i < LARGE_SIZE; i++) {
    vec.push_back(i);
    
    // Verify all elements are accessible
    for (int j = 0; j <= i; j++) {
      EXPECT_EQ(vec.at(j), j);
    }
  }
  
  // Should have grown in capacity to accommodate all elements (dynamic malloc)
  EXPECT_EQ(vec.size(), LARGE_SIZE);
}

TEST(CircVectorCore, PushFrontResizingStressTest) {
  CircVector<int> vec;
  
  // Get initial capacity by pushing elements until we would need to resize and then testing if elements are still accessible
  const int LARGE_SIZE = 100; 
  
  for (int i = 0; i < LARGE_SIZE; i++) {
    vec.push_front(i);
    
    // Verify all elements are accessible
    for (int j = 0; j <= i; j++) {
      EXPECT_EQ(vec.at(j), i - j);
    }
  }
  
  //passes if grown in capacity to accommodate all elements
  EXPECT_EQ(vec.size(), LARGE_SIZE);
}

// CircVec AUGMENTED TESTS

TEST(CircVectorAugmented, CopyConstructorEmpty) {
  CircVector<int> empty;
  CircVector<int> copied(empty);
  
  EXPECT_TRUE(copied.empty());
  EXPECT_EQ(copied.size(), 0);
}

TEST(CircVectorAugmented, CopyConstructorSingleElement) {
  CircVector<int> original;
  original.push_back(42);
  
  CircVector<int> copied(original);
  EXPECT_EQ(copied.size(), 1);
  EXPECT_EQ(copied.at(0), 42);
}

TEST(CircVectorAugmented, CopyConstructorMultipleElements) {
  CircVector<int> original;
  for (int i = 0; i < 5; i++) {
    original.push_back(i);
  }
  
  CircVector<int> copied(original);
  EXPECT_EQ(copied.size(), 5);
  for (size_t i = 0; i < 5; i++) {
    EXPECT_EQ(copied.at(i), i);
  }
  
  // Verify deep copy; changing original should not affect copy
  original.at(0) = 100;
  EXPECT_EQ(copied.at(0), 0);
}

TEST(CircVectorAugmented, CopyConstructorWraparound) {
  // Create a vector that will have to wrap around "buffer"
  CircVector<int> original(5);
  
  // Push enough elements to force wraparound
  original.push_back(1);
  original.push_back(2);
  original.push_back(3);
  original.push_front(0); //  might cause wraparound
  original.push_front(-1); // should def cause wraparound
  
  // Copy and verify all elements
  CircVector<int> copied(original);
  EXPECT_EQ(copied.size(), 5);
  for (int i = -1; i < 4; i++) {
    EXPECT_EQ(copied.at(i+1), i);
  }
}

TEST(CircVectorAugmented, AssignmentOperatorEmpty) {
  CircVector<int> vec1;
  vec1.push_back(10);
  
  CircVector<int> empty;
  vec1 = empty;
  
  EXPECT_TRUE(vec1.empty());
}

TEST(CircVectorAugmented, AssignmentOperatorNonEmpty) {
  CircVector<int> vec1;
  vec1.push_back(10);
  vec1.push_back(20);
  
  CircVector<int> vec2;
  vec2.push_back(30);
  vec2.push_back(40);
  vec2.push_back(50);
  
  vec1 = vec2;
  
  EXPECT_EQ(vec1.size(), 3);
  EXPECT_EQ(vec1.at(0), 30);
  EXPECT_EQ(vec1.at(1), 40);
  EXPECT_EQ(vec1.at(2), 50);
  
  // Verify deep copy
  vec2.at(0) = 99;
  EXPECT_EQ(vec1.at(0), 30);
}

TEST(CircVectorAugmented, AssignmentOperatorWraparound) {
  // Create source vector with wraparound
  CircVector<int> src(5); 
  src.push_back(1);
  src.push_back(2);
  src.push_back(3);
  src.push_front(0);
  src.push_front(-1);
  
  // Target vector
  CircVector<int> target;
  target.push_back(100);
  
  // Assignment
  target = src;
  
  // Verify all elements
  EXPECT_EQ(target.size(), 5);
  for (int i = -1; i < 4; i++) {
    EXPECT_EQ(target.at(i+1), i);
  }
}

TEST(CircVectorAugmented, AssignmentOperatorSelfAssignment) {
  CircVector<int> vec;
  vec.push_back(10);
  vec.push_back(20);
  
  // Create a ref to the same vector
  CircVector<int>& vecRef = vec;
  
  // Self-assignment through ref
  vec = vecRef;
  
  // Should still have the same data
  EXPECT_EQ(vec.size(), 2);
  EXPECT_EQ(vec.at(0), 10);
  EXPECT_EQ(vec.at(1), 20);
}

TEST(CircVectorAugmented, ToStringEmpty) {
  CircVector<int> vec;
  EXPECT_EQ(vec.to_string(), "[]");
}

TEST(CircVectorAugmented, ToStringSingleElement) {
  CircVector<int> vec;
  vec.push_back(42);
  EXPECT_EQ(vec.to_string(), "[42]");
}

TEST(CircVectorAugmented, ToStringMultipleElements) {
  CircVector<int> vec;
  vec.push_back(10);
  vec.push_back(20);
  vec.push_back(30);
  
  EXPECT_EQ(vec.to_string(), "[10, 20, 30]");
}

TEST(CircVectorAugmented, ToStringWraparound) {
  // Create vector with wraparound
  CircVector<int> vec(5);
  vec.push_back(1);
  vec.push_back(2);
  vec.push_back(3);
  vec.push_front(0);
  vec.push_front(-1);
  
  EXPECT_EQ(vec.to_string(), "[-1, 0, 1, 2, 3]");
}

TEST(CircVectorAugmented, FindElementExists) {
  CircVector<int> vec;
  vec.push_back(10);
  vec.push_back(20);
  vec.push_back(30);
  vec.push_back(20); // Duplicate
  
  EXPECT_EQ(vec.find(10), 0);
  EXPECT_EQ(vec.find(20), 1); // First occurrence
  EXPECT_EQ(vec.find(30), 2);
}

TEST(CircVectorAugmented, FindElementDoesNotExist) {
  CircVector<int> vec;
  vec.push_back(10);
  vec.push_back(20);
  
  EXPECT_EQ(vec.find(999), -1);
}

TEST(CircVectorAugmented, FindEmptyVector) {
  CircVector<int> vec;
  
  EXPECT_EQ(vec.find(10), -1);
}


TEST(CircVectorAugmented, FindWithWraparound) {
  // Create vector with wraparound
  CircVector<int> vec(5); // capacity 5
  vec.push_back(1);
  vec.push_back(2);
  vec.push_back(3);
  vec.push_front(0);
  vec.push_front(-1);
  
  EXPECT_EQ(vec.find(-1), 0);
  EXPECT_EQ(vec.find(0), 1);
  EXPECT_EQ(vec.find(3), 4);
  EXPECT_EQ(vec.find(999), -1);
}

TEST(CircVectorAugmented, RemoveAtFront) {
  CircVector<int> vec;
  vec.push_back(10);
  vec.push_back(20);
  vec.push_back(30);
  
  vec.remove_at(0);
  EXPECT_EQ(vec.size(), 2);
  EXPECT_EQ(vec.at(0), 20);
  EXPECT_EQ(vec.at(1), 30);
}

TEST(CircVectorAugmented, RemoveAtMiddle) {
  CircVector<int> vec;
  vec.push_back(10);
  vec.push_back(20);
  vec.push_back(30);
  
  vec.remove_at(1);
  EXPECT_EQ(vec.size(), 2);
  EXPECT_EQ(vec.at(0), 10);
  EXPECT_EQ(vec.at(1), 30);
}

TEST(CircVectorAugmented, RemoveAtLast) {
  CircVector<int> vec;
  vec.push_back(10);
  vec.push_back(20);
  vec.push_back(30);
  
  vec.remove_at(2);
  EXPECT_EQ(vec.size(), 2);
  EXPECT_EQ(vec.at(0), 10);
  EXPECT_EQ(vec.at(1), 20);
}

TEST(CircVectorAugmented, RemoveAtSingleElement) {
  CircVector<int> vec;
  vec.push_back(10);
  
  vec.remove_at(0);
  EXPECT_TRUE(vec.empty());
}

TEST(CircVectorAugmented, RemoveAtInvalidIndex) {
  CircVector<int> vec;
  vec.push_back(10);
  
  EXPECT_THROW(vec.remove_at(1), out_of_range);
  EXPECT_THROW(vec.remove_at(100), out_of_range);
}

TEST(CircVectorAugmented, RemoveAtWithWraparound) {
  // Create vector with wraparound
  CircVector<int> vec(5); // capacity 5
  vec.push_back(1);
  vec.push_back(2);
  vec.push_back(3);
  vec.push_front(0);
  vec.push_front(-1);
  
  vec.remove_at(2); // Remove element with value 1
  EXPECT_EQ(vec.size(), 4);
  EXPECT_EQ(vec.at(0), -1);
  EXPECT_EQ(vec.at(1), 0);
  EXPECT_EQ(vec.at(2), 2);
  EXPECT_EQ(vec.at(3), 3);
}

// CircVec EXTRAS (100 Pts)

TEST(CircVectorExtras, InsertAfterFront) {
  CircVector<int> vec;
  vec.push_back(10);
  vec.push_back(30);
  
  vec.insert_after(0, 20);
  EXPECT_EQ(vec.size(), 3);
  EXPECT_EQ(vec.at(0), 10);
  EXPECT_EQ(vec.at(1), 20);
  EXPECT_EQ(vec.at(2), 30);
}

TEST(CircVectorExtras, InsertAfterLast) {
  CircVector<int> vec;
  vec.push_back(10);
  vec.push_back(20);
  
  vec.insert_after(1, 30);
  EXPECT_EQ(vec.size(), 3);
  EXPECT_EQ(vec.at(0), 10);
  EXPECT_EQ(vec.at(1), 20);
  EXPECT_EQ(vec.at(2), 30);
}

TEST(CircVectorExtras, InsertAfterMiddle) {
  CircVector<int> vec;
  vec.push_back(10);
  vec.push_back(20);
  vec.push_back(40);
  
  vec.insert_after(1, 30);
  EXPECT_EQ(vec.size(), 4);
  EXPECT_EQ(vec.at(0), 10);
  EXPECT_EQ(vec.at(1), 20);
  EXPECT_EQ(vec.at(2), 30);
  EXPECT_EQ(vec.at(3), 40);
}

TEST(CircVectorExtras, InsertAfterSingleElement) {
  CircVector<int> vec;
  vec.push_back(10);
  
  vec.insert_after(0, 20);
  EXPECT_EQ(vec.size(), 2);
  EXPECT_EQ(vec.at(0), 10);
  EXPECT_EQ(vec.at(1), 20);
}

TEST(CircVectorExtras, InsertAfterInvalidIndex) {
  CircVector<int> vec;
  vec.push_back(10);
  
  EXPECT_ANY_THROW(vec.insert_after(1, 20));
  EXPECT_ANY_THROW(vec.insert_after(100, 20));
}

TEST(CircVectorExtras, InsertAfterEmptyVector) {
  CircVector<int> vec;
  
  EXPECT_ANY_THROW(vec.insert_after(0, 10));
}

TEST(CircVectorExtras, InsertAfterWithWraparound) {
  CircVector<int> vec(5);
  vec.push_back(0);
  vec.push_back(1);
  
  // Create wraparound by pushing and popping
  vec.push_back(2);
  vec.push_back(3);
  vec.push_back(4);
  vec.pop_front();
  vec.pop_front();
  
  // At this point, the vector should look like:
  // [_, _, 2, 3, 4] with front at idx 2
  
  // Insert 5 after the first element (which is @ idx 2)
  vec.insert_after(0, 5);
  
  EXPECT_EQ(vec.size(), 4);
  EXPECT_EQ(vec.at(0), 2);
  EXPECT_EQ(vec.at(1), 5); 
  EXPECT_EQ(vec.at(2), 3); 
  EXPECT_EQ(vec.at(3), 4);
}

TEST(CircVectorExtras, InsertAfterWhenFull) {
  // Create a vector with exactly capacity elements
  CircVector<int> vec(3);
  vec.push_back(10);
  vec.push_back(20);
  vec.push_back(30);
  
  //should trigger resize()
  vec.insert_after(1, 15);
  
  EXPECT_EQ(vec.size(), 4);
  EXPECT_EQ(vec.at(0), 10);
  EXPECT_EQ(vec.at(1), 20);
  EXPECT_EQ(vec.at(2), 15);
  EXPECT_EQ(vec.at(3), 30);
  
  EXPECT_GT(vec.get_capacity(), 3);
}

TEST(CircVectorExtras, RemoveEveryOtherEmpty) {
  CircVector<int> vec;
  vec.remove_every_other();
  EXPECT_TRUE(vec.empty());
}

TEST(CircVectorExtras, RemoveEveryOtherSingleElement) {
  CircVector<int> vec;
  vec.push_back(10);
  
  vec.remove_every_other();
  EXPECT_EQ(vec.size(), 1);
  EXPECT_EQ(vec.at(0), 10);
}

TEST(CircVectorExtras, RemoveEveryOtherTwoElements) {
  CircVector<int> vec;
  vec.push_back(10);
  vec.push_back(20);
  
  vec.remove_every_other();
  EXPECT_EQ(vec.size(), 1);
  EXPECT_EQ(vec.at(0), 10);
}

TEST(CircVectorExtras, RemoveEveryOtherOddSize) {
  CircVector<int> vec;
  vec.push_back(0);
  vec.push_back(1);
  vec.push_back(2);
  vec.push_back(3);
  vec.push_back(4);
  
  vec.remove_every_other();
  EXPECT_EQ(vec.size(), 3);
  EXPECT_EQ(vec.at(0), 0);
  EXPECT_EQ(vec.at(1), 2);
  EXPECT_EQ(vec.at(2), 4);
}

TEST(CircVectorExtras, RemoveEveryOtherEvenSize) {
  CircVector<int> vec;
  vec.push_back(0);
  vec.push_back(1);
  vec.push_back(2);
  vec.push_back(3);
  
  vec.remove_every_other();
  EXPECT_EQ(vec.size(), 2);
  EXPECT_EQ(vec.at(0), 0);
  EXPECT_EQ(vec.at(1), 2);
}

TEST(CircVectorExtras, RemoveEveryOtherWithWraparound) {
  // Create vector with wraparound
  CircVector<int> vec(5); // capacity 5
  vec.push_back(0);
  vec.push_back(1);
  vec.push_back(2);
  vec.push_back(3);
  vec.push_back(4);
  
  // Force wraparound by popping and pushing
  vec.pop_front(); // [1,2,3,4]
  vec.pop_front(); // [2,3,4]
  vec.push_back(5);// [2,3,4,5]
  vec.push_back(6);// [2,3,4,5,6] with wraparound
  
  vec.remove_every_other();
  EXPECT_EQ(vec.size(), 3);
  EXPECT_EQ(vec.at(0), 2);
  EXPECT_EQ(vec.at(1), 4);
  EXPECT_EQ(vec.at(2), 6);
}

//All tests passed for CircVec :)