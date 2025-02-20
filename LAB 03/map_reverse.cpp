#include <map>
#include <set>
#include <string>
#include <vector>

using namespace std;

// TODO student: update signature
/**
 * Returns a new map that is the "reversal" of the given map `m`.
 * For example, the map `{{"key", "value"}}` would be reversed to the map
 * `{{"value", "key"}}`.
 */
map<string, set<string>> reverseMap(const map<string, string>& m) {
    map<string, set<string>> reversed;
    for (const auto& pair : m) {
        reversed[pair.second].insert(pair.first);  // Map value to key(s)
    }
    return reversed;
}


int main() {
  // Because you're changing the signature, we don't have tests for this
  // function. Instead, update this example to demonstrate that your function
  // works as intended!
  map<string, string> example = {
      {"key", "value"},
      {"key 2", "value 2"},
  };
}
