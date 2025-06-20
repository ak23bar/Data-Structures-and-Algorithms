#pragma once

#include <iostream>
#include <map>
#include <set>
#include <unordered_map>
#include <vector>

using namespace std;
//CS 251: Project 6 OSM
/*
 * Graph ADT Implementation
 * ------------------------
 * Authored and Edited by: Akbar Aman
 * Circa April 2025
 */


/// @brief Simple directed graph using an adjacency list.
/// @tparam VertexT vertex type
/// @tparam WeightT edge weight type
template <typename VertexT, typename WeightT>
class graph {
 private:
  unordered_map<VertexT, map<VertexT, WeightT>> adjList; //unordered for O(1) lookup
  size_t edgeCount; //tracker

 public:
  /// Default constructor
  graph() {
    // Initialize an empty graph with zero edges
    edgeCount = 0;
  }

  /// @brief Add the vertex `v` to the graph, must typically be O(1).
  /// @param v
  /// @return true if successfully added; false if it existed already
  bool addVertex(VertexT v) {
    // check if vertex already exists
    if (adjList.find(v) != adjList.end()) {
      return false;  // Vertex already exists
    }
    
    // add new vertex with empty adj map
    adjList[v] = map<VertexT, WeightT>();
    return true;
  }

  /// @brief Add or overwrite directed edge in the graph, must typically be
  /// O(1).
  /// @param from starting vertex
  /// @param to ending vertex
  /// @param weight edge weight / label
  /// @return true if successfully added or overwritten;
  ///         false if either vertices isn't in graph
  bool addEdge(VertexT from, VertexT to, WeightT weight) {
    // Check if both vertices exist
    if (adjList.find(from) == adjList.end() || adjList.find(to) == adjList.end()) {
      return false;  // One or both vertices don't exist
    }
    
    // Check if edge already exists (for edge count tracking)
    bool edgeExists = (adjList[from].find(to) != adjList[from].end());
    
    // Add/update the edge
    adjList[from][to] = weight;
    
    // Update edge count
    if (!edgeExists) {
      edgeCount++;
    }
    
    return true;
  }

  /// @brief Maybe get the weight associated with a given edge, must typically
  /// be O(1).
  /// @param from starting vertex
  /// @param to ending vertex
  /// @param weight output parameter
  /// @return true if the edge exists, and `weight` is set;
  ///         false if the edge does not exist
  bool getWeight(VertexT from, VertexT to, WeightT& weight) const {
   
    // Check if "from" vertex exists
    auto fromIter = adjList.find(from);
    if (fromIter == adjList.end()) {
      return false;  // From vertex DNE
    }
    
    // Check if edge exists
    auto toIter = fromIter->second.find(to);
    if (toIter == fromIter->second.end()) {
      return false;  // Edge DNE
    }
    
    // Edge exists: therefore, set weight and return true
    weight = toIter->second;
    return true;
  }

  /// @brief Get the out-neighbors of `v`. Must run in at most O(|V|).
  /// @param v
  /// @return vertices that v has an edge to
  set<VertexT> neighbors(VertexT v) const {
    set<VertexT> S;
    
    // Check if vertex exists
    auto it = adjList.find(v);
    if (it != adjList.end()) {
      // Add all neighbor vertices to the set
      for (const auto& neighbor : it->second) {
        S.insert(neighbor.first);
      }
    }
    
    return S;
  }

  /// @brief Return a vector containing all vertices in the graph
  vector<VertexT> getVertices() const {
    vector<VertexT> vertices;
    // Reserve space to remain efficient 
    vertices.reserve(adjList.size());
    
    // Add all vertices to the result vector
    for (const auto& vertex : adjList) {
      vertices.push_back(vertex.first);
    }
    
    return vertices;
  }

  /// @brief Get the number of vertices in the graph. Runs in O(1).
  size_t numVertices() const {
    return adjList.size();
  }

  /// @brief Get the number of directed edges in the graph. Runs in at most
  /// O(|V|), but should be O(1).
  size_t numEdges() const {
    return edgeCount;
  }
};