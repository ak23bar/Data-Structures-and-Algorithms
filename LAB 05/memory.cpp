#include <iostream>

using namespace std;

void f1() {
  int arr[5] = {1, 2, 3, 4, 5};
  for (int i = 0; i < 5; ++i) {  // correct loop condition to fix out of bounds
                                 // error (i <= 5 to i < 5)
    cout << arr[i] << endl;
  }
}

void f2() {
  int* ptr = new int;
  *ptr = 42;
  delete ptr;  // freeing allocated memory
}

void f3() {
  int* arr = new int[5];
  delete[] arr;
  // free memory only once by removing redundent "delete[] arr"
}

void f4() {
  int* p = new int;
  *p = 5;
  delete p;  // Free allocated memory
}

void f5() {
  int* ptr = new int;
  *ptr = 42;
  delete ptr;  // free allocated memory
}

int main() {
  // Call the functions here.
  f1();
  f2();
  f3();
  f4();
  f5();
}