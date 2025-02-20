#include <fstream>
#include <iostream>
#include <map>
#include <set>
#include <string>
#include <vector>

#include "billboard.h"

using namespace std;

map<string, int> buildCounts(const vector<string>& artists) {
  // The returned map should answer the question:
  //     How many weeks has this artist had a number
  //     one song in the Billboard top 100?

  map<string, int> counts;
  for(const string& artist : artists){
    counts[artist]++;
  }
  return counts;
}

map<char, set<string>> buildLetterIndex(const vector<string>& artists) {
 map<char, set<string>> letterIndex;
 for(const string& artist : artists){
  char firstLetter = artist[0];
  letterIndex[firstLetter].insert(artist);
}
return letterIndex;
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
