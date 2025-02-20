#include <fstream>
#include <iostream>
#include <map>
#include <set>
#include <string>
#include <vector>

#include "billboard.h"

using namespace std;

map<__________, ________> buildCounts(const vector<string>& artists) {
  // The returned map should answer the question:
  //     How many weeks has this artist had a number
  //     one song in the Billboard top 100?

  // TODO by student
}

map<__________, ________> buildLetterIndex(const vector<string>& artists) {
  // The returned map should answer the question:
  //     What are the artists who have had a #1 song in the
  //     Billboard top 100, whose name start with the given
  //     character?

  // TODO by student
}

int main() {
  // string filename = "billboard.txt";
  string filename = "billboardSmall.txt";
  vector<string> artists;

  parseBillboardFile(filename, artists);

  cout << "<<<Number of weeks with a #1 song>>>" << endl;
  auto counts = buildCounts(artists);
  for (auto entry : counts) {
    cout << entry.first << " : " << entry.second << endl;
  }

  cout << endl;

  cout << "<<<Index of all artists with a #1 song>>>" << endl;
  auto letterIndex = buildLetterIndex(artists);
  for (auto entry : letterIndex) {
    cout << entry.first << ":" << endl;
    for (string artist : entry.second) {
      cout << "\t" << artist << endl;
    }
  }
}
