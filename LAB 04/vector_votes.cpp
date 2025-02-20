#include "include/vector_votes.h"

#include <fstream>
#include <iostream>
#include <set>
#include <sstream>
#include <vector>

using namespace std;

void processVotesVector(ifstream& file, vector<string>& votes,
                        set<string>& restaurants) {
  string restaurant;
  while (getline(file, restaurant)) {
    votes.push_back(restaurant);
    restaurants.insert(restaurant);
  }
}

string findMostVotedVector(const vector<string>& votes,
                           const set<string>& restaurants) {
  string mostVoted;
  int maxVotes = 0;

  for (const string& restaurant : restaurants) {
    int count = 0;
    for (const string& vote : votes) {
      if (vote == restaurant) {
        count++;
      }
    }
    if (count > maxVotes) {
      maxVotes = count;
      mostVoted = restaurant;
    }
  }

  return mostVoted;
}
