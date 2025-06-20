#pragma once

#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <utility>

using namespace std;

//CS 251 Project 5: Hashmap
//Authored by: Akbar Aman
//Note CORE functionality of HashMap will most likely be relevant material on midterm 2

template <typename KeyT, typename ValT>
class HashMap {
 private:
  struct ChainNode {
    const KeyT key;
    ValT value;
    ChainNode* next;

    ChainNode(KeyT key, ValT value) : key(key), value(value), next(nullptr) {
    }

    ChainNode(KeyT key, ValT value, ChainNode* next) : key(key), value(value), next(next) {
    }
  };

  ChainNode** data;
  size_t sz;
  size_t capacity;

  // Utility members for begin/next
  ChainNode* curr;
  size_t curr_idx;

  // Helper function to initialize the data array with nullptrs
  void initialize_data() {
    for (size_t i = 0; i < capacity; i++) {
      data[i] = nullptr;
    }
  }

  // Helper function to determine the bucket index for a key
  size_t get_bucket_index(const KeyT& key) const {
    return hash<KeyT>{}(key) % capacity;
  }

  // Helper function to check if resizing is needed
  bool should_resize() const {
    return (static_cast<double>(sz) / static_cast<double>(capacity)) > 1.5; //flag for resizing
  }

  // Helper function to resize the hashmap
  void resize() {
    size_t old_capacity = capacity;
    ChainNode** old_data = data;
    
    // Double the capacity 
    capacity *= 2;
    
    // Allocate new data array
    data = new ChainNode*[capacity];
    initialize_data();
    
    // Rehash all existing nodes
    for (size_t i = 0; i < old_capacity; i++) {
      ChainNode* curr = old_data[i];
      while (curr != nullptr) {
        ChainNode* next = curr->next;
        
        // Calculate new bucket index
        size_t new_idx = get_bucket_index(curr->key);
        
        // Insert at beginning of new chain
        curr->next = data[new_idx];
        data[new_idx] = curr;
        
        curr = next;
      }
    }
    
    // Free old array
    delete[] old_data;
  }

 public:
  
 /**
 * Constructor: creates an empty HashMap with a specified initial capacity.
 * 
 * These constructors initialize the hash tables to store key-val pairs.
 * It allocates an array of ChainNode pointers with the given capacity,
 * initializes all buckets to nullptr (empty chains), and sets the size to 0.
 * It also initializes the iter-related members (curr and curr_idx).
 * 
 * Time Complexity: O(capacity) due to initializing all bucket pointers to nullptr
 */
 
  //10 buckets
  HashMap() {
    capacity = 10;
    sz = 0;
    data = new ChainNode*[capacity];
    initialize_data();
    curr = nullptr;
    curr_idx = 0;
  }

  //capacity buckets
  HashMap(size_t capacity) {
    this->capacity = capacity;
    sz = 0;
    data = new ChainNode*[capacity];
    initialize_data();
    curr = nullptr;
    curr_idx = 0;
  }

  /** 
   * compares the current size (# elements) to 0.
   * Returns true if there are no elements, false otherwise.
   * 
   * Checks if the `HashMap` is empty/no mappings. Runs in O(1).
   */
  bool empty() const {
    return sz == 0;
  }

  /**
   * Returns # of key-value mappings in the `HashMap`. Runs in O(1).
   * 
   * returns the internal size counter that tracks how many
   * elements have been inserted (and not removed)
   */
  size_t size() const {
    return sz;
  }

  /**
   * Adds the mapping `{key -> value}` to the `HashMap`. If the key already
   * exists, does not update the mapping (like the C++ STL map).
   * 
   * First calculate the bucket index using the hash function.
   * Then check if the key already exists in that chain, if so, does nothing.
   * If the key DNE, it checks if adding this element would exceed
   * the load factor threshold of 1.5 after adding element. If so, it resizes the hash table.
   * Finally, creates new node containing the key-val pair and inserts it
   * at the beginning of the appropriate chain.
   *
   * Uses the hash value of the key to determine the location in the
   * underlying hash table. Creates exactly one new node; resizes by doubling
   * when the load factor exceeds 1.5.
   *
   * On resize, doesn't create new nodes, but rearranges existing ones.
   *
   * Runs in O(L), where L is the length of the longest chain.
   * If resize occurs: O(N + capacity) where N is total number of elements
   */
  void insert(KeyT key, ValT value) {
    // Calculate bucket index
    size_t idx = get_bucket_index(key);
    
    // Check if key already exists
    ChainNode* curr = data[idx];
    while (curr != nullptr) {
      if (curr->key == key) {
        // Key already exists, do not update
        return;
      }
      curr = curr->next;
    }
    
    // Check if resize is needed AFTER adding this new element(I had it as before previously which was wrong)
    if ((static_cast<double>(sz + 1) / static_cast<double>(capacity)) > 1.5) {
      resize();
      // Recalculate index after resize since capacity changed
      idx = get_bucket_index(key);
    }
    
    // Key DNE, insert new node at beginning of chain
    ChainNode* new_node = new ChainNode(key, value, data[idx]);
    data[idx] = new_node;
    sz++;
}

  /**
   * Return a reference to the value stored for `key` in the map.
   *
   * If key is not present in the map, throw `out_of_range` exception.
   * 
   * Calculates the bucket index using the hash function.
   * Then traverses the chain at that bucket, comparing each key.
   * If found, returns a ref to the associated val.
   * If not found, throws said exception.
   *
   * Runs in O(L), where L is the length of the longest chain.
   */
  ValT& at(const KeyT& key) const {
    size_t idx = get_bucket_index(key);
    ChainNode* curr = data[idx];
    
    while (curr != nullptr) {
      if (curr->key == key) {
        return curr->value;
      }
      curr = curr->next;
    }
    
    // Key not found
    throw out_of_range("Key not found in HashMap");
  }

  /**
   * Returns `true` if the key is present in the map, and false otherwise.
   * 
   * Calculates the bucket index using the hash function.
   * Then traverses the chain at that bucket, comparing each key.
   * Returns true if the key is found, false otherwise.
   *
   * Runs in O(L), where L is the length of the longest chain.
   */
  bool contains(const KeyT& key) const {
    size_t idx = get_bucket_index(key);
    ChainNode* curr = data[idx];
    
    while (curr != nullptr) {
      if (curr->key == key) {
        return true;
      }
      curr = curr->next;
    }
    
    return false;
  }

  /**
   * Empties the `HashMap`, freeing all nodes. The bucket array may be left
   * alone.
   *
   * Iterates through each bucket in the array.
   * For each bucket, traverses its chain and deletes each node.
   * Resets all bucket pointers to nullptr and sets size to 0.
   * The capacity remains unchanged.
   * 
   * Runs in O(N+B), where N is the number of mappings and B is the number of
   * buckets.
   */
  void clear() {
    for (size_t i = 0; i < capacity; i++) {
      ChainNode* curr = data[i];
      while (curr != nullptr) {
        ChainNode* next = curr->next;
        delete curr;
        curr = next;
      }
      data[i] = nullptr;
    }
    sz = 0;
  }

  /**
   * Destructor, cleans up the `HashMap`.
   * 
   * First calls clear() to delete all nodes in the hash table.
   * Then deletes the array of bucket pointers itself.
   *
   * Runs in O(N+B), where N is the number of mappings and B is the number of
   * buckets.
   */
  ~HashMap() {
    clear();
    delete[] data;
  }

  /**
   * Removes the mapping for the given key from the `HashMap`, and returns the
   * value.
   *
   * Throws `out_of_range` if the key is not present in the map. Creates no new
   * nodes, and does not update the key or value of any existing nodes.
   * 
   * Calculates the bucket index using the hash function.
   * Traverses the chain at that bucket, tracking both current and previous nodes.
   * If the key is found, removes the node from the chain (handles cases where
   * the node is at the start, middle, or end of the chain), deletes it,
   * decrements the size, and returns the val.
   *
   * Runs in O(L), where L is the length of the longest chain.
   */
  ValT erase(const KeyT& key) {
    size_t idx = get_bucket_index(key);
    ChainNode* curr = data[idx];
    ChainNode* prev = nullptr;
    
    while (curr != nullptr) {
      if (curr->key == key) {
        // Found the key
        ValT value = curr->value;
        
        // Remove node from chain
        if (prev == nullptr) {
          // Node is at the beginning of the chain
          data[idx] = curr->next;
        } else {
          // Node is in the middle or end of the chain
          prev->next = curr->next;
        }
        
        // Free memory
        delete curr;
        sz--;
        
        return value;
      }
      
      prev = curr;
      curr = curr->next;
    }
    
    // Key not found
    throw out_of_range("Key not found in HashMap");
  }

  /**
   * Copy constructor.
   *
   * Copies the mappings from the provided `HashMap`.
   * 
   * Creates a new `HashMap` as a copy of an existing one.
   * Initializes this HashMap with the same capacity as the source.
   * Iterates through all mappings in the source HashMap and inserts
   * them into this HashMap using the insert method.
   * 
   * Note that its possible using insert() may cause resizing if the load factor exceeds 1.5
   *
   * Runs in O(N+B), where N is the number of mappings in `other`, and B is the
   * number of buckets.
   */
  HashMap(const HashMap& other) {
    // Initialize this HashMap
    capacity = other.capacity;
    sz = 0;  // Will be incremented during insertions
    data = new ChainNode*[capacity];
    initialize_data();
    curr = nullptr;
    curr_idx = 0;
    
    // Copy all mappings from other
    for (size_t i = 0; i < other.capacity; i++) {
      ChainNode* curr = other.data[i];
      while (curr != nullptr) {
        insert(curr->key, curr->value);
        curr = curr->next;
      }
    }
  }

  /**
   * Assignment operator; `operator=`.
   *
   * Clears this table, and copies the mappings from the provided `HashMap`.
   * 
   * First checks for self-assignment to avoid unnecessary work.
   * Clears this HashMap to remove existing mappings.
   * If capacities differ, reallocates the data array.
   * Copies all mappings from the other HashMap into this one
   * using the insert method.
   *
   * Runs in O((N1+B1) + (N2+B2)), where N1 and B1 are the number of mappings
   * and buckets in `this`, and N2 and B2 are the number of mappings and buckets
   * in `other`.
   */
  HashMap& operator=(const HashMap& other) {
    // Check for self-assignment
    if (this == &other) {
      return *this;
    }
    
    // Clear existing data
    clear();
    
    // If capacities differ, reallocate data array
    if (capacity != other.capacity) {
      delete[] data;
      capacity = other.capacity;
      data = new ChainNode*[capacity];
      initialize_data();
    }
    
    // Copy all mappings from other
    for (size_t i = 0; i < other.capacity; i++) {
      ChainNode* curr = other.data[i];
      while (curr != nullptr) {
        insert(curr->key, curr->value);
        curr = curr->next;
      }
    }
    
    return *this;
  }

  

  /**
   * Resets internal state for an iterative traversal.
   *
   * See `next` for usage details. Modifies nothing except for `curr` and
   * `curr_idx`.
   *
   * Runs in worst-case O(B), where B is the number of buckets.
   */
  void begin() {
    curr_idx = 0;
    curr = nullptr;
    
    // Find the first non-empty bucket
    while (curr_idx < capacity && data[curr_idx] == nullptr) {
      curr_idx++;
    }
    
    // If a non-empty bucket is found, set curr to the first node
    if (curr_idx < capacity) {
      curr = data[curr_idx];
    }
  }

  /**
   * Uses the internal state to return the "next" key and value
   * by reference, and advances the internal state. Returns `true` if the
   * reference parameters were set, and `false` otherwise.
   *
   * Example usage:
   *
   * ```c++
   * HashMap<string, int> hm;
   * hm.begin();
   * string key;
   * int value;
   * while (hm.next(key, val)) {
   *   cout << key << ": " << val << endl;
   * }
   * ```
   *
   * Does not visit the mappings in any defined order.
   *
   * Modifies nothing except for `curr` and `curr_idx`.
   *
   * Runs in worst-case O(B) where B is the number of buckets.
   */
  bool next(KeyT& key, ValT& value) {
    // Check if we've reached the end
    if (curr == nullptr) {
      return false;
    }
    
    // Get current key and value
    key = curr->key;
    value = curr->value;
    
    // Move to next node
    curr = curr->next;
    
    // If end of current chain, find next non-empty bucket
    if (curr == nullptr) {
      curr_idx++;
      while (curr_idx < capacity && data[curr_idx] == nullptr) {
        curr_idx++;
      }
      
      // If we found a non-empty bucket, set curr to the first node
      if (curr_idx < capacity) {
        curr = data[curr_idx];
      }
    }
    
    return true;
  }

  // ===============================================

  /**
   * Returns a pointer to the underlying memory managed by the `HashMap`.
   * For autograder testing purposes only.
   */
  void* get_data() const {
    return this->data;
  }

  /**
   * Returns the capacity of the underlying memory managed by the `HashMap`. For
   * autograder testing purposes only.
   */
  size_t get_capacity() {
    return this->capacity;
  }
};