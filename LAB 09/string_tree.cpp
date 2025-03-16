#include <string>
#include <fstream>
#include <sstream>

#include "string_tree.h"

using namespace std;

StringTree::StringTree(istream& input) {
    root = buildTree(input);
}

StringTreeNode* StringTree::buildTree(istream& input) {
    string line;
    if (getline(input, line)) {
        string stringForNode = line.substr(3); // Skip "I: " or "L: "
        
        StringTreeNode* curr = new StringTreeNode(stringForNode);
        
        if (line.at(0) == 'I') { // Insert at next level
            curr->left = buildTree(input);
            
            curr->right = buildTree(input);
        }
        
        return curr;
    } 
    else {
        return nullptr;
    }
}

void StringTree::serialize(ostream& output) {
    serializeHelper(output, root);
}

void StringTree::serializeHelper(ostream& output, StringTreeNode* node) {
    if (node == nullptr) {
        return;
    }
    
    // Check if node is a leaf (has no children)
    if (node->left == nullptr && node->right == nullptr) {
        output << "L: " << node->data << endl;
    } else {
        output << "I: " << node->data << endl;
    }
    
    // Recursively serialize left and right subtrees
    serializeHelper(output, node->left);
    serializeHelper(output, node->right);
}

StringTree::~StringTree() {
    destroyTree(root);
}

void StringTree::destroyTree(StringTreeNode* curr) {
    if (curr == nullptr) {
        return;
    }
    
    destroyTree(curr->left);
    
    destroyTree(curr->right);
    
    delete curr;
}