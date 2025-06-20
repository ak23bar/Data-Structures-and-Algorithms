#include <fstream>
#include <map>
#include <sstream>

#include "heap.h"

vector<TrainStation> topK(vector<TrainStation> allStations, int k) {
  // Create a min-heap to keep track of the top k stations
  MinHeap minHeap;
  vector<TrainStation> result;
  
  // Process each station
  for (TrainStation station : allStations) {
    // If the heap has less than k elements, simply insert
    if (minHeap.size() < k) {
      minHeap.insert(station);
    } 
    // If the current station has higher ridership than the minimum in our heap,
    // remove the minimum and insert the current station
    else if (station.ridership > minHeap.peek().ridership) {
      minHeap.remove(); // Remove the smallest element
      minHeap.insert(station); // Insert the new, larger element
    }
    // If the current station has lower ridership than the minimum in our heap,
    // ignore it as it cannot be in the top k
  }
  
  // Extract all elements from the heap to the result vector
  // The elements will be in ascending order of ridership
  while (minHeap.size() > 0) {
    result.push_back(minHeap.remove());
  }
  
  return result;
}

int main() {
  ifstream file("cta_ridership_perStation.csv");
  map<int, TrainStation> stationMap;
  string line;
  getline(file, line);  // Read the header line.

  // Read in the data for all stations and accumulate the ridership.
  while (getline(file, line)) {
    istringstream iss(line);
    string token;
    getline(iss, token, ',');  // read id
    int id = stoi(token);

    getline(iss, token, ',');  // read name
    string name = token;

    getline(iss, line, ',');  // discard date
    getline(iss, line, ',');  // discard daytype

    getline(iss, token, ',');  // read number of rides
    int rides = stoi(token);

    if (stationMap.count(id) == 0) {
      TrainStation ts;
      ts.id = id;
      ts.name = name;
      ts.ridership = rides;
      stationMap[id] = ts;
    } else {
      stationMap[id].ridership = stationMap[id].ridership + rides;
    }
  }

  // Put all the train stations in a vector.
  vector<TrainStation> allStations;
  for (pair<int, TrainStation> p : stationMap) {
    allStations.push_back(p.second);
  }

  int k = 10;
  vector<TrainStation> topKStations = topK(allStations, k);

  cout << "Top " << k << " stations in terms of ridership: " << endl;
  for (int i = 1; i <= k; i++) {
    int j = k - i;
    string name = topKStations[j].name;
    int ridership = topKStations[j].ridership;
    cout << i << ". " << name << " - " << ridership << " rides" << endl;
  }
}
