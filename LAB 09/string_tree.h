#include <string>
#include <fstream>


using namespace std;

class StringTreeNode {
  public:
  string data;
  StringTreeNode* left;
  StringTreeNode* right;

  StringTreeNode(string data) {
    this->data = data;
    this->left = nullptr;
    this->right = nullptr;
  }
  StringTreeNode(string data, StringTreeNode* left, StringTreeNode* right) {
    this->data = data;
    this->left = left;
    this->right = right;
  }
};

class StringTree {
 public:
  StringTreeNode* root;

  StringTree(StringTreeNode* root) {
    this->root = root;
  }

  StringTree(istream& input);
  void serialize(ostream& output);
  ~StringTree();

 private:
  // TODO_STUDENT: declare recursive helpers for the...
  // - Constructor
  // - Serializer
  // - Destructor
  StringTreeNode* buildTree(istream& input);

  // Recursive helper for serializer (writes tree to output stream)
  void serializeHelper(ostream& output, StringTreeNode* node);

  // Recursive helper for destructor (deletes all nodes)
  void destroyTree(StringTreeNode* node);
};