#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <random>
#include <map>
#include <vector>
#include <string>
#include <sstream>
#include <utility>
#include "bstmap.h"

using namespace std;
using namespace testing;

namespace {

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

// Helper function to convert a standard map to the expected to_string format
template<typename K, typename V>
string map_to_string(const map<K, V>& m) {
  ostringstream ss;
  for (const auto &[key, value] : m) {
    ss << key << ": " << value << endl;
  }
  return ss.str();
}

// Helper function to shuffle a vector
template <typename T>
void shuffle_vec(vector<T>& vec) {
  for (int i = vec.size() - 1; i >= 1; i--) {
    size_t j = Random::randInt(i);
    swap(vec.at(i), vec.at(j));
  }
}

// CORE TESTS

// Test constructor
TEST(BSTMapCore, Constructor) {
  BSTMap<int, string> bst;
  EXPECT_TRUE(bst.empty());
  EXPECT_EQ(bst.size(), 0);
}

// Test empty and size
TEST(BSTMapCore, EmptyAndSize) {
  // Empty tree
  BSTMap<int, string> bst;
  EXPECT_TRUE(bst.empty());
  EXPECT_EQ(bst.size(), 0);
  
  // Insert one element
  bst.insert(10, "ten");
  EXPECT_FALSE(bst.empty());
  EXPECT_EQ(bst.size(), 1);
  
  // Insert more elements
  bst.insert(5, "five");
  bst.insert(15, "fifteen");
  EXPECT_FALSE(bst.empty());
  EXPECT_EQ(bst.size(), 3);
  
  // Clear the tree
  bst.clear();
  EXPECT_TRUE(bst.empty());
  EXPECT_EQ(bst.size(), 0);
}

// Test insert
TEST(BSTMapCore, Insert) {
  BSTMap<int, string> bst;
  
  // Insert first element
  bst.insert(10, "ten");
  EXPECT_EQ(bst.size(), 1);
  EXPECT_TRUE(bst.contains(10));
  EXPECT_EQ(bst.at(10), "ten");
  
  // Insert more elements
  bst.insert(5, "five");
  bst.insert(15, "fifteen");
  EXPECT_EQ(bst.size(), 3);
  EXPECT_TRUE(bst.contains(5));
  EXPECT_TRUE(bst.contains(15));
  EXPECT_EQ(bst.at(5), "five");
  EXPECT_EQ(bst.at(15), "fifteen");
  
  // Try inserting a duplicate key
  bst.insert(10, "TEN");
  EXPECT_EQ(bst.size(), 3); // Size shouldn't change
  EXPECT_EQ(bst.at(10), "ten"); // Value shouldn't change
}

// Test insert with random data
TEST(BSTMapCore, InsertRandom) {
  Random::seed(42);
  BSTMap<int, string> bst;
  map<int, string> reference;
  
  const int N = 100;
  vector<int> keys;
  
  // Generate unique keys
  for (int i = 0; i < N; i++) {
    keys.push_back(i);
  }
  
  // Shuffle keys to insert in random order
  shuffle_vec(keys);
  
  // Insert elements
  for (int key : keys) {
    string value = "value_" + to_string(key);
    bst.insert(key, value);
    reference[key] = value;
    
    // Check size is correct after each insertion
    EXPECT_EQ(bst.size(), reference.size());
  }
  
  // Verify all keys are present with correct values
  for (int key : keys) {
    EXPECT_TRUE(bst.contains(key));
    EXPECT_EQ(bst.at(key), reference[key]);
  }
}

// Test contains
TEST(BSTMapCore, Contains) {
  BSTMap<int, string> bst;
  
  // Check on empty tree
  EXPECT_FALSE(bst.contains(10));
  
  // Insert some elements
  bst.insert(10, "ten");
  bst.insert(5, "five");
  bst.insert(15, "fifteen");
  bst.insert(3, "three");
  bst.insert(7, "seven");
  
  // Check existing keys
  EXPECT_TRUE(bst.contains(10));
  EXPECT_TRUE(bst.contains(5));
  EXPECT_TRUE(bst.contains(15));
  EXPECT_TRUE(bst.contains(3));
  EXPECT_TRUE(bst.contains(7));
  
  // Check non-existing keys
  EXPECT_FALSE(bst.contains(1));
  EXPECT_FALSE(bst.contains(20));
  EXPECT_FALSE(bst.contains(6));
}

// Test at
TEST(BSTMapCore, At) {
  BSTMap<int, string> bst;
  
  // Check on empty tree; should throw out of range as expected in autograder
  EXPECT_THROW(bst.at(10), out_of_range);
  
  // Insert some elements
  bst.insert(10, "ten");
  bst.insert(5, "five");
  bst.insert(15, "fifteen");
  
  // Check existing keys
  EXPECT_EQ(bst.at(10), "ten");
  EXPECT_EQ(bst.at(5), "five");
  EXPECT_EQ(bst.at(15), "fifteen");
  
  // Check non-existing keys should throw
  EXPECT_THROW(bst.at(1), out_of_range);
  EXPECT_THROW(bst.at(20), out_of_range);
  
  // Modify using ref
  bst.at(10) = "TEN";
  EXPECT_EQ(bst.at(10), "TEN");
}

// Test clear
TEST(BSTMapCore, Clear) {
  BSTMap<int, string> bst;
  
  // Clear empty tree
  bst.clear();
  EXPECT_TRUE(bst.empty());
  EXPECT_EQ(bst.size(), 0);
  
  // Insert some elements
  bst.insert(10, "ten");
  bst.insert(5, "five");
  bst.insert(15, "fifteen");
  EXPECT_EQ(bst.size(), 3);
  
  // Clear tree
  bst.clear();
  EXPECT_TRUE(bst.empty());
  EXPECT_EQ(bst.size(), 0);
  EXPECT_FALSE(bst.contains(10));
  
  // Insert after clear
  bst.insert(20, "twenty");
  EXPECT_EQ(bst.size(), 1);
  EXPECT_TRUE(bst.contains(20));
}

// Test destructor (note: can't directly test but can check for memory leaks)
TEST(BSTMapCore, Destructor) {
  // Create and populate a tree
  {
    BSTMap<int, string> bst;
    for (int i = 0; i < 100; i++) {
      bst.insert(i, "value_" + to_string(i));
    }
    // Tree will be destroyed when out of scope
  }
  // If destructor has issues, address sani should catch it
}

// Test to_string
TEST(BSTMapCore, ToString) {
  BSTMap<int, string> bst;
  map<int, string> reference;
  
  // Empty tree
  EXPECT_EQ(bst.to_string(), "");
  
  // Insert elements
  vector<pair<int, string>> entries = {
    {10, "ten"},
    {5, "five"},
    {15, "fifteen"},
    {3, "three"},
    {7, "seven"},
    {12, "twelve"},
    {20, "twenty"}
  };
  
  for (const auto& [key, value] : entries) {
    bst.insert(key, value);
    reference[key] = value;
  }
  
  // Verify to_string output matches expected format
  EXPECT_EQ(bst.to_string(), map_to_string(reference));
}

// Test to_string with random data
TEST(BSTMapCore, ToStringRandom) {
  Random::seed(123);
  BSTMap<int, int> bst;
  map<int, int> reference;
  
  // Generate and insert random key-value pairs
  const int N = 50;
  for (int i = 0; i < N; i++) {
    int key = Random::randInt(1000);
    int value = Random::randInt(1000);
    
    bst.insert(key, value);
    reference[key] = value;
  }
  
  // Verify to_string output
  EXPECT_EQ(bst.to_string(), map_to_string(reference));
}

// Test copy constructor
TEST(BSTMapCore, CopyConstructor) {
  // Test with empty tree
  BSTMap<int, string> empty_bst;
  BSTMap<int, string> empty_copy(empty_bst);
  
  EXPECT_TRUE(empty_copy.empty());
  EXPECT_EQ(empty_copy.size(), 0);
  
  // Test with populated tree
  BSTMap<int, string> bst;
  vector<pair<int, string>> entries = {
    {10, "ten"},
    {5, "five"},
    {15, "fifteen"},
    {3, "three"},
    {7, "seven"},
    {12, "twelve"},
    {20, "twenty"}
  };
  
  for (const auto& [key, value] : entries) {
    bst.insert(key, value);
  }
  
  // Create a copy
  BSTMap<int, string> bst_copy(bst);
  
  // Check size
  EXPECT_EQ(bst_copy.size(), bst.size());
  
  // Check all elements exist with correct values
  for (const auto& [key, value] : entries) {
    EXPECT_TRUE(bst_copy.contains(key));
    EXPECT_EQ(bst_copy.at(key), value);
  }
  
  // Modify original, check that copy is unchanged
  bst.insert(30, "thirty");
  bst.at(10) = "TEN";
  
  EXPECT_FALSE(bst_copy.contains(30));
  EXPECT_EQ(bst_copy.at(10), "ten");
  
  // Check to_string output
  EXPECT_EQ(bst.to_string() != bst_copy.to_string(), true);
}

// Test combined operations
TEST(BSTMapCore, CombinedOperations) {
  Random::seed(42);
  BSTMap<int, string> bst;
  map<int, string> reference;
  
  // Insert random elements
  const int N = 50;
  for (int i = 0; i < N; i++) {
    int key = Random::randInt(1000);
    string value = "value_" + to_string(key);
    bst.insert(key, value);
    reference[key] = value;
  }
  
  // Check size and contents
  EXPECT_EQ(bst.size(), reference.size());
  
  for (const auto& [key, value] : reference) {
    EXPECT_TRUE(bst.contains(key));
    EXPECT_EQ(bst.at(key), value);
  }
  
  // Create a copy
  BSTMap<int, string> bst_copy(bst);
  
  // Check copy contents
  EXPECT_EQ(bst_copy.size(), bst.size());
  
  for (const auto& [key, value] : reference) {
    EXPECT_TRUE(bst_copy.contains(key));
    EXPECT_EQ(bst_copy.at(key), value);
  }
  
  // Clear the original
  bst.clear();
  EXPECT_TRUE(bst.empty());
  EXPECT_EQ(bst.size(), 0);
  
  // Check that copy is unaffected
  EXPECT_EQ(bst_copy.size(), reference.size());
  for (const auto& [key, value] : reference) {
    EXPECT_TRUE(bst_copy.contains(key));
    EXPECT_EQ(bst_copy.at(key), value);
  }
}

// Test assignment operator
TEST(BSTMapCore, AssignmentOperator) {
  // Create source maps
  BSTMap<int, string> source;
  source.insert(5, "five");
  source.insert(3, "three");
  source.insert(8, "eight");
  
  // Create an empty source map
  BSTMap<int, string> empty_source;
  
  // Test assignment from populated map to empty map
  {
    BSTMap<int, string> target;
    target = source;
    
    // Check size
    EXPECT_EQ(target.size(), source.size());
    EXPECT_EQ(target.size(), 3);
    
    // Check all elements exist with correct values
    EXPECT_TRUE(target.contains(5));
    EXPECT_TRUE(target.contains(3));
    EXPECT_TRUE(target.contains(8));
    EXPECT_EQ(target.at(5), "five");
    EXPECT_EQ(target.at(3), "three");
    EXPECT_EQ(target.at(8), "eight");
  }
  
  // Test assignment from populated map to populated map
  {
    BSTMap<int, string> target;
    target.insert(10, "ten");
    target.insert(20, "twenty");
    
    target = source;
    
    // Check size is exactly the size of source
    EXPECT_EQ(target.size(), source.size());
    EXPECT_EQ(target.size(), 3);
    
    // Check source elements exist with correct values
    EXPECT_TRUE(target.contains(5));
    EXPECT_TRUE(target.contains(3));
    EXPECT_TRUE(target.contains(8));
    
    // Check original elements don't exist anymore
    EXPECT_FALSE(target.contains(10));
    EXPECT_FALSE(target.contains(20));
  }
  
  // Test assignment from empty map to populated map
  {
    BSTMap<int, string> target;
    target.insert(7, "seven");
    target.insert(12, "twelve");
    
    target = empty_source;
    
    // Check the map is now empty
    EXPECT_TRUE(target.empty());
    EXPECT_EQ(target.size(), 0);
    EXPECT_FALSE(target.contains(7));
    EXPECT_FALSE(target.contains(12));
  }
  
  // Test assignment from empty map to empty map
  {
    BSTMap<int, string> target;
    
    target = empty_source;
    
    EXPECT_TRUE(target.empty());
    EXPECT_EQ(target.size(), 0);
  }
  
  // Test chained assignment
  {
    BSTMap<int, string> target1;
    BSTMap<int, string> target2;
    
    target1 = target2 = source;
    
    // Check both targets have correct size and elements
    EXPECT_EQ(target1.size(), 3);
    EXPECT_EQ(target2.size(), 3);
    EXPECT_TRUE(target1.contains(5));
    EXPECT_TRUE(target2.contains(5));
  }
}

// Test self-assignment properly 
TEST(BSTMapCore, SelfAssignmentProtection) {
  BSTMap<int, string> bst;
  bst.insert(1, "one");
  bst.insert(2, "two");
  
  // Store the original size
  size_t originalSize = bst.size();
  
  // Get a ref to the same object
  BSTMap<int, string>& bstRef = bst;
  
  // Perform assignment through the reference (same as self-assignment, note that bst = bst would cause errors as you would expect lol)
  bst = bstRef;
  
  // Check that nothing has changed
  EXPECT_EQ(bst.size(), originalSize);
  EXPECT_TRUE(bst.contains(1));
  EXPECT_TRUE(bst.contains(2));
  EXPECT_EQ(bst.at(1), "one");
  EXPECT_EQ(bst.at(2), "two");
}

// Test different key types
TEST(BSTMapCore, DifferentKeyTypes) {
  // Test with string keys
  BSTMap<string, int> string_bst;
  string_bst.insert("apple", 1);
  string_bst.insert("banana", 2);
  string_bst.insert("cherry", 3);
  
  EXPECT_EQ(string_bst.size(), 3);
  EXPECT_TRUE(string_bst.contains("apple"));
  EXPECT_TRUE(string_bst.contains("banana"));
  EXPECT_TRUE(string_bst.contains("cherry"));
  EXPECT_EQ(string_bst.at("apple"), 1);
  
  // Copy constructor with string keys
  BSTMap<string, int> string_copy(string_bst);
  EXPECT_EQ(string_copy.size(), 3);
  EXPECT_TRUE(string_copy.contains("cherry"));
  
  // Test with char keys
  BSTMap<char, string> char_bst;
  char_bst.insert('A', "Alpha");
  char_bst.insert('B', "Beta");
  char_bst.insert('C', "Charlie");
  
  EXPECT_EQ(char_bst.size(), 3);
  EXPECT_TRUE(char_bst.contains('B'));
  EXPECT_EQ(char_bst.at('C'), "Charlie");
  
  // Clear and check
  char_bst.clear();
  EXPECT_TRUE(char_bst.empty());
}

// Test with deep trees to ensure proper handling of tree structure
TEST(BSTMapCore, DeepTreeStructure) {
  BSTMap<int, string> bst;
  
  // Create a tree with a specific structure
  // Insert creating a "deep" tree
  bst.insert(50, "fifty");
  bst.insert(25, "twenty-five");
  bst.insert(75, "seventy-five");
  bst.insert(12, "twelve");
  bst.insert(37, "thirty-seven");
  bst.insert(63, "sixty-three");
  bst.insert(87, "eighty-seven");
  bst.insert(6, "six");
  bst.insert(18, "eighteen");
  bst.insert(31, "thirty-one");
  bst.insert(43, "forty-three");
  
  // Check the structure via contains and at
  EXPECT_EQ(bst.size(), 11);
  EXPECT_TRUE(bst.contains(6));
  EXPECT_TRUE(bst.contains(18));
  EXPECT_TRUE(bst.contains(87));
  EXPECT_EQ(bst.at(31), "thirty-one");
  EXPECT_EQ(bst.at(43), "forty-three");
  
  // Copy the tree
  BSTMap<int, string> bst_copy(bst);
  
  // Check that copy has the same structure
  EXPECT_EQ(bst_copy.size(), 11);
  EXPECT_TRUE(bst_copy.contains(6));
  EXPECT_TRUE(bst_copy.contains(18));
  EXPECT_TRUE(bst_copy.contains(87));
  EXPECT_EQ(bst_copy.at(31), "thirty-one");
  EXPECT_EQ(bst_copy.at(43), "forty-three");
  
  // Verify both to_string outputs match
  EXPECT_EQ(bst.to_string(), bst_copy.to_string());
}

// Test for edge cases in insert and contains
TEST(BSTMapCore, InsertAndContainsEdgeCases) {
  BSTMap<int, string> bst;
  
  // Insert and check extreme values
  bst.insert(INT_MAX, "max_int");
  bst.insert(INT_MIN, "min_int");
  
  EXPECT_TRUE(bst.contains(INT_MAX));
  EXPECT_TRUE(bst.contains(INT_MIN));
  EXPECT_EQ(bst.at(INT_MAX), "max_int");
  EXPECT_EQ(bst.at(INT_MIN), "min_int");
  
  // Try inserting duplicates
  bst.insert(INT_MAX, "different_max");
  EXPECT_EQ(bst.size(), 2); // Size shouldn't change
  EXPECT_EQ(bst.at(INT_MAX), "max_int"); // Val shouldn't be updated
}

// AUGMENTED TESTS
// Testing remove_min, begin, and next funcs, 85 point threshold

// Test remove_min function when the tree is empty
TEST(BSTMapAugmented, RemoveMinEmpty) {
  BSTMap<int, string> empty_bst;
  EXPECT_THROW(empty_bst.remove_min(), runtime_error);
}

// Test if remove_min properly updates the size
TEST(BSTMapAugmented, RemoveMinUpdatesSize) {
  BSTMap<int, string> bst;
  bst.insert(10, "ten");
  bst.insert(5, "five");
  
  size_t initial_size = bst.size();
  bst.remove_min();
  EXPECT_EQ(bst.size(), initial_size - 1);
  
  // Test multiple removals
  bst.insert(3, "three");
  bst.insert(7, "seven");
  initial_size = bst.size();
  
  bst.remove_min();
  EXPECT_EQ(bst.size(), initial_size - 1);
  
  bst.remove_min();
  EXPECT_EQ(bst.size(), initial_size - 2);
}

// Test if remove_min returns the correct pair
TEST(BSTMapAugmented, RemoveMinCorrectReturn) {
  BSTMap<int, string> bst;
  bst.insert(10, "ten");
  bst.insert(5, "five");
  bst.insert(15, "fifteen");
  bst.insert(3, "three");
  
  auto removed = bst.remove_min();
  EXPECT_EQ(removed.first, 3);
  EXPECT_EQ(removed.second, "three");
  
  removed = bst.remove_min();
  EXPECT_EQ(removed.first, 5);
  EXPECT_EQ(removed.second, "five");
}

// Test remove_min with different tree shapes
TEST(BSTMapAugmented, RemoveMinDifferentShapes) {
  // Left-skewed tree
  {
    BSTMap<int, string> left_skewed;
    left_skewed.insert(30, "thirty");
    left_skewed.insert(20, "twenty");
    left_skewed.insert(10, "ten");
    
    auto removed = left_skewed.remove_min();
    EXPECT_EQ(removed.first, 10);
    EXPECT_EQ(removed.second, "ten");
    EXPECT_EQ(left_skewed.size(), 2);
    EXPECT_FALSE(left_skewed.contains(10));
  }
  
  // Right-skewed tree
  {
    BSTMap<int, string> right_skewed;
    right_skewed.insert(10, "ten");
    right_skewed.insert(20, "twenty");
    right_skewed.insert(30, "thirty");
    
    auto removed = right_skewed.remove_min();
    EXPECT_EQ(removed.first, 10);
    EXPECT_EQ(removed.second, "ten");
    EXPECT_EQ(right_skewed.size(), 2);
    EXPECT_FALSE(right_skewed.contains(10));
  }

  // Balanced tree
  {
    BSTMap<int, string> balanced;
    balanced.insert(20, "twenty");
    balanced.insert(10, "ten");
    balanced.insert(30, "thirty");
    balanced.insert(5, "five");
    balanced.insert(15, "fifteen");
    balanced.insert(25, "twenty-five");
    balanced.insert(35, "thirty-five");
    
    auto removed = balanced.remove_min();
    EXPECT_EQ(removed.first, 5);
    EXPECT_EQ(removed.second, "five");
    EXPECT_EQ(balanced.size(), 6);
    EXPECT_FALSE(balanced.contains(5));
  }
}

// Test remove_min when removing the root node
TEST(BSTMapAugmented, RemoveMinRoot) {
  BSTMap<int, string> bst;
  bst.insert(5, "five");
  bst.insert(10, "ten");
  bst.insert(15, "fifteen");
  
  // Remove the root (which is also the min)
  auto removed = bst.remove_min();
  EXPECT_EQ(removed.first, 5);
  EXPECT_EQ(removed.second, "five");
  
  // Check that the tree structure is still valid
  EXPECT_TRUE(bst.contains(10));
  EXPECT_TRUE(bst.contains(15));
  EXPECT_EQ(bst.size(), 2);
  
  // Make sure we can still access values
  EXPECT_EQ(bst.at(10), "ten");
  EXPECT_EQ(bst.at(15), "fifteen");
}

// Test remove_min preserves children of the removed node
TEST(BSTMapAugmented, RemoveMinPreservesChildren) {
  BSTMap<int, string> bst;
  bst.insert(10, "ten");
  bst.insert(5, "five");
  bst.insert(3, "three");
  bst.insert(7, "seven");
  bst.insert(6, "six");
  bst.insert(8, "eight");
  
  // Remove the minimum node (3)
  bst.remove_min();
  
  // Check that other nodes are still accessible
  EXPECT_FALSE(bst.contains(3));
  EXPECT_TRUE(bst.contains(5));
  EXPECT_TRUE(bst.contains(6));
  EXPECT_TRUE(bst.contains(7));
  EXPECT_TRUE(bst.contains(8));
  EXPECT_TRUE(bst.contains(10));
  
  // Remove the new minimum node (5) which has children
  auto removed = bst.remove_min();
  EXPECT_EQ(removed.first, 5);
  
  // Ensure its children (6, 7, 8) are still in the tree
  EXPECT_TRUE(bst.contains(6));
  EXPECT_TRUE(bst.contains(7));
  EXPECT_TRUE(bst.contains(8));
  EXPECT_TRUE(bst.contains(10));
}

// Test begin and next with an empty tree
TEST(BSTMapAugmented, BeginNextEmpty) {
  BSTMap<int, string> empty_bst;
  
  // Test begin on empty tree
  empty_bst.begin();
  
  // Test next on empty tree
  int key;
  string value;
  EXPECT_FALSE(empty_bst.next(key, value));
}

// Test begin starts at the correct (leftmost) node
TEST(BSTMapAugmented, BeginStartsAtMin) {
  BSTMap<int, string> bst;
  bst.insert(10, "ten");
  bst.insert(5, "five");
  bst.insert(15, "fifteen");
  bst.insert(3, "three");
  bst.insert(7, "seven");
  
  bst.begin();
  
  int key;
  string value;
  EXPECT_TRUE(bst.next(key, value));
  
  // Should start with the minimum key (3)
  EXPECT_EQ(key, 3);
  EXPECT_EQ(value, "three");
}

// Test next returns false when done iterating
TEST(BSTMapAugmented, NextReturnsFalseWhenDone) {
  BSTMap<int, string> bst;
  bst.insert(1, "one");
  bst.insert(2, "two");
  
  bst.begin();
  
  int key;
  string value;
  
  // First call to next should return true
  EXPECT_TRUE(bst.next(key, value));
  EXPECT_EQ(key, 1);
  
  // Second call to next should return true
  EXPECT_TRUE(bst.next(key, value));
  EXPECT_EQ(key, 2);
  
  // Third call to next should return false (no more elements left)
  EXPECT_FALSE(bst.next(key, value));
}

// Test next visits all nodes in correct order
TEST(BSTMapAugmented, NextVisitsAllNodesInOrder) {
  BSTMap<int, string> bst;
  bst.insert(10, "ten");
  bst.insert(5, "five");
  bst.insert(15, "fifteen");
  bst.insert(3, "three");
  bst.insert(7, "seven");
  bst.insert(12, "twelve");
  bst.insert(20, "twenty");
  
  // Expected traversal order: 3, 5, 7, 10, 12, 15, 20 (ascending order)
  vector<int> expected_keys = {3, 5, 7, 10, 12, 15, 20};
  vector<string> expected_values = {"three", "five", "seven", "ten", "twelve", "fifteen", "twenty"};
  
  bst.begin();
  
  int key;
  string value;
  int index = 0;
  
  // Collect all nodes via next() and verify they're in order
  while (bst.next(key, value)) {
    EXPECT_LT(index, expected_keys.size()); // Make sure not visiting too many nodes
    EXPECT_EQ(key, expected_keys[index]);
    EXPECT_EQ(value, expected_values[index]);
    index++;
  }
  
  // Verify visited all nodes
  EXPECT_EQ(index, expected_keys.size());
}

// Test next doesn't skip any nodes
TEST(BSTMapAugmented, NextDoesntSkipNodes) {
  BSTMap<int, string> bst;
  
  // Create a left-skewed tree
  bst.insert(30, "thirty");
  bst.insert(20, "twenty");
  bst.insert(10, "ten");
  
  vector<int> expected_keys = {10, 20, 30};
  
  bst.begin();
  
  int key;
  string value;
  int count = 0;
  
  while (bst.next(key, value)) {
    EXPECT_EQ(key, expected_keys[count]);
    count++;
  }
  
  // Make sure visited exactly 3 nodes
  EXPECT_EQ(count, 3);
  
  // Create a right-skewed tree
  BSTMap<int, string> bst2;
  bst2.insert(10, "ten");
  bst2.insert(20, "twenty");
  bst2.insert(30, "thirty");
  
  vector<int> expected_keys2 = {10, 20, 30};
  
  bst2.begin();
  
  count = 0;
  
  while (bst2.next(key, value)) {
    EXPECT_EQ(key, expected_keys2[count]);
    count++;
  }
  
  // Make sure visited exactly 3 nodes
  EXPECT_EQ(count, 3);
}

// Test next doesn't revisit same nodes
TEST(BSTMapAugmented, NextDoesntRevisitNodes) {
  BSTMap<int, string> bst;
  bst.insert(10, "ten");
  bst.insert(5, "five");
  bst.insert(15, "fifteen");
  
  bst.begin();
  
  int key;
  string value;
  set<int> visited_keys;
  
  // Collect all keys visited by next()
  while (bst.next(key, value)) {
    // Make sure this key hasn't been visited before
    EXPECT_EQ(visited_keys.count(key), 0);
    visited_keys.insert(key);
  }
  
  // Verify visited each key exactly once
  EXPECT_EQ(visited_keys.size(), 3);
  EXPECT_EQ(visited_keys.count(5), 1);
  EXPECT_EQ(visited_keys.count(10), 1);
  EXPECT_EQ(visited_keys.count(15), 1);
}

// ERASE TESTS
// Test that erase throws when the key isn't in the map
TEST(BSTMapErase, EraseThrowsWhenKeyNotFound) {
  BSTMap<int, string> bst;
  bst.insert(10, "ten");
  bst.insert(5, "five");
  bst.insert(15, "fifteen");
  
  // Try to erase a key that doesn't exist
  EXPECT_THROW(bst.erase(20), out_of_range);
  EXPECT_THROW(bst.erase(7), out_of_range);
  
  // Empty map should throw too
  BSTMap<int, string> empty_bst;
  EXPECT_THROW(empty_bst.erase(1), out_of_range);
}

// Test that erase preserves descendents of the removed node
TEST(BSTMapErase, ErasePreservesDescendents) {
  BSTMap<int, string> bst;
  bst.insert(10, "ten");
  bst.insert(5, "five");
  bst.insert(15, "fifteen");
  bst.insert(3, "three");
  bst.insert(7, "seven");
  bst.insert(12, "twelve");
  bst.insert(17, "seventeen");
  
  // Erase a node with children
  bst.erase(5);
  
  // Check that children of the erased node are still in the tree
  EXPECT_TRUE(bst.contains(3));
  EXPECT_TRUE(bst.contains(7));
  
  // Check that other nodes are still in the tree
  EXPECT_TRUE(bst.contains(10));
  EXPECT_TRUE(bst.contains(15));
  EXPECT_TRUE(bst.contains(12));
  EXPECT_TRUE(bst.contains(17));
  
  // Check that we can still access the values
  EXPECT_EQ(bst.at(3), "three");
  EXPECT_EQ(bst.at(7), "seven");
}

// Test erasing the root node with no children
TEST(BSTMapErase, EraseRootWithNoChildren) {
  BSTMap<int, string> bst;
  bst.insert(10, "ten");
  
  // Erase the root (only node)
  auto val = bst.erase(10);
  
  // Check the return value
  EXPECT_EQ(val, "ten");
  
  // Check that the tree is now empty
  EXPECT_TRUE(bst.empty());
  EXPECT_EQ(bst.size(), 0);
  
  // Try inserting a new node after erasing the root
  bst.insert(5, "five");
  EXPECT_EQ(bst.size(), 1);
  EXPECT_TRUE(bst.contains(5));
}

// Test erasing the root node with one child
TEST(BSTMapErase, EraseRootWithOneChild) {
  // Test with left child
  {
    BSTMap<int, string> bst;
    bst.insert(10, "ten");
    bst.insert(5, "five");
    
    // Erase the root
    auto val = bst.erase(10);
    
    // Check the return value
    EXPECT_EQ(val, "ten");
    
    // Check that the tree structure is valid
    EXPECT_EQ(bst.size(), 1);
    EXPECT_TRUE(bst.contains(5));
    EXPECT_FALSE(bst.contains(10));
  }
  
  // Test with right child
  {
    BSTMap<int, string> bst;
    bst.insert(10, "ten");
    bst.insert(15, "fifteen");
    
    // Erase the root
    auto val = bst.erase(10);
    
    // Check the return value
    EXPECT_EQ(val, "ten");
    
    // Check that the tree structure is valid
    EXPECT_EQ(bst.size(), 1);
    EXPECT_TRUE(bst.contains(15));
    EXPECT_FALSE(bst.contains(10));
  }
}

// Test erasing the root node with two children
TEST(BSTMapErase, EraseRootWithTwoChildren) {
  BSTMap<int, string> bst;
  bst.insert(10, "ten");
  bst.insert(5, "five");
  bst.insert(15, "fifteen");
  bst.insert(3, "three");
  bst.insert(7, "seven");
  bst.insert(12, "twelve");
  bst.insert(17, "seventeen");
  
  // Erase the root
  auto val = bst.erase(10);
  
  // Check the return value
  EXPECT_EQ(val, "ten");
  
  // Check that all other nodes are still in the tree
  EXPECT_FALSE(bst.contains(10));
  EXPECT_TRUE(bst.contains(5));
  EXPECT_TRUE(bst.contains(15));
  EXPECT_TRUE(bst.contains(3));
  EXPECT_TRUE(bst.contains(7));
  EXPECT_TRUE(bst.contains(12));
  EXPECT_TRUE(bst.contains(17));
  
  // Tree should have correct size
  EXPECT_EQ(bst.size(), 6);
}

// Test erasing a node with two children where the successor has children
TEST(BSTMapErase, EraseNodeWithTwoChildrenSuccessorHasChildren) {
  BSTMap<int, string> bst;
  bst.insert(10, "ten");
  bst.insert(5, "five");
  bst.insert(20, "twenty");
  bst.insert(15, "fifteen");
  bst.insert(30, "thirty");
  bst.insert(12, "twelve");
  bst.insert(17, "seventeen");
  bst.insert(16, "sixteen");
  bst.insert(18, "eighteen");
  
  // In this case, erasing 10 should use 12 as its successor,
  // but 15 has child (17)
  auto val = bst.erase(10);
  
  // Check return value
  EXPECT_EQ(val, "ten");
  
  // Check that tree is still valid
  EXPECT_FALSE(bst.contains(10));
  EXPECT_TRUE(bst.contains(5));
  EXPECT_TRUE(bst.contains(20));
  EXPECT_TRUE(bst.contains(15));
  EXPECT_TRUE(bst.contains(30));
  EXPECT_TRUE(bst.contains(12));
  EXPECT_TRUE(bst.contains(17));
  EXPECT_TRUE(bst.contains(16));
  EXPECT_TRUE(bst.contains(18));
  
  // should still be able to access all values
  EXPECT_EQ(bst.at(17), "seventeen");
  EXPECT_EQ(bst.at(15), "fifteen");
  
  // Size should be dec by 1
  EXPECT_EQ(bst.size(), 8);
  
  // test the reverse situation, i.e: erase a node where successor is the right child and has children
  val = bst.erase(15);
  
  // Check return val
  EXPECT_EQ(val, "fifteen");
  
  // Check that tree is still valid
  EXPECT_FALSE(bst.contains(10));
  EXPECT_FALSE(bst.contains(15));
  EXPECT_TRUE(bst.contains(5));
  EXPECT_TRUE(bst.contains(20));
  EXPECT_TRUE(bst.contains(30));
  EXPECT_TRUE(bst.contains(12));
  EXPECT_TRUE(bst.contains(17));
  EXPECT_TRUE(bst.contains(16));
  EXPECT_TRUE(bst.contains(18));
  
  // Size should be dec by 1 again
  EXPECT_EQ(bst.size(), 7);
}

// Test erasing a node with two children where the successor is not the right child
TEST(BSTMapErase, EraseNodeWithTwoChildrenSuccessorNotRightChild) {
  BSTMap<int, string> bst;
  bst.insert(20, "twenty");
  bst.insert(10, "ten");
  bst.insert(30, "thirty");
  bst.insert(5, "five");
  bst.insert(15, "fifteen");
  bst.insert(25, "twenty-five");
  bst.insert(40, "forty");
  bst.insert(12, "twelve");
  bst.insert(17, "seventeen");
  bst.insert(22, "twenty-two");
  bst.insert(27, "twenty-seven");
  
  // When erasing 20, the successor is 22 (not the right child 30)
  auto val = bst.erase(20);
  
  // Check return value
  EXPECT_EQ(val, "twenty");
  
  // Check that tree is still valid
  EXPECT_FALSE(bst.contains(20));
  EXPECT_TRUE(bst.contains(10));
  EXPECT_TRUE(bst.contains(30));
  EXPECT_TRUE(bst.contains(5));
  EXPECT_TRUE(bst.contains(15));
  EXPECT_TRUE(bst.contains(25));
  EXPECT_TRUE(bst.contains(40));
  EXPECT_TRUE(bst.contains(12));
  EXPECT_TRUE(bst.contains(17));
  EXPECT_TRUE(bst.contains(22));
  EXPECT_TRUE(bst.contains(27));
  
  // We should still be able to access all values
  EXPECT_EQ(bst.at(22), "twenty-two");
  EXPECT_EQ(bst.at(27), "twenty-seven");
  
  // Size should be dec by 1
  EXPECT_EQ(bst.size(), 10);
  
  // Now try another node with the successor not being the right child
  val = bst.erase(10);
  
  // Check return value
  EXPECT_EQ(val, "ten");
  
  // Check that the tree is still valid
  EXPECT_FALSE(bst.contains(20));
  EXPECT_FALSE(bst.contains(10));
  EXPECT_TRUE(bst.contains(30));
  EXPECT_TRUE(bst.contains(5));
  EXPECT_TRUE(bst.contains(15));
  EXPECT_TRUE(bst.contains(25));
  EXPECT_TRUE(bst.contains(40));
  EXPECT_TRUE(bst.contains(12));
  EXPECT_TRUE(bst.contains(17));
  EXPECT_TRUE(bst.contains(22));
  EXPECT_TRUE(bst.contains(27));
  
  // Size should be dec by 1 again
  EXPECT_EQ(bst.size(), 9);
}

// Test erasing nodes in all three cases: leaf, one child, two children
TEST(BSTMapErase, EraseAllCases) {
  BSTMap<int, string> bst;
  bst.insert(20, "twenty");
  bst.insert(10, "ten");
  bst.insert(30, "thirty");
  bst.insert(5, "five");
  bst.insert(15, "fifteen");
  bst.insert(25, "twenty-five");
  bst.insert(35, "thirty-five");
  
  // Case 1: Erase a leaf node
  auto val = bst.erase(5);
  EXPECT_EQ(val, "five");
  EXPECT_FALSE(bst.contains(5));
  EXPECT_EQ(bst.size(), 6);
  
  // Case 2: Erase a node with one child
  val = bst.erase(30);
  EXPECT_EQ(val, "thirty");
  EXPECT_FALSE(bst.contains(30));
  EXPECT_TRUE(bst.contains(35));
  EXPECT_EQ(bst.size(), 5);
  
  // Case 3: Erase a node with two children
  val = bst.erase(20);
  EXPECT_EQ(val, "twenty");
  EXPECT_FALSE(bst.contains(20));
  EXPECT_TRUE(bst.contains(10));
  EXPECT_TRUE(bst.contains(15));
  EXPECT_TRUE(bst.contains(25));
  EXPECT_TRUE(bst.contains(35));
  EXPECT_EQ(bst.size(), 4);
}

// Test specifically targeting the case where successor has children
TEST(BSTMapErase, EraseSuccessorWithChildren) {
  BSTMap<int, string> bst;
  
  // Create a specific tree structure where we can test the successor-with-children case
  bst.insert(20, "twenty");      //        20
  bst.insert(10, "ten");         //       /  
  bst.insert(30, "thirty");      //     10    30
  bst.insert(25, "twenty-five"); //          /
  bst.insert(28, "twenty-eight");//        25
  bst.insert(27, "twenty-seven");//          
                                 //           28
                                 //          /
                                 //         27
  
  // Record the original size
  size_t original_size = bst.size();
  
  // When erasing 20, the successor is 25, and 25 has a right child (28) which has a left child (27)
  // This tests case where the successor has a non-zero subtree (non-trivial which is lin alg terminology)
  string val = bst.erase(20);
  
  // Check basic expectations
  EXPECT_EQ(val, "twenty");
  EXPECT_FALSE(bst.contains(20));
  EXPECT_EQ(bst.size(), original_size - 1);
  
  // Verify all other nodes are still present and accessible
  EXPECT_TRUE(bst.contains(10));
  EXPECT_TRUE(bst.contains(30));
  EXPECT_TRUE(bst.contains(25));
  EXPECT_TRUE(bst.contains(28));
  EXPECT_TRUE(bst.contains(27));
  
  // Check that we can still access values
  EXPECT_EQ(bst.at(25), "twenty-five");
  EXPECT_EQ(bst.at(28), "twenty-eight");
  EXPECT_EQ(bst.at(27), "twenty-seven");
  
  // Further test the tree structure by traversing it
  vector<int> expected_keys = {10, 25, 27, 28, 30};
  vector<string> expected_values = {"ten", "twenty-five", "twenty-seven", "twenty-eight", "thirty"};
  
  bst.begin();
  int key;
  string value;
  int index = 0;
  
  while (bst.next(key, value)) {
    EXPECT_EQ(key, expected_keys[index]);
    EXPECT_EQ(value, expected_values[index]);
    index++;
  }
  
  // Make sure all expected keys are seen
  EXPECT_EQ(index, expected_keys.size());
  
  // erase a node that will use a successor with even more "complex" children
  val = bst.erase(25);
  EXPECT_EQ(val, "twenty-five");
  EXPECT_FALSE(bst.contains(25));
  EXPECT_TRUE(bst.contains(27));
  EXPECT_TRUE(bst.contains(28));
  
  // Final tree should be in correct order
  expected_keys = {10, 27, 28, 30};
  expected_values = {"ten", "twenty-seven", "twenty-eight", "thirty"};
  
  bst.begin();
  index = 0;
  
  while (bst.next(key, value)) {
    EXPECT_EQ(key, expected_keys[index]);
    EXPECT_EQ(value, expected_values[index]);
    index++;
  }
  
  // Make sure we've seen all expected keys
  EXPECT_EQ(index, expected_keys.size());
}

}  // namespace

//passed all tests: 100/100 :) 