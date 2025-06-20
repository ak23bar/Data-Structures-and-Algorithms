#include "application.h"
#include <iostream>
#include <limits>
#include <map>
#include <queue> 
#include <set>
#include <stack>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include "dist.h"
#include "graph.h"
#include "json.hpp"
//CS 251: Project 6 OSM
//To take the shortest path or to not take the shortest path?
/*
 * OSM Navigation System
 * ---------------------
 * Implementation of a UIC campus navigation system using OpenStreetMap data.
 * 
 * Features:
 *   - builds a graph from and using OSM JSON data (build graph func)
 *   - finds shortest paths between buildings using Dijkstra's algorithm (dijkstra func)
 *   - calculates optimal meeting points between 2 people
 *   - includes looking up buildings via names/abrreviations
 *   - supports building and launching an OSM server in web-browser (see makefile)
 *   
 *   
 * 
 * Authored and Edited by: Akbar Aman
 * Circa April 2025
 */

using namespace std;

double INF = numeric_limits<double>::max();

using json = nlohmann::json;

const string builds = "buildings";
const string ways = "waypoints"; //fixed bug by using const strings in order to use .contains() function for grabbing
const string foots = "footways";


void buildGraph(istream& input, graph<long long, double>& g, vector<BuildingInfo>& buildings, unordered_map<long long, Coordinates>& coords) {
  json jsonData;
  input >> jsonData;

  // clear old data first
  buildings.clear();
  coords.clear();

  // grab all the waypoints from json
  if (jsonData.contains(ways)) {
      for (auto waypointInit : jsonData[ways].items()) {
          auto wayPoint = waypointInit.value();
          long long id = wayPoint.value("id", 0LL);
          Coordinates coord = {
              wayPoint.value("lat", 0.0),
              wayPoint.value("lon", 0.0)
          };
          coords[id] = coord;
          g.addVertex(id); // add to graph
      }
  }

  // get the paths
  if (jsonData.contains(foots)) {
      for (auto footway : jsonData[foots]) {
          auto it = footway.begin();
          while (it + 1 != footway.end()) {
              long long firstNode = it->get<long long>();
              long long secondNode = (it + 1)->get<long long>();
              auto coords1 = coords[firstNode];
              auto coords2 = coords[secondNode];
              double distance = distBetween2Points(coords1, coords2);
              // need both ways cuz u can walk in both directions
              g.addEdge(firstNode, secondNode, distance);
              g.addEdge(secondNode, firstNode, distance);
              ++it;
          }
      }
  }

  // store all the buildings
  if (jsonData.contains(builds)) {
      for (auto buildinggg : jsonData[builds]) {
          BuildingInfo building;
          building.abbr = buildinggg.value("abbr", "");
          building.location.lat = buildinggg.value("lat", 0.0);
          building.location.lon = buildinggg.value("lon", 0.0);
          building.id = buildinggg.value("id", -1LL);
          building.name = buildinggg.value("name", "");

          buildings.push_back(building);
          g.addVertex(building.id); // add to graph as well
      }
  }

  // connect buildings to nearby waypoints
  for (const BuildingInfo& building : buildings) {
      for (const auto& waypointPair : coords) {
          const double offset = 0.036; // max distance to connect
          long long wpId = waypointPair.first;
          const Coordinates& wpCoords = waypointPair.second;
          double distance = distBetween2Points(building.location, wpCoords);
          if (distance <= offset) {
              // close enough so add edges both ways
              g.addEdge(building.id, wpId, distance);
              g.addEdge(wpId, building.id, distance);
          }
      }
  }
}

BuildingInfo getBuildingInfo(const vector<BuildingInfo> &buildings, const string &query) {
  for (const BuildingInfo &building : buildings) {
    if (building.abbr == query) {
      return building;
    } else if (building.name.find(query) != string::npos) {
      return building;
    }
  }
  BuildingInfo fail;
  fail.id = -1;
  return fail;
}

BuildingInfo getClosestBuilding(const vector<BuildingInfo> &buildings,
                                Coordinates c) {
  double minDestDist = INF;
  BuildingInfo ret = buildings.at(0);
  for (const BuildingInfo &building : buildings) {
    double dist = distBetween2Points(building.location, c);
    if (dist < minDestDist) {
      minDestDist = dist;
      ret = building;
    }
  }
  return ret;
}

// class for prioritizing vertices in the pq
class prioritize {
  public:
   bool operator()(const pair<long long, double>& p1, const pair<long long, double>& p2) const {
     return p1.second > p2.second;
   }
 };
 
 vector<long long> dijkstra(const graph<long long, double> &G, long long start, long long target, const set<long long> &ignoreNodes) {
   // handle edge cases
   if (start == target) {
     return {start};  // path is just the start node
   }
   
   // exception for start and target as these should never be ignored
   set<long long> ignoredVertices = ignoreNodes;
   ignoredVertices.erase(start);
   ignoredVertices.erase(target);
   
  
   unordered_map<long long, double> dist;    // distance from start to node
   unordered_map<long long, long long> prev; // prev node in optimal path
   unordered_set<long long> visited;         // set of visited verts
   
   // Pq for Dijkstra's 
   priority_queue<pair<long long, double>,vector<pair<long long, double>>, prioritize> pq;
   
   // initialize with infinity
   for (const long long &vertex : G.getVertices()) {
     // skip verts that should be ignored
     if (ignoredVertices.count(vertex) > 0) {
       continue;
     }
     dist[vertex] = INF;
   }
   
   // 0 start distance
   dist[start] = 0.0;
   
   // push onto pq
   pq.push({start, 0.0});
   
   // main loop with algorithm logic
   while (!pq.empty()) {
     // fetch vertex with smallest dist
     long long current = pq.top().first;
     double currentDist = pq.top().second;
     pq.pop();
     
     // skip if already visited
     if (visited.count(current) > 0) {
       continue;
     }
     
     // break if destination is reached
     if (current == target) {
       break;
     }
     
     // marking visited
     visited.insert(current);
     
     // skip if the distance in pq is outdated
     if (currentDist > dist[current]) {
       continue;
     }
     
     // process all neighbors
     for (const long long &neighbor : G.neighbors(current)) {
       // skip neighbors that should be ignored
       if (ignoredVertices.count(neighbor) > 0) {
         continue;
       }
       
       // get edge weight
       double weight;
       if (!G.getWeight(current, neighbor, weight)) {
         continue;  // Edge DNE
       }
       
       // Calc total distance to neighbor through current vertex
       double newDist = dist[current] + weight;
       
       // Update if shorter path is found
       if (newDist < dist[neighbor]) {
         dist[neighbor] = newDist;
         prev[neighbor] = current;
         pq.push({neighbor, newDist});
       }
     }
   }
   
   // If the target is unreachable: return empty path
   if (dist[target] == INF || prev.find(target) == prev.end()) {
     return {};
   }
   
   // rebuild the path from start to target
   vector<long long> path;
   for (long long at = target; at != start; at = prev[at]) {
     path.push_back(at);
   }
   path.push_back(start);
   
   // reverse  the path in order to get it from start to target
   reverse(path.begin(), path.end());
   
   return path;
 }

double pathLength(const graph<long long, double> &G,
                  const vector<long long> &path) {
  double length = 0.0;
  double weight;
  for (size_t i = 0; i + 1 < path.size(); i++) {
    bool res = G.getWeight(path.at(i), path.at(i + 1), weight);
    if (!res) {
      return -1;
    }
    length += weight;
  }
  return length;
}

void outputPath(const vector<long long> &path) {
  for (size_t i = 0; i < path.size(); i++) {
    cout << path.at(i);
    if (i != path.size() - 1) {
      cout << "->";
    }
  }
  cout << endl;
}

// Honestly this function is just a holdover from an old version of the project
void application(const vector<BuildingInfo> &buildings,
                 const graph<long long, double> &G) {
  string person1Building, person2Building;

  set<long long> buildingNodes;
  for (const auto &building : buildings) {
    buildingNodes.insert(building.id);
  }

  cout << endl;
  cout << "Enter person 1's building (partial name or abbreviation), or #> ";
  getline(cin, person1Building);

  while (person1Building != "#") {
    cout << "Enter person 2's building (partial name or abbreviation)> ";
    getline(cin, person2Building);

    // Look up buildings by query
    BuildingInfo p1 = getBuildingInfo(buildings, person1Building);
    BuildingInfo p2 = getBuildingInfo(buildings, person2Building);
    Coordinates P1Coords, P2Coords;
    string P1Name, P2Name;

    if (p1.id == -1) {
      cout << "Person 1's building not found" << endl;
    } else if (p2.id == -1) {
      cout << "Person 2's building not found" << endl;
    } else {
      cout << endl;
      cout << "Person 1's point:" << endl;
      cout << " " << p1.name << endl;
      cout << " " << p1.id << endl;
      cout << " (" << p1.location.lat << ", " << p1.location.lon << ")" << endl;
      cout << "Person 2's point:" << endl;
      cout << " " << p2.name << endl;
      cout << " " << p2.id << endl;
      cout << " (" << p2.location.lon << ", " << p2.location.lon << ")" << endl;

      Coordinates centerCoords = centerBetween2Points(p1.location, p2.location);
      BuildingInfo dest = getClosestBuilding(buildings, centerCoords);

      cout << "Destination Building:" << endl;
      cout << " " << dest.name << endl;
      cout << " " << dest.id << endl;
      cout << " (" << dest.location.lat << ", " << dest.location.lon << ")"
           << endl;

      vector<long long> P1Path = dijkstra(G, p1.id, dest.id, buildingNodes);
      vector<long long> P2Path = dijkstra(G, p2.id, dest.id, buildingNodes);

      // This should NEVER happen with how the graph is built
      if (P1Path.empty() || P2Path.empty()) {
        cout << endl;
        cout << "At least one person was unable to reach the destination "
                "building. Is an edge missing?"
             << endl;
        cout << endl;
      } else {
        cout << endl;
        cout << "Person 1's distance to dest: " << pathLength(G, P1Path);
        cout << " miles" << endl;
        cout << "Path: ";
        outputPath(P1Path);
        cout << endl;
        cout << "Person 2's distance to dest: " << pathLength(G, P2Path);
        cout << " miles" << endl;
        cout << "Path: ";
        outputPath(P2Path);
      }
    }

    //
    // another navigation?
    //
    cout << endl;
    cout << "Enter person 1's building (partial name or abbreviation), or #> ";
    getline(cin, person1Building);
  }
}
