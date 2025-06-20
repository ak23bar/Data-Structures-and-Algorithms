#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <map>
#include <random>
#include <vector>

#include "hashmap.h"

using namespace std;
using namespace testing;

//CS 251 Project 5: HashMap TEST suite
//Authored by: Akbar Aman
//Core : Testing insertion, lookup, deletion, and capacity management via core functions
//Iter : Testing begin() and next() methods work correctly for iterating through all key-value pairs 

namespace {

// Random number generator helper for consistent test behavior
class Random {
 private:
  static mt19937 rng;

 public:
  static void seed(int s) {
    Random::rng.seed(s);
  }

  /**
   * Generate a random integer in the range 0 (inclusive) to `max` (exclusive)
   */
  static int randInt(int max) {
    // Even though mt19937 is standardized, STL distributions aren't!
    // So, unfortunately, even though this is biased, we're forced to
    // do something like this. Technically uint32_fast_t isn't...
    // totally consistent across platforms? But within reason it works.
    return rng() % (max + 1);
  }
};

std::mt19937 Random::rng;


// CORE tests all the CORE operations to ensure the HashMap implements the expected behavior of a hash map ds.(THIS WILL BE ON EXAM)
// Icludes testing buggy edge cases like duplicate keys, collisions, and capacity resizing.
class HashMapCore : public Test {
 protected:
  void SetUp() override {
    Random::seed(42); //for consistency
  }
};


// CORE TESTS

// Test constructors and basic methods
TEST_F(HashMapCore, DefaultConstructor) {
  HashMap<int, string> map;
  EXPECT_TRUE(map.empty());
  EXPECT_EQ(map.size(), 0);
}

//Random but needed test
TEST_F(HashMapCore, CustomCapacityConstructor) {
  HashMap<int, string> map(20);
  EXPECT_TRUE(map.empty());
  EXPECT_EQ(map.size(), 0);
}

// Test empty and size
TEST_F(HashMapCore, EmptyAndSize) {
  HashMap<int, string> map;
  EXPECT_TRUE(map.empty());
  EXPECT_EQ(map.size(), 0);
  
  map.insert(1, "one");
  EXPECT_FALSE(map.empty());
  EXPECT_EQ(map.size(), 1);
  
  map.insert(2, "two");
  EXPECT_FALSE(map.empty());
  EXPECT_EQ(map.size(), 2);
}

// Test insert functionality
TEST_F(HashMapCore, InsertBasic) {
  HashMap<int, string> map;
  map.insert(1, "one");
  map.insert(2, "two");
  map.insert(3, "three");
  
  EXPECT_EQ(map.size(), 3);
  EXPECT_TRUE(map.contains(1));
  EXPECT_TRUE(map.contains(2));
  EXPECT_TRUE(map.contains(3));
}

TEST_F(HashMapCore, InsertDuplicate) {
  HashMap<int, string> map;
  map.insert(1, "one");
  map.insert(1, "ONE");  // Should not update the value
  
  EXPECT_EQ(map.size(), 1);
  EXPECT_EQ(map.at(1), "one");  // Should still be the original value
}

TEST_F(HashMapCore, InsertCollision) {
  // Create a map with capacity 10 to force collisions
  HashMap<int, string> map(10);
  map.insert(0, "zero");
  map.insert(10, "ten");  // will hash to the same bucket as 0
  map.insert(20, "twenty");  // will hash to the same bucket as 0 and 10
  
  EXPECT_EQ(map.size(), 3);
  EXPECT_EQ(map.at(0), "zero");
  EXPECT_EQ(map.at(10), "ten");
  EXPECT_EQ(map.at(20), "twenty");
}

TEST_F(HashMapCore, InsertResize) {
  // Create a map with capacity 10 and insert 16 elements
  // should trigger a resize (load factor > 1.5)
  HashMap<int, string> map(10);
  
  for (int i = 0; i < 16; i++) {
    map.insert(i, to_string(i));
  }
  
  EXPECT_EQ(map.size(), 16);
  // Check that all values are still accessible
  for (int i = 0; i < 16; i++) {
    EXPECT_TRUE(map.contains(i));
    EXPECT_EQ(map.at(i), to_string(i));
  }
  
  // Capacity should be doubled to 20
  EXPECT_EQ(map.get_capacity(), 20);
}

// Test contains functionality
TEST_F(HashMapCore, ContainsBasic) {
  HashMap<int, string> map;
  map.insert(1, "one");
  map.insert(2, "two");
  
  EXPECT_TRUE(map.contains(1));
  EXPECT_TRUE(map.contains(2));
  EXPECT_FALSE(map.contains(3));
}

TEST_F(HashMapCore, ContainsCollision) {
  HashMap<int, string> map(10);
  map.insert(0, "zero");
  map.insert(10, "ten");  // Same bucket as 0
  
  EXPECT_TRUE(map.contains(0));
  EXPECT_TRUE(map.contains(10));
  EXPECT_FALSE(map.contains(20));
}

// Test at functionality
TEST_F(HashMapCore, AtBasic) {
  HashMap<int, string> map;
  map.insert(1, "one");
  map.insert(2, "two");
  
  EXPECT_EQ(map.at(1), "one");
  EXPECT_EQ(map.at(2), "two");
}

TEST_F(HashMapCore, AtThrowsWhenKeyMissing) {
  HashMap<int, string> map;
  map.insert(1, "one");
  
  EXPECT_THROW(map.at(2), out_of_range);
}

TEST_F(HashMapCore, AtCollision) {
  HashMap<int, string> map(10);
  map.insert(0, "zero");
  map.insert(10, "ten");  // Same bucket as 0
  map.insert(20, "twenty");  // Same bucket as 0 and 10
  
  EXPECT_EQ(map.at(0), "zero");
  EXPECT_EQ(map.at(10), "ten");
  EXPECT_EQ(map.at(20), "twenty");
}

// Test clear functionality
TEST_F(HashMapCore, Clear) {
  HashMap<int, string> map;
  map.insert(1, "one");
  map.insert(2, "two");
  map.insert(3, "three");
  
  EXPECT_EQ(map.size(), 3);
  
  map.clear();
  
  EXPECT_TRUE(map.empty());
  EXPECT_EQ(map.size(), 0);
  EXPECT_FALSE(map.contains(1));
  EXPECT_FALSE(map.contains(2));
  EXPECT_FALSE(map.contains(3));
}

// Test erase functionality
TEST_F(HashMapCore, EraseBasic) {
  HashMap<int, string> map;
  map.insert(1, "one");
  map.insert(2, "two");
  
  EXPECT_EQ(map.erase(1), "one");
  EXPECT_EQ(map.size(), 1);
  EXPECT_FALSE(map.contains(1));
  EXPECT_TRUE(map.contains(2));
}

//Self-explanatory
TEST_F(HashMapCore, EraseThrowsWhenKeyMissing) {
  HashMap<int, string> map;
  map.insert(1, "one");
  
  EXPECT_THROW(map.erase(2), out_of_range);
}

TEST_F(HashMapCore, EraseCollision) {
  HashMap<int, string> map(10);
  map.insert(0, "zero");
  map.insert(10, "ten");  // Same bucket as 0
  map.insert(20, "twenty");  // Same bucket as 0 and 10
  
  EXPECT_EQ(map.erase(10), "ten");
  EXPECT_EQ(map.size(), 2);
  EXPECT_TRUE(map.contains(0));
  EXPECT_FALSE(map.contains(10));
  EXPECT_TRUE(map.contains(20));
  
  // Make sure can still access the other values in the same chain
  EXPECT_EQ(map.at(0), "zero");
  EXPECT_EQ(map.at(20), "twenty");
}

// Test copy constructor
TEST_F(HashMapCore, CopyConstructor) {
  HashMap<int, string> map1;
  map1.insert(1, "one");
  map1.insert(2, "two");
  
  HashMap<int, string> map2(map1);
  
  EXPECT_EQ(map2.size(), 2);
  EXPECT_TRUE(map2.contains(1));
  EXPECT_TRUE(map2.contains(2));
  EXPECT_EQ(map2.at(1), "one");
  EXPECT_EQ(map2.at(2), "two");
  
  // Modifying map2 should not affect map1
  map2.insert(3, "three");
  EXPECT_EQ(map2.size(), 3);
  EXPECT_EQ(map1.size(), 2);
  EXPECT_FALSE(map1.contains(3));
}

TEST_F(HashMapCore, CopyConstructorWithCollisions) {
  HashMap<int, string> map1(10);
  map1.insert(0, "zero");
  map1.insert(10, "ten");  // Same bucket as 0
  map1.insert(20, "twenty");  // Same bucket as 0 and 10
  
  HashMap<int, string> map2(map1);
  
  EXPECT_EQ(map2.size(), 3);
  EXPECT_TRUE(map2.contains(0));
  EXPECT_TRUE(map2.contains(10));
  EXPECT_TRUE(map2.contains(20));
  EXPECT_EQ(map2.at(0), "zero");
  EXPECT_EQ(map2.at(10), "ten");
  EXPECT_EQ(map2.at(20), "twenty");
}

TEST_F(HashMapCore, CopyEmptyMap) {
  HashMap<int, string> map1;
  HashMap<int, string> map2(map1);
  
  EXPECT_TRUE(map2.empty());
  EXPECT_EQ(map2.size(), 0);
}

// Test assignment operator
TEST_F(HashMapCore, AssignmentOperator) {
  HashMap<int, string> map1;
  map1.insert(1, "one");
  map1.insert(2, "two");
  
  HashMap<int, string> map2;
  map2.insert(3, "three");
  
  map2 = map1;
  
  EXPECT_EQ(map2.size(), 2);
  EXPECT_TRUE(map2.contains(1));
  EXPECT_TRUE(map2.contains(2));
  EXPECT_FALSE(map2.contains(3));
  EXPECT_EQ(map2.at(1), "one");
  EXPECT_EQ(map2.at(2), "two");
  
  // Modifying map2 should not affect map1
  map2.insert(4, "four");
  EXPECT_EQ(map2.size(), 3);
  EXPECT_EQ(map1.size(), 2);
  EXPECT_FALSE(map1.contains(4));
}

TEST_F(HashMapCore, AssignmentOperatorWithCollisions) {
  HashMap<int, string> map1(10);
  map1.insert(0, "zero");
  map1.insert(10, "ten");  // Same bucket as 0
  map1.insert(20, "twenty");  // Same bucket as 0 and 10
  
  HashMap<int, string> map2;
  map2 = map1;
  
  EXPECT_EQ(map2.size(), 3);
  EXPECT_TRUE(map2.contains(0));
  EXPECT_TRUE(map2.contains(10));
  EXPECT_TRUE(map2.contains(20));
  EXPECT_EQ(map2.at(0), "zero");
  EXPECT_EQ(map2.at(10), "ten");
  EXPECT_EQ(map2.at(20), "twenty");
}

TEST_F(HashMapCore, AssignEmptyMap) {
  HashMap<int, string> map1;
  HashMap<int, string> map2;
  map2.insert(1, "one");
  
  map2 = map1;
  
  EXPECT_TRUE(map2.empty());
  EXPECT_EQ(map2.size(), 0);
}

//Direct selfassignment testing without just saying map = map 
TEST_F(HashMapCore, SelfAssignment) {
  HashMap<int, string> map;
  map.insert(1, "one");
  map.insert(2, "two");
  
  // Create a reference to the same map
  HashMap<int, string>& mapRef = map;
  map = mapRef;  // Self-assignment via ref as was done in previous projects
  
  EXPECT_EQ(map.size(), 2);
  EXPECT_TRUE(map.contains(1));
  EXPECT_TRUE(map.contains(2));
  EXPECT_EQ(map.at(1), "one");
  EXPECT_EQ(map.at(2), "two");
}


// ITERATING TESTS


// Iter TESTS verify both overarching and buggy errors including cases with empty maps, maps with collisions,
// and maps that have been modified between iterations.

class HashMapIterating : public Test {
 protected:
  void SetUp() override {
    Random::seed(42); //for consistency
  }
};

// Test begin and next functionality
TEST_F(HashMapIterating, EmptyMap) {
  HashMap<int, string> map;
  map.begin();
  
  int key;
  string value;
  EXPECT_FALSE(map.next(key, value));
}

TEST_F(HashMapIterating, SingleElement) {
  HashMap<int, string> map;
  map.insert(1, "one");
  
  map.begin();
  
  int key;
  string value;
  EXPECT_TRUE(map.next(key, value));
  EXPECT_EQ(key, 1);
  EXPECT_EQ(value, "one");
  
  EXPECT_FALSE(map.next(key, value));
}

TEST_F(HashMapIterating, MultipleElements) {
  HashMap<int, string> map;
  map.insert(1, "one");
  map.insert(2, "two");
  map.insert(3, "three");
  
  map.begin();
  
  int key;
  string value;
  vector<pair<int, string>> elements;
  
  while (map.next(key, value)) {
    elements.push_back({key, value});
  }
  
  EXPECT_EQ(elements.size(), 3);
  
  // Sort the elements to make the test "deterministic" (fancy terminilogy)
  sort(elements.begin(), elements.end());
  
  EXPECT_EQ(elements[0].first, 1);
  EXPECT_EQ(elements[0].second, "one");
  EXPECT_EQ(elements[1].first, 2);
  EXPECT_EQ(elements[1].second, "two");
  EXPECT_EQ(elements[2].first, 3);
  EXPECT_EQ(elements[2].second, "three");
}

TEST_F(HashMapIterating, ElementsWithCollisions) {
  HashMap<int, string> map(10);
  map.insert(0, "zero");
  map.insert(10, "ten");  // Same bucket as 0
  map.insert(20, "twenty");  // Same bucket as 0 and 10
  map.insert(5, "five");  // Different bucket
  
  map.begin();
  
  int key;
  string value;
  vector<pair<int, string>> elements;
  
  while (map.next(key, value)) {
    elements.push_back({key, value});
  }
  
  EXPECT_EQ(elements.size(), 4);
  
  // Create a set of expected elements
  set<pair<int, string>> expected = {
    {0, "zero"},
    {5, "five"},
    {10, "ten"},
    {20, "twenty"}
  };
  
  // Convert vector to set for comparison
  set<pair<int, string>> actual(elements.begin(), elements.end());
  
  EXPECT_EQ(actual, expected);
}

TEST_F(HashMapIterating, IterateAfterModification) {
  HashMap<int, string> map;
  map.insert(1, "one");
  map.insert(2, "two");
  
  map.erase(1);
  map.insert(3, "three");
  
  map.begin();
  
  int key;
  string value;
  vector<pair<int, string>> elements;
  
  while (map.next(key, value)) {
    elements.push_back({key, value});
  }
  
  EXPECT_EQ(elements.size(), 2);
  
  // Sort the elements to make the test "deterministic" (fancy terminology)
  sort(elements.begin(), elements.end());
  
  EXPECT_EQ(elements[0].first, 2);
  EXPECT_EQ(elements[0].second, "two");
  EXPECT_EQ(elements[1].first, 3);
  EXPECT_EQ(elements[1].second, "three");
}

}  // namespace

//All tests passed 100/100 :)