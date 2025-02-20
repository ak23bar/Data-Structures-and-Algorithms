#include "include/search.h"

#include <algorithm>
#include <cctype>
#include <fstream>
#include <iostream>
#include <map>
#include <set>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

// CS 251
// Project 2: SEARCH
// Authored by: Akbar Aman

// Cleans a token by removing surrounding punctuation and converting to
// lowercase; Returns an empty string if the token is purely numeric or contains
// no letters
string cleanToken(const string& token) {
  // TODO student
  bool letter = false;
  bool num = false;
  for (char checking : token) {
    if (isalpha(checking)) {
      letter = true;
    }
    if (isdigit(checking)) {
      num = true;
    }
  }

  if (num && !letter) {
    return "";
  } else if (!num && !letter) {
    return "";
  }
  int start = 0;
  int end = token.size() - 1;

  // remove leading punct, other while removes trailing punct
  while (start <= end && ispunct(token[start])) {
    start++;
  }

  while (end >= start && ispunct(token[end])) {
    end--;
  }

  string cleanedToken;
  for (int i = start; i <= end; i++) {
    if (isalpha(token.at(i))) {
      cleanedToken += tolower(token.at(i));
    } else {
      cleanedToken += token.at(i);
    }
  }
  if (cleanedToken.empty()) {
    return "";
  }
  return cleanedToken;
}

// Skips over spaces and returns the index of the next non-space character: used
// in recursive func gatherTokens.
int checkingSpace(const string& s, int index) {
  while (index < s.length() && s[index] == ' ') {
    index++;
  }
  return index;
}

// Skips over letters (or words) and returns the index of the next space: used
// in recursive func gatherTokens
int checkingLetter(const string& s, int index) {
  while (index < s.length() && s[index] != ' ') {
    index++;
  }
  return index;
}

// Extracts unique words from a given text, Recursive function using previous
// declared functions.
set<string> gatherTokens(const string& text) {
  set<string> finishedTokens;
  int index = 0;
  while (index < text.size()) {
    int spaces = checkingSpace(text, index);
    int letters = checkingLetter(text, spaces);

    // Extracts and cleans tokens
    if (letters > spaces) {
      string temp = text.substr(spaces, letters - spaces);
      temp = cleanToken(temp);
      if (!temp.empty()) {
        finishedTokens.emplace(temp);
      }
    }

    index = letters + 1;
  }

  return finishedTokens;
}

// Reads a file and builds an index mapping words to URLs
int buildIndex(const string& filename, map<string, set<string>>& index) {
  ifstream dataFS;
  dataFS.open(filename);
  if (!dataFS.is_open()) {
    cout << "Invalid filename." << endl;
    return 0;
  }
  string temp;
  int count = 0;
  while (getline(dataFS, temp)) {
    count++;
    string tempURL = temp;
    string tempContent;
    set<string> tempToken;
    if (getline(dataFS, tempContent)) {
      tempToken = gatherTokens(tempContent);

      for (const string& token : tempToken) {
        index[token].insert(tempURL);
      }
    }
  }
  return count;
}

// Processes a query and returns matching URLs
set<string> findQueryMatches(const map<string, set<string>>& index,
                             const string& sentence) {
  stringstream sss(sentence);
  vector<string> wordz;
  set<string> URLresultz;
  string word;

  // Split query into individual words
  while (sss >> word) {
    wordz.push_back(word);
  }
  if (wordz.size() == 1) {
    string cleanedWord = cleanToken(wordz[0]);
    if (index.count(cleanedWord)) {
      set<string> matchingURLs = index.at(cleanedWord);
      URLresultz.insert(matchingURLs.begin(), matchingURLs.end());
    }
    return URLresultz;
  }

  for (int i = 0; i < wordz.size(); i++) {
    string cleanedWord = cleanToken(wordz[i]);
    if (i == 0) {
      if (index.count(cleanedWord)) {
        URLresultz = index.at(cleanedWord);
      }
    } else if (wordz[i].at(0) == '+') {
      cleanedWord = cleanToken(wordz[i].substr(1));
      if (index.count(cleanedWord)) {
        const set<string>& matchingURLs = index.at(cleanedWord);
        for (auto it = URLresultz.begin(); it != URLresultz.end();) {
          if (matchingURLs.find(*it) == matchingURLs.end()) {
            it = URLresultz.erase(it);
          } else {
            ++it;
          }
        }
      } else {
        URLresultz.clear();
      }
    } else if (wordz[i].at(0) == '-') {
      cleanedWord = cleanToken(wordz[i].substr(1));
      if (index.count(cleanedWord)) {
        const set<string>& matchingURLs = index.at(cleanedWord);
        for (auto it = URLresultz.begin(); it != URLresultz.end();) {
          if (matchingURLs.find(*it) != matchingURLs.end()) {
            it = URLresultz.erase(it);
          } else {
            ++it;
          }
        }
      }
    } else {
      if (index.count(cleanedWord)) {
        const set<string>& additionalURLs = index.at(cleanedWord);
        URLresultz.insert(additionalURLs.begin(), additionalURLs.end());
      }
    }
  }

  return URLresultz;
}

// Main function that initializes the search engine
void searchEngine(const string& filename) {
  map<string, set<string>> index;
  int pagesCount = buildIndex(filename, index);
  set<string> wordCount;
  for (pair<string, set<string>> p : index) {
    wordCount.insert(p.first);
  }

  cout << "Stand by while building index..." << endl;
  cout << "Indexed " << pagesCount << " pages containing " << wordCount.size()
       << " unique terms" << endl;
  cout << "Enter query sentence (press enter to quit):" << endl;

  string input;
  while (true) {
    getline(cin, input);

    if (input.empty()) {
      cout << "Thank you for searching!" << endl;
      break;
    }

    set<string> resultsURL = findQueryMatches(index, input);
    cout << "Found " << resultsURL.size() << " matching pages" << endl;
    for (string url : resultsURL) {
      cout << url << endl;
    }

    cout << "Enter query sentence (press enter to quit):" << endl;
  }
}

// passed all tests, expected: 100/100 :)