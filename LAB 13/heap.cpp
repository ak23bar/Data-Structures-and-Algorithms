#include "heap.h"

#include <cctype>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

void MinHeap::insert(TrainStation val) {
  // Add the new element to the end of the vector
  data.push_back(val);
  
  // Sift up the newly added element to its proper position
  _siftup(data.size() - 1);
}

TrainStation MinHeap::remove() {
  // Store the minimum element to return later
  TrainStation min = data[0];
  
  // Replace the root with the last element
  data[0] = data[data.size() - 1];
  
  // Remove the last element
  data.pop_back();
  
  // If the heap is not empty, sift down the new root
  if (data.size() > 0) {
    _siftdown(0);
  }
  
  return min;
}

TrainStation MinHeap::peek() {
  // Return the minimum element (at the root)
  return data[0];
}

void MinHeap::_siftup(int index) {
  // If this is the root, we can't sift up further
  if (index == 0) {
    return;
  }
  
  int parentIndex = _parent(index);
  
  // Keep sifting up as long as the current element is smaller than its parent
  // (In this case, comparing ridership values)
  while (index > 0 && data[index].ridership < data[parentIndex].ridership) {
    // Swap the current element with its parent
    TrainStation temp = data[index];
    data[index] = data[parentIndex];
    data[parentIndex] = temp;
    
    // Move up to the parent
    index = parentIndex;
    parentIndex = _parent(index);
  }
}

void MinHeap::_siftdown(int index) {
  int size = data.size();
  int leftChild = _left(index);
  int rightChild = _right(index);
  int smallest = index;
  
  // Find the smallest among the node and its children
  if (leftChild < size && data[leftChild].ridership < data[smallest].ridership) {
    smallest = leftChild;
  }
  
  if (rightChild < size && data[rightChild].ridership < data[smallest].ridership) {
    smallest = rightChild;
  }
  
  // If the smallest is not the current node, swap and continue sifting down
  if (smallest != index) {
    TrainStation temp = data[index];
    data[index] = data[smallest];
    data[smallest] = temp;
    
    // Recursively sift down the smaller child
    _siftdown(smallest);
  }
}