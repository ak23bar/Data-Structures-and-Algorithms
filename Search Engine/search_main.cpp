#include <iostream>
#include <string>

#include "include/search.h"

using namespace std;

int main() {
  string filename;

  cout << "Enter a filename: ";
  getline(cin, filename);

  // This works becuz I'm done with the project!
  searchEngine(filename);
  return 0;
}
