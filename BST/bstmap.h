#pragma once

#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <utility>
//CS 251: Project 4 BSTMap
//Authored by: Akbar Aman


using namespace std;

template <typename KeyT, typename ValT>
class BSTMap {
 private:
  struct BSTNode {
    // This member should never change after initialization!
    const KeyT key; //STAYS CONST
    ValT value;
    BSTNode* parent;
    BSTNode* left;
    BSTNode* right;

    // This is fancy constructor syntax
    // Don't worry too much about it -- it's just necesssary for the const
    // member.
    BSTNode(KeyT key, ValT value, BSTNode* parent)
        : key(key),
          value(value),
          parent(parent),
          left(nullptr),
          right(nullptr) {
    }
  };

  BSTNode* root;
  size_t sz;

  // Utility pointer for begin and next.
  BSTNode* curr;

  // Private helper functions
  
  //recursive clear func
  void _clear(BSTNode* node) {
    if (node == nullptr) {
      return;
    }
    
    _clear(node->left);
    _clear(node->right);
    delete node;
  }
  
  //to string helper
  string _to_string(BSTNode* node) const {
    ostringstream result;
    
    // Uses an iterative in-order traversal instead of recursion
    if (node == nullptr) {
        return "";
    }
    
    // Create a stack for iterative traversal
    vector<BSTNode*> stack;
    BSTNode* current = node;
    
    // Traverse the tree
    while (current != nullptr || !stack.empty()) {
        // Reach the leftmost node of the current node
        while (current != nullptr) {
            stack.push_back(current);
            current = current->left;
        }
        
        // since current is now nullptr, pop from stack
        current = stack.back();
        stack.pop_back();
        
        // Process the node
        result << current->key << ": " << current->value << endl;
        
        // Move to the right subtree
        current = current->right;
    }
    
    return result.str();
}

//copy helper
BSTNode* _copy(const BSTNode* node, BSTNode* parent) {
    if (node == nullptr) {
      return nullptr;
    }
    
    BSTNode* new_node = new BSTNode(node->key, node->value, parent);
    new_node->left = _copy(node->left, new_node);
    new_node->right = _copy(node->right, new_node);
    
    return new_node;
  }

  //find min node helper
  BSTNode* _findMin(BSTNode* node) const {
    if (node == nullptr) {
      return nullptr;
    }
    
    while (node->left != nullptr) {
      node = node->left;
    }
    
    return node;
  }

  //find node helper
  BSTNode* _findNode(const KeyT& key) const {
    BSTNode* current = root;
    
    while (current != nullptr) {
      if (key == current->key) {
        return current;
      }
      
      if (key < current->key) {
        current = current->left;
      } else {
        current = current->right;
      }
    }
    
    return nullptr;
  }

// Helper func for erase (removing nodes which handles 4 specific cases of children)
ValT _removeNode(BSTNode* node) {
  ValT returnValue = node->value;
  
  // Case 1: Node is a leaf
  if (node->left == nullptr && node->right == nullptr) {
      if (node == root) {
          root = nullptr;
      } else if (node->parent->left == node) {
          node->parent->left = nullptr;
      } else {
          node->parent->right = nullptr;
      }
      delete node;
  }
  // Case 2: Node has only right child
  else if (node->left == nullptr) {
      BSTNode* rightChild = node->right;
      rightChild->parent = node->parent;

      if (node == root) {
          root = rightChild;
      } else if (node->parent->left == node) {
          node->parent->left = rightChild;
      } else {
          node->parent->right = rightChild;
      }
      delete node;
  }
  // Case 3: Node has only left child
  else if (node->right == nullptr) {
      BSTNode* leftChild = node->left;
      leftChild->parent = node->parent;

      if (node == root) {
          root = leftChild;
      } else if (node->parent->left == node) {
          node->parent->left = leftChild;
      } else {
          node->parent->right = leftChild;
      }
      delete node;
  }
  // Case 4: Node has both children
  else {
      // Find successor (minimum node in right subtree)
      BSTNode* successor = _findMin(node->right);
      ValT successorValue = successor->value; // Save successor's value
      
      // Create a new node with the successor's key and value
      BSTNode* newNode = new BSTNode(successor->key, successorValue, node->parent);
      
      // Connect the new node to the parent
      if (node == root) {
          root = newNode;
      } else if (node->parent->left == node) {
          node->parent->left = newNode;
      } else {
          node->parent->right = newNode;
      }
      
      // Connect the children to the new node
      newNode->left = node->left;
      if (node->left) {
          node->left->parent = newNode;
      }
      
      // Handle the right subtree connection
      if (successor == node->right) {
          // If successor is the direct right child
          newNode->right = successor->right;
          if (successor->right) {
              successor->right->parent = newNode;
          }
          delete successor; // Delete the successor
      } else {
          // If successor is deeper in the right subtree
          // First, update the successor's parent to point to successor's right child
          successor->parent->left = successor->right;
          if (successor->right) {
              successor->right->parent = successor->parent;
          }
          
          // Connect the node's right subtree to the new node
          newNode->right = node->right;
          if (node->right) {
              node->right->parent = newNode;
          }
          
          delete successor; // Delete the successor
      }
      
      delete node; // Delete the original node
  }
  
  sz--;
  return returnValue;
}
//This implementation of the helper allows no change to the const Key T key as specified by proffesor ayala.
  
 public:
  /**
   * Creates an empty `BSTMap`. Runs in O(1).
   */
  BSTMap() {
    root = nullptr;
    sz = 0;
    curr = nullptr;
  }

  /**
   * Checks if the `BSTMap` is empty. Runs in O(1).
   */
  bool empty() const {
    return sz == 0;
  }

  /**
   * Returns the number of mappings in the `BSTMap`. Runs in O(1).
   */
  size_t size() const {
    return sz;
  }

  /**
   * Adds the mapping `{key -> value}` to the `BSTMap`. If the key already
   * exists, does not update the mapping (like the C++ STL map).
   *
   * Uses the key to determine the location in the underlying BST. Creates
   * exactly one new node.
   *
   * Runs in O(H), where H is the height of the tree.
   */
  void insert(KeyT key, ValT value) {
    if (root == nullptr) {
      root = new BSTNode(key, value, nullptr);
      sz++;
      return;
    }
    
    BSTNode* current = root;
    BSTNode* parent = nullptr;
    
    while (current != nullptr) {
      if (key == current->key) {
        // Key already exists, do not update
        return;
      }
      
      parent = current;
      if (key < current->key) {
        current = current->left;
      } else {
        current = current->right;
      }
    }
    
    // Insert new node
    if (key < parent->key) {
      parent->left = new BSTNode(key, value, parent);
    } else {
      parent->right = new BSTNode(key, value, parent);
    }
    
    sz++;
  }

  /**
   * Return a reference to the value stored for `key` in the map.
   *
   * If key is not present in the map, throw `out_of_range` exception.
   *
   * Runs in O(H), where H is the height of the tree.
   */
  ValT& at(const KeyT& key) const {
    BSTNode* node = _findNode(key);
    
    if (node == nullptr) {
      throw out_of_range("Key not found in BSTMap");
    }
    
    return node->value;
  }

  /**
   * Returns `true` if the key is present in the map, and false otherwise.
   *
   * Runs in O(H), where H is the height of the tree.
   */
  bool contains(const KeyT& key) const {
    return _findNode(key) != nullptr;
  }

  /**
   * Empties the `BSTMap`, freeing all memory it controls.
   *
   * Runs in O(N), where N is the number of mappings.
   */
  void clear() {
    _clear(root);
    root = nullptr;
    sz = 0;
    curr = nullptr;
  }

  /**
   * Destructor, cleans up the `BSTMap`.
   *
   * Runs in O(N), where N is the number of mappings.
   */
  ~BSTMap() {
    clear();
  }

  /**
   * Converts the `BSTMap` to a string representation, with the values
   * in-order by key.
   *
   * Example:
   *
   * ```c++
   * BSTMap<string, int> names;
   * names.insert("Gwen", 10);
   * names.insert("Jen", 5);
   * names.insert("Ben", 10);
   * names.insert("Sven", 7);
   * ```
   *
   * Calling `names.to_string()` would return the following **multi-line**
   * string:
   *
   * ```text
   * Ben: 10
   * Gwen: 10
   * Jen: 5
   * Sven: 7
   * ```
   *
   * Runs in O(N), where N is the number of mappings.
   */
  string to_string() const {
    return _to_string(root);
  }

  /**
   * Copy constructor.
   *
   * Copies the mappings from the provided `BSTMap`.
   * The internal tree structure must be copied exactly.
   *
   * Runs in O(N), where N is the number of mappings in `other`.
   */
  BSTMap(const BSTMap& other) {
    root = nullptr;
    sz = 0;
    curr = nullptr;
    
    if (other.root != nullptr) {
      root = _copy(other.root, nullptr);
      sz = other.sz;
    }
  }

  /**
   * Assignment operator; `operator=`.
   *
   * Clears this tree, and copies the mappings from the provided `BSTMap`.
   * The internal tree structure must be copied exactly.
   *
   * Runs in O(N + O), where N is the number of mappings in `this`, and O is
   * the number of mappings in `other`.
   */
  BSTMap& operator=(const BSTMap& other) {
    // Check for self-assignment
    if (this == &other) {
      return *this;
    }
    
    // Clear this tree
    clear();
    
    // Copy the other tree
    if (other.root != nullptr) {
      root = _copy(other.root, nullptr);
      sz = other.sz;
    }
    
    return *this;
  }

  // =========================================================================

  /**
   * Locates the smallest key in the BSTMap, and removes and returns its
   * mapping. If the `BSTMap` is empty, throws a `runtime_error`.
   *
   * Creates no new nodes, and does not update the key or value of any
   * existing nodes.
   *
   * Runs in O(H), where H is the height of the tree.
   */
  pair<KeyT, ValT> remove_min() {
    if (empty()) {
      throw runtime_error("Cannot remove_min from an empty BSTMap");
    }
    
    BSTNode* min_node = _findMin(root);
    KeyT key = min_node->key;
    ValT value = min_node->value;
    
    // Remove the minimum node
    if (min_node == root && root->left == nullptr) {
      // Special case: root is the minimum and has no left child
      BSTNode* right_child = root->right;
      
      if (right_child != nullptr) {
        right_child->parent = nullptr;
      }
      
      delete root;
      root = right_child;
    } else if (min_node->right == nullptr) {
      // Minimum node has no right child
      if (min_node->parent->left == min_node) {
        min_node->parent->left = nullptr;
      } else {
        min_node->parent->right = nullptr;
      }
      
      delete min_node;
    } else {
      // Minimum node has a right child
      BSTNode* right_child = min_node->right;
      right_child->parent = min_node->parent;
      
      if (min_node->parent->left == min_node) {
        min_node->parent->left = right_child;
      } else {
        min_node->parent->right = right_child;
      }
      
      delete min_node;
    }
    
    sz--;
    return {key, value};
  }

  /**
   * Resets internal state for an iterative inorder traversal.
   *
   * See `next` for usage details. Modifies nothing except for `curr`.
   *
   * O(H), where H is the maximum height of the tree.
   */
  void begin() {
    if (empty()) {
      curr = nullptr;
      return;
    }
    
    // Set curr to the leftmost node (smallest key)
    curr = root;
    while (curr->left != nullptr) {
      curr = curr->left;
    }
  }

  /**
   * Uses the internal state to return the next in-order key and value
   * by reference, and advances the internal state. Returns `true` if the
   * reference parameters were set, and `false` otherwise.
   *
   * Example usage:
   *
   * ```c++
   * BSTMap<string, int> bst;
   * bst.begin();
   * string key;
   * int value;
   * while (bst.next(key, val)) {
   *   cout << key << ": " << val << endl;
   * }
   * ```
   *
   * Modifies nothing except for `curr`.
   *
   * Runs in worst-case O(H) where H is the height of the tree.
   */
  bool next(KeyT& key, ValT& val) {
    if (curr == nullptr) {
      return false;
    }
    
    // Set the output parameters
    key = curr->key;
    val = curr->value;
    
    // Advance to the next in-order node
    if (curr->right != nullptr) {
      // If there's a right child, go to the leftmost node in the right subtree
      curr = curr->right;
      while (curr->left != nullptr) {
        curr = curr->left;
      }
    } else {
      // If there's no right child, go up until we find a parent where we came from the left
      BSTNode* parent = curr->parent;
      while (parent != nullptr && curr == parent->right) {
        curr = parent;
        parent = parent->parent;
      }
      curr = parent;
    }
    
    return true;
  }

  // =========================================================================

  /**
   * Removes the mapping for the given key from the `BSTMap`, and returns the
   * value.
   *
   * Throws `out_of_range` if the key is not present in the map. Creates no new
   * nodes, and does not update the key or value of any existing nodes.
   *
   * Runs in worst-case O(H) where H is the height of the tree.
   */
  ValT erase(const KeyT& key) {
    BSTNode* node = _findNode(key);
    
    if (node == nullptr) {
      throw out_of_range("Key not found in BSTMap");
    }
    
    return _removeNode(node);
  }

  // =========================================================================

  /**
   * Returns a pointer to the root node of the `BSTMap`. For autograder testing
   * purposes only.
   */
  void* getRoot() const {
    return this->root;
  }
};