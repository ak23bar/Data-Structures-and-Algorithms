#include <iostream>

using namespace std;

class ourvector {
 private:
  int* data;
  size_t capacity;
  size_t numElements;

  void resize(size_t newCapacity) {
    if (newCapacity > this->capacity) {
      int* newData = new int[newCapacity];

      for (size_t i = 0; i < this->numElements; ++i) {
        newData[i] = this->data[i];
      }

      delete[] this->data;  // Free old memory
      this->data = newData;
      this->capacity = newCapacity;
    }
  }

 public:
  ourvector() {
    this->capacity = 2;
    this->numElements = 0;
    this->data = new int[capacity];
  }

  // Copy constructor (deep copy)
  ourvector(const ourvector& other) {
    this->capacity = other.capacity;
    this->numElements = other.numElements;
    this->data = new int[this->capacity];

    for (size_t i = 0; i < this->numElements; ++i) {
      this->data[i] = other.data[i];
    }
  }

  // Assignment operator (deep copy)
  ourvector& operator=(const ourvector& other) {
    if (this != &other) {   // Prevent self-assignment
      delete[] this->data;  // Free existing memory

      this->capacity = other.capacity;
      this->numElements = other.numElements;
      this->data = new int[this->capacity];

      for (size_t i = 0; i < this->numElements; ++i) {
        this->data[i] = other.data[i];
      }
    }
    return *this;
  }

  void push_back(int value) {
    if (this->numElements == this->capacity) {
      resize(this->capacity * 2);  // Double capacity for efficiency
    }

    this->data[this->numElements] = value;
    this->numElements++;
  }

  int size() const {
    return this->numElements;
  }

  int at(size_t index) const {
    if (index >= this->numElements) {
      throw out_of_range("Index out of bounds");
    }
    return this->data[index];
  }

  ~ourvector() {
    delete[] this->data;
    this->data = nullptr;
  }
};

int sumvec(const ourvector& v) {  // Pass by reference to avoid copy
  int sum = 0;
  for (size_t j = 0; j < v.size(); j++) {
    sum += v.at(j);
  }
  return sum;
}

int main() {
  ourvector vec;
  int i = 0;
  while (i <= 10) {
    vec.push_back(i++);
  }

  cout << "Summation = " << sumvec(vec) << endl;
}
