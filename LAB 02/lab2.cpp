#include "lab2.h"

#include <iostream>
#include <sstream>
#include <string>

using namespace std;

int numWordsIn(const string& sentence) {
  // TODO student
  istringstream stream(sentence);
  string word;
  int count = 0;

  while (stream >> word) {
    count++;
  }
  return count;
}

int main() {
  string sentence;

  cout << "Enter a sentence please: ";
  getline(cin, sentence);

  int wordCount = numWordsIn(sentence);

  cout << "# of words: " << wordCount << endl;

  return 0;
}
