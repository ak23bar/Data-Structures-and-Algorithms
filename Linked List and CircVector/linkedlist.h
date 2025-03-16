#pragma once

#include <iostream>
#include <sstream>
#include <string>
#include <stdexcept>

// CS 251: Project 3
// LinkedList: O(N) runtime for specified functions
// Authored by Akbar Aman

using namespace std;

template <typename T>
class LinkedList {
 private:
  class Node {
   public:
    T value;
    Node* next;

    // Constructor for creating a node with just data (end of a list)
    Node(T data) : value(data), next(nullptr) {}
    // Constructor for inserting a node in the middle of a list
    // Links this node to the rest of the chain
    Node(T data, Node* next) : value(data), next(next) {}
  };

  size_t list_size;
  Node* list_front;

 public:
 
 /**
   * Default constructor. Creates an empty `LinkedList`.
   */
   LinkedList() {
    list_size = 0;
    list_front = nullptr;
  }

  /**
   * Returns whether the `LinkedList' is empty (ex: whether its size is 0).
   */
  bool empty() const {
    return list_size == 0;
  }

  /**
   * Returns the number of elements in the `LinkedList`.
   */
  size_t size() const {
    return list_size;
  }

  /**
   * Adds the given `T` to the front of the `LinkedList`.
   */
  void push_front(T data) {
    Node* new_node = new Node(data, list_front);
    list_front = new_node;
    list_size++;
  }

  /**
   * Adds the given `T` to the back of the `LinkedList`.
   */
  void push_back(T data) {
    Node* new_node = new Node(data);

    if (empty()) {
      list_front = new_node;
    } 
    else {
      Node* current = list_front;
      while (current->next != nullptr) {
        current = current->next;
      }
      current->next = new_node;
    }

    list_size++;
  }

  /**
   * Removes the element at the front of the `LinkedList`.
   *
   * If the `LinkedList` is empty, throw error
   */
  T pop_front() {
    if (empty()) {
        throw runtime_error("List is empty");
    }

    Node* temp = list_front;
    T value = temp->value;
    list_front = list_front->next;
    delete temp;
    list_size--;

    return value;
}



  /**
   * Removes the element at the back of the `LinkedList`.
   *
   * If the `LinkedList` is empty, throw error
   */
  T pop_back() {
    if (empty()) {
        throw runtime_error("List is empty");
    }

    if (list_size == 1) {
        T value = list_front->value;
        delete list_front;
        list_front = nullptr;
        list_size = 0;
        return value;
    }

    Node* current = list_front;
    while (current->next->next != nullptr) {
        current = current->next;
    }

    T value = current->next->value;
    delete current->next;
    current->next = nullptr;
    list_size--;

    return value;
}


  /**
   * Empties the `LinkedList`, releasing all allocated memory, and resetting
   * member variables appropriately.
   */
  void clear() {
    while (!empty()) {
      pop_front();
    }
  }

  /**
   * Destructor. Clears all allocated memory.
   */
  virtual ~LinkedList() {
    clear();
  }

  /**
   * Returns the element at the given index in the `LinkedList`.
   *
   * If the index is invalid, throw error
   */
  T& at(size_t index) {
    if (index >= list_size) {
        throw out_of_range("Index out of range");
    }

    Node* current = list_front;
    for (size_t i = 0; i < index; i++) {
        current = current->next;
    }

    return current->value;
}


  /**
 * Copy constructor. Creates a deep copy of the given `LinkedList`.
 *
 * Must run in O(N) time.
 */
LinkedList(const LinkedList& other) {
  list_size = 0;
  list_front = nullptr;
  
  if (other.empty()) {
    return;
  }
  
  // Copy the first node
  Node* other_current = other.list_front;
  list_front = new Node(other_current->value);
  list_size = 1;
  
  // Keep track of the tail of new list
  Node* current = list_front;
  other_current = other_current->next;
  
  // Copy remaining nodes
  while (other_current != nullptr) {
    current->next = new Node(other_current->value);
    current = current->next;
    other_current = other_current->next;
    list_size++;
  }
}

  /**
   * Assignment operator. Sets the current `LinkedList` to a deep copy of the
   * given `LinkedList`.
   *
   * Must run in O(N) time.
   */
  LinkedList& operator=(const LinkedList& other) {
    if (this == &other) return *this;
  
    // Clear the current list
    clear();
    list_size = 0;
    
    if (other.empty()) {
      return *this;
    }
    
    // Copy the first node
    Node* other_current = other.list_front;
    list_front = new Node(other_current->value);
    list_size = 1;
    
    // Keep track of the tail of new list
    Node* current = list_front;
    other_current = other_current->next;
    
    // Copy remaining nodes
    while (other_current != nullptr) {
      current->next = new Node(other_current->value);
      current = current->next;
      other_current = other_current->next;
      list_size++;
    }
    
    return *this;
  }

  /**
   * Converts the `LinkedList` to a string. Formatted like `[0, 1, 2, 3, 4]`
   * (without the backticks -- hover the function name to see). Runs in O(N)
   * time.
   */
  string to_string() const {
    stringstream ss;
    ss << "[";

    Node* current = list_front;
    if (current != nullptr) {
      ss << current->value;
      current = current->next;

      while (current != nullptr) {
        ss << ", " << current->value;
        current = current->next;
      }
    }

    ss << "]";
    return ss.str();
  }

  /**
   * Searches the `LinkedList` for the first matching element, and returns its
   * index. If no match is found, returns "-1".
   */
  size_t find(const T& data) const {
    Node* current = list_front;
    size_t index = 0;

    while (current != nullptr) {
      if (current->value == data) {
        return index;
      }
      current = current->next;
      index++;
    }

    return -1;  
  }

  /**
   * Remove the element at the specified index in this list.
   *
   * If the index is invalid, throw error
   */
  void remove_at(size_t index) {
    if (index >= list_size) {
        throw out_of_range("Index out of range");
    }

    // Handle removal of the first element
    if (index == 0) {
        pop_front();
        return;
    }

    // Traverse to the node before the one to be removed
    Node* temp = list_front;
    for (size_t i = 0; i < index - 1; i++) {
        temp = temp->next;
    }

    // Remove the target node
    Node* nodeToRemove = temp->next;
    temp->next = nodeToRemove->next;

    delete nodeToRemove;
    list_size--;
}


  /**
   * Inserts the given `T` as a new element in the `LinkedList` after
   * the given index. If the index is invalid, throw error
   */
  void insert_after(size_t index, T data) {
    if (index >= list_size) {
        throw out_of_range("Index out of range");
    }

    Node* current = list_front;
    for (size_t i = 0; i < index; i++) {
        current = current->next;
    }

    Node* new_node = new Node(data);
    new_node->next = current->next;
    current->next = new_node;
    list_size++;
}



  /**
   * Remove every other element (alternating) from the
   * `LinkedList`, starting at index 1. Must run in O(N).
   *
   * For example, if a list was `[0, 1, 2, 3, 4]`, removing every other element
   * would change the list to `[0, 2, 4]`.
   */
  void remove_every_other() {
    if (list_size <= 1) {
      return;  // Nothing to remove
    }

    Node* current = list_front;

    while (current != nullptr && current->next != nullptr) {
      Node* to_delete = current->next;
      current->next = to_delete->next;
      delete to_delete;
      list_size--;

      current = current->next;  // Move to the next node (if any)
    }
  }

  /**
   * Returns a pointer to the node at the front of the `LinkedList`. For
   * autograder testing purposes only.
   */
  void* front() const {
    return this->list_front;
  }
};