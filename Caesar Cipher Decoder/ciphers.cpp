// CS 251: Project 1: Ciphers
// Edited by: Akbar Aman
#include <cctype>
#include <cstdlib>
#include <fstream>
#include <iomanip>  // added library for setprecision (used in SubstDec region)
#include <iostream>
#include <limits>  // added library for numeric_limits (used in SubstDec region)
#include <sstream>
#include <string>
#include <unordered_set>
#include <vector>

#include "include/caesar_dec.h"
#include "include/caesar_enc.h"
#include "include/subst_dec.h"
#include "include/subst_enc.h"
#include "utils.h"

using namespace std;

// Initialize random number generator in .cpp file for ODR reasons
std::mt19937 Random::rng;

const string ALPHABET = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";

// Function declarations go at the top of the file so we can call them
// anywhere in our program, such as in main or in other functions.
// Most other function declarations are in the included header
// files.

// When you add a new helper function, make sure to declare it up here!

/**
 * Print instructions for using the program.
 */
void printMenu();
void decryptSubstCipherFileCommand(const QuadgramScorer& scorer);

int main() {
  Random::seed(time(NULL));
  string command;

  // Open and read dictionary
  ifstream dataFS;
  dataFS.open("dictionary.txt");
  if (!dataFS.is_open()) {
    cout << "Failed to open dictionary" << endl;
    return 1;
  }

  vector<string> dictionary;
  string temp;
  while (dataFS >> temp) {
    dictionary.push_back(temp);
  }
  dataFS.close();

  // Read quadgrams
  vector<string> quadgrams;
  vector<int> counts;
  ifstream quadFS("english_quadgrams.txt");

  if (!quadFS.is_open()) {
    cout << "Failed to open english_quadgrams" << endl;
    return 1;
  }

  string line;
  while (getline(quadFS, line)) {
    size_t comma = line.find(',');
    if (comma != string::npos) {
      quadgrams.push_back(line.substr(0, comma));
      counts.push_back(stoi(line.substr(comma + 1)));
    }
  }
  quadFS.close();

  QuadgramScorer scorer(quadgrams, counts);

  cout << "Welcome to Ciphers!" << endl;
  cout << "-------------------" << endl;
  cout << endl;

  do {
    printMenu();
    cout << endl << "Enter a command (case does not matter!): ";
    getline(cin, command);
    cout << endl;

    if (command == "R" || command == "r") {
      string seed_str;
      cout << "Enter a non-negative integer to seed the random number "
              "generator: ";
      getline(cin, seed_str);
      Random::seed(stoi(seed_str));
    } else if (command == "C" || command == "c") {
      caesarEncryptCommand();
    } else if (command == "D" || command == "d") {
      caesarDecryptCommand(dictionary);
    } else if (command == "E" || command == "e") {
      computeEnglishnessCommand(scorer);
    } else if (command == "A" || command == "a") {
      applyRandSubstCipherCommand();
    } else if (command == "S" || command == "s") {
      decryptSubstCipherCommand(scorer);
    } else if (command == "F" || command == "f") {
      decryptSubstCipherFileCommand(scorer);
    } else if (command != "x" && command != "X" && !cin.eof()) {
      cout << "Invalid command. Try again." << endl;
    }

    cout << endl;

  } while (!(command == "x" || command == "X") && !cin.eof());

  return 0;
}

void printMenu() {
  cout << "Ciphers Menu" << endl;
  cout << "------------" << endl;
  cout << "C - Encrypt with Caesar Cipher" << endl;
  cout << "D - Decrypt Caesar Cipher" << endl;
  cout << "E - Compute English-ness Score" << endl;
  cout << "A - Apply Random Substitution Cipher" << endl;
  cout << "S - Decrypt Substitution Cipher from Console" << endl;
  cout << "F - Decrypt Substitution Cipher from File" << endl;
  cout << "R - Set Random Seed for Testing" << endl;
  cout << "X - Exit Program" << endl;
}

// "#pragma region" and "#pragma endregion" group related functions in this file
// to tell VSCode that these are "foldable". You might have noticed the little
// down arrow next to functions or loops, and that you can click it to collapse
// those bodies. This lets us do the same thing for arbitrary chunks!
#pragma region CaesarEnc

char rot(char c, int amount) {
  size_t pos = ALPHABET.find(c);
  if (pos == string::npos) {
    return c;
  }

  int newPos = (pos + amount) % 26;

  // Wrap around for negative rotation cases
  if (newPos < 0) {
    newPos += 26;
  }

  return ALPHABET[newPos];
}

string rot(const string& line, int amount) {
  string result;

  for (char c : line) {
    if (isalpha(c)) {
      result += rot(toupper(c), amount);
    } else if (isspace(c)) {
      result += c;
    }
  }

  return result;
}

void caesarEncryptCommand() {
  cout << "Enter the text to encrypt: ";
  string text;
  getline(cin, text);

  cout << "Enter the amount of rotation: ";
  string amountStr;
  getline(cin, amountStr);

  int amount = stoi(amountStr);

  string encrypted = rot(text, amount);
  cout << "Encrypted text: " << encrypted << endl;
}

#pragma endregion CaesarEnc

#pragma region CaesarDec

void rot(vector<string>& strings, int amount) {
  amount = amount % 26;  // Normalize

  for (string& str : strings) {
    for (char& ch : str) {
      if (isalpha(ch)) {
        char base = isupper(ch) ? 'A' : 'a';
        int shifted = ch - base + amount;  // addition for decryption
        shifted = (shifted + 26) % 26;     // wraparound
        ch = base + shifted;
      }
    }
  }
}

string clean(const string& s) {
  string result;
  for (char ch : s) {
    if (isalpha(ch)) {
      result += toupper(ch);
    }
  }
  return result;
}

vector<string> splitBySpaces(const string& s) {
  vector<string> words;
  string word;

  for (char ch : s) {
    if (ch == ' ' || ch == '\n') {  // newline handling
      if (!word.empty()) {
        words.push_back(word);
        word.clear();
      }
    } else {
      word += ch;
    }
  }
  if (!word.empty()) {
    words.push_back(word);
  }
  return words;
}

string joinWithSpaces(const vector<string>& words) {
  if (words.empty()) return "";

  string result;
  for (size_t i = 0; i < words.size(); ++i) {
    if (i > 0) result += " ";
    result += words[i];
  }
  return result;
}

int numWordsIn(const vector<string>& words, const vector<string>& dict) {
  unordered_set<string> dictSet;
  for (const string& word : dict) {
    dictSet.insert(word);
  }

  int count = 0;
  for (const string& word : words) {
    if (dictSet.count(word)) {
      count++;
    }
  }
  return count;
}

void caesarDecryptCommand(const vector<string>& dict) {
  string input;
  getline(cin, input);
  if (input.empty()) {
    cout << "No good decryptions found" << endl;
    return;
  }

  // split by spaces, then clean each word
  vector<string> words = splitBySpaces(input);
  for (string& word : words) {
    word = clean(word);
  }

  bool found = false;
  for (int shift = 0; shift < 26; ++shift) {
    vector<string> rotated = words;  // fresh start
    rot(rotated, shift);

    // Check if more than half the words are in dictionary
    int matches = numWordsIn(rotated, dict);
    if (matches > rotated.size() / 2) {
      cout << joinWithSpaces(rotated) << endl;
      found = true;
    }
  }

  if (!found) {
    cout << "No good decryptions found" << endl;
  }
}

#pragma endregion CaesarDec

#pragma region SubstEnc

string applySubstCipher(const vector<char>& cipher, const string& s) {
  string result;
  for (char c : s) {
    if (isalpha(c)) {
      char upperC = toupper(c);
      result += cipher[upperC - 'A'];  // Map based on the cipher
    } else {
      result += c;
    }
  }
  return result;
}

void applyRandSubstCipherCommand() {
  vector<char> cipher = genRandomSubstCipher();
  string input;
  getline(cin, input);
  string encrypted = applySubstCipher(cipher, input);
  cout << encrypted << endl;
}

#pragma endregion SubstEnc

#pragma region SubstDec

double scoreString(const QuadgramScorer& scorer, const string& s) {
  if (s.length() < 4) {
    return 0.0;
  }

  string cleanedStr = clean(s);
  double score = 0.0;

  for (size_t i = 0; i + 3 < cleanedStr.length(); ++i) {
    string quad = cleanedStr.substr(i, 4);
    score += scorer.getScore(quad);
  }

  return score;
}

void computeEnglishnessCommand(const QuadgramScorer& scorer) {
  cout << "Enter a string to score: ";
  string temp;
  getline(cin, temp);

  // redudnant call but does not matter
  temp = clean(temp);

  double score = scoreString(scorer, temp);

  cout << "Englishness: " << score << endl;
}

vector<char> decryptSubstCipher(const QuadgramScorer& scorer,
                                const string& ciphertext) {
  string cleanText = clean(ciphertext);
  vector<char> bestCipherOverall(26);
  double bestScoreOverall = -numeric_limits<double>::infinity();

  for (int run = 0; run < 20; ++run) {
    vector<char> currentCipher = genRandomSubstCipher();
    vector<char> bestCipherThisRun = currentCipher;
    string decrypted = applySubstCipher(currentCipher, cleanText);
    double bestScoreThisRun = scoreString(scorer, decrypted);

    int noImprovements = 0;
    while (noImprovements < 1500) {
      int pos1 = Random::randInt(25);
      int pos2;
      do {
        pos2 = Random::randInt(25);
      } while (pos2 == pos1);

      swap(currentCipher[pos1], currentCipher[pos2]);
      decrypted = applySubstCipher(currentCipher, cleanText);
      double currentScore = scoreString(scorer, decrypted);

      if (currentScore > bestScoreThisRun) {
        bestScoreThisRun = currentScore;
        bestCipherThisRun = currentCipher;
        noImprovements = 0;
      } else {
        swap(currentCipher[pos1], currentCipher[pos2]);
        noImprovements++;
      }
    }

    if (bestScoreThisRun > bestScoreOverall) {
      bestScoreOverall = bestScoreThisRun;
      bestCipherOverall = bestCipherThisRun;
    }
  }

  return bestCipherOverall;
}

void decryptSubstCipherCommand(const QuadgramScorer& scorer) {
  string input;
  getline(cin, input);

  if (input.empty()) {
    for (char c = 'A'; c <= 'Z'; ++c) {
      cout << c;
      if (c != 'Z') cout << " ";
    }
    cout << endl;
    return;
  }

  vector<char> cipher = decryptSubstCipher(scorer, input);
  string decrypted = applySubstCipher(cipher, input);
  cout << decrypted << endl;
}

void decryptSubstCipherFileCommand(const QuadgramScorer& scorer) {
  string inputFilename, outputFilename;
  cout << "Enter filename with text to substitution-cipher decrypt: ";
  getline(cin, inputFilename);
  cout << "Enter filename to write results to: ";
  getline(cin, outputFilename);

  ifstream inputFile(inputFilename);
  if (!inputFile.is_open()) {
    cout << "Failed to open input file: " << inputFilename << endl;
    return;
  }

  stringstream buffer;
  buffer << inputFile.rdbuf();
  string ciphertext = buffer.str();
  inputFile.close();

  vector<char> bestCipher = decryptSubstCipher(scorer, ciphertext);
  string decrypted = applySubstCipher(bestCipher, ciphertext);

  ofstream outputFile(outputFilename);
  if (!outputFile.is_open()) {
    cout << "Failed to open output file: " << outputFilename << endl;
    return;
  }

  outputFile << decrypted;
  outputFile.close();
}
// 58/58 test cases passed: =)

#pragma endregion SubstDec
