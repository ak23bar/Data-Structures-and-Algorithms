#pragma once

#include <iostream>
#include <sstream>
#include <string>
#include <stdexcept>

// CS 251: Project 3
// CircVector: circular array with dynamic malloc
// Authored by Akbar Aman

using namespace std;

template <typename T>
class CircVector {
 private:
  T *data;
  size_t vec_size;
  size_t capacity;
  size_t front_idx;

  // Private helper functions
  
  // Convert logical index to physical index (Wrapping Function)
  size_t wrap(size_t index) const {
    return (front_idx + index) % capacity;
  }

  // Resize the vector when it's full
  void resize() {
    size_t new_capacity = capacity * 2;
    T* new_data = new T[new_capacity];
  
    // Copy elements to the new array, adjusting for circular indexing
    for (size_t i = 0; i < vec_size; i++) {
      new_data[i] = data[(front_idx + i) % capacity];
    }
  
    delete[] data;
    data = new_data;
    front_idx = 0;
    capacity = new_capacity;
  }
  

 public:
  
 /**
   * Default constructor. Creates an empty `CircVector` with capacity 10.
   */
  CircVector() {
    capacity = 10;
    data = new T[capacity];
    vec_size = 0;
    front_idx = 0;
  }

  /**
   * Creates an empty `CircVector` with given capacity. Capacity must > 0.
   */
  CircVector(size_t capacity) {
    this->capacity = capacity;
    data = new T[capacity];
    vec_size = 0;
    front_idx = 0;
  }

  /**
   * Returns whether the `CircVector` is empty (i.e. whether its
   * size is 0).
   */
  bool empty() const {
    return vec_size == 0;
  }

  /**
   * Returns the number of elements in the `CircVector`.
   */
  size_t size() const {
    return vec_size;
  }

  /**
   * Adds the given `T` to the front of the `CircVector`.
   */
  void push_front(T data_elem) {
    if (vec_size == capacity) 
     resize();  // Resize if necessary

    // Decrement front_idx with wraparound
    if (front_idx == 0)
      front_idx = capacity - 1;
    else
      front_idx = front_idx - 1;
    
    data[front_idx] = data_elem;
    vec_size++;
  }
  

  /**
   * Adds the given `T` to the back of the `CircVector`.
   */
  void push_back(T data_elem) {
    if (vec_size == capacity){ 
     resize();  // Resize if necessary
    }
    size_t back_idx = (front_idx + vec_size) % capacity;
    data[back_idx] = data_elem;
    vec_size++;
  }
  
  /**
   * Removes the element at the front of the `CircVector`.
   *
   * If the `CircVector` is empty, throw error
   */
  T pop_front() {
    if (empty()) {
     throw runtime_error("Vector is empty");
    }
    T value = data[front_idx];
    front_idx = (front_idx + 1) % capacity;
    vec_size--;
    return value;
  }
  

  /**
   * Removes the element at the back of the `CircVector`.
   *
   * If the `CircVector` is empty, throw error
   */
  T pop_back() {
    if (empty()) { 
     throw runtime_error("Vector is empty");
    }
    size_t back_idx = (front_idx + vec_size - 1) % capacity;
    T value = data[back_idx];
    vec_size--;
    return value;
  }
  
  /**
   * Removes all elements from the `CircVector`.
   */
  void clear() {
    vec_size = 0;
    front_idx = 0;
  }

  /**
   * Destructor. Clears all allocated memory.
   */
  virtual ~CircVector() {
    delete[] data;
  }

  /**
   * Returns the element at the given index in the `CircVector`.
   *
   * If the index is invalid, throw error
   */
  T& at(size_t index) const {
    if (index >= vec_size) {
      throw out_of_range("Index out of range");
    }
    size_t idx = (front_idx + index) % capacity;
    return data[idx];
  }
  
  
  /**
   * Copy constructor. Creates a deep copy of the given `CircVector`.
   *
   * Must run in O(N) time.
   */
 
   // Copy constructor; creates deep copy of the given CircVec
 CircVector(const CircVector &other) {
  capacity = other.capacity;
  vec_size = other.vec_size;
  front_idx = other.front_idx;  // Keep the same front_idx

  // alloc new memory
  data = new T[capacity];

  // Copy elements 
  for (size_t i = 0; i < vec_size; i++) {
      size_t src_idx = other.wrap(i);
      size_t dest_idx = wrap(i);
      data[dest_idx] = other.data[src_idx];
  }
}

  /**
   * Assignment operator. Sets the current `CircVector` to a deep copy of the
   * given `CircVector`.
   *
   * Must run in O(N) time.
   */
  
  // Assignment operator; sets the current CircVec to a deep copy of the given CircVec
  CircVector &operator=(const CircVector &other) {
    if (this != &other) {
        // Clean up old data
        delete[] data;
  
        // Copy new values
        capacity = other.capacity;
        vec_size = other.vec_size;
        front_idx = other.front_idx;
        data = new T[capacity];
  
        // Copy elements in the same circular structure
        for (size_t i = 0; i < vec_size; i++) {
            size_t src_idx = other.wrap(i);
            size_t dest_idx = wrap(i);
            data[dest_idx] = other.data[src_idx];
        }
    }
    return *this;
  }


  /**
   * Converts the `CircVector` to a string. Formatted like `[0, 1, 2, 3, 4]`
   * (without the backticks -- hover the function name to see). Runs in O(N)
   * time.
   */
  string to_string() const {
    stringstream ss;
    ss << "[";
    if (vec_size > 0) {
      for (size_t i = 0; i < vec_size - 1; i++) {
        ss << data[(front_idx + i) % capacity] << ", ";
      }
      ss << data[(front_idx + vec_size - 1) % capacity];
    }
    ss << "]";
    return ss.str();
  }
  

  /**
   * Searches the `CircVector` for the first matching element, and returns its
   * index in the `CircVector`. If no match is found, returns "-1".
   */
  size_t find(const T &target) {
    for (size_t i = 0; i < vec_size; i++) {
      if (at(i) == target) {
        return i;
      }
    }
    return -1;  // Return -1 
                      
  }

  /**
   * Remove the element at the specified index in this list.
   *
   * If the index is invalid, do nothing
   */
  void remove_at(size_t index) {
    if (index >= vec_size){ 
      throw out_of_range("Index out of range");
    }

    // Shift elements to the left
    for (size_t i = index; i < vec_size - 1; i++) {
      size_t current_idx = (front_idx + i) % capacity;
      size_t next_idx = (front_idx + i + 1) % capacity;
      data[current_idx] = data[next_idx];
    }
  
    vec_size--;
  }
  

  /**
   * Inserts the given `T` as a new element in the `CircVector` after
   * the given index. If the index is out of bounds, throw error
   */
  
   // Inserts the given `T` as a new element in the CircVec after the given idx 
  void insert_after(size_t index, T elem) {
    if (vec_size == 0 || index >= vec_size) {
      throw out_of_range("Invalid index");
    }
  
    if (vec_size == capacity) {
      resize();
    }
  
    // wrap() already includes front_idx
    size_t insert_idx = wrap(index + 1);  // Get wrapped index for insertion
    
    // Shift elements to make room for the new element, start from the end and move backward
    for (size_t i = vec_size; i > index + 1; i--) {
      data[wrap(i)] = data[wrap(i - 1)];
    }
  
    // Insert new element
    data[insert_idx] = elem;
    vec_size++;
  }

  /**
   * Remove every other element (alternating) from the
   * `CircVector`, starting at index 1. Must run in O(N). May not reallocate.
   *
   * For example, if a list was `[0, 1, 2, 3, 4]`, removing every other element
   * would change the list to `[0, 2, 4]`.
   */
  
  // Remove every other element (alternating) from the CircVec, starting at idx 1.
  void remove_every_other() {
    if (vec_size <= 1) {
     return;
    }
    size_t new_size = 0;
    for (size_t i = 0; i < vec_size; i += 2) {
        data[wrap(new_size)] = data[wrap(i)];
        new_size++;
    }
    vec_size = new_size;
}



  /**
   * Returns a pointer to the underlying memory managed by the `CircVec`.
   * For autograder testing purposes only.
   */
  T *get_data() const {
    return this->data;
  }

  /**
   * Returns the capacity of the underlying memory managed by the `CircVec`. For
   * autograder testing purposes only.
   */
  size_t get_capacity() const {
    return this->capacity;
  }
};