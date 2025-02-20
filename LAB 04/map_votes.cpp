#include "include/map_votes.h"

#include <fstream>
#include <iostream>
#include <map>
#include <sstream>

using namespace std;

void processVotesMap(ifstream& file, map<string, int>& restVotes) {
  string restaurant;
  while (getline(file, restaurant)) {
    restVotes[restaurant]++;
  }
}

string findMostVotedMap(const map<string, int>& restVotes) {
  string mostVoted;
  int maxVotes = 0;

  for (const auto& entry : restVotes) {
    if (entry.second > maxVotes) {
      maxVotes = entry.second;
      mostVoted = entry.first;
    }
  }

  return mostVoted;
}