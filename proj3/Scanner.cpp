// CMSC 341 - Spring 2020 - Project 3

#include <vector>
#include "Scanner.h"

// Balance function for use with BSTs.  This implements the
// height-balance property for AVL trees.  Returns true if
// height-balance property is violated.
bool Scanner::heightImbalance(int leftHeight, int rightHeight, int leftSize, int rightSize) {
  return (leftHeight > rightHeight + 1) || (rightHeight > leftHeight + 1);
}

// SNode constructor.  bounds contains the  and first bounds on
// the weights for this BST.
Scanner::SNode::SNode(pair<int, int> bounds) {
    _bounds = bounds;
    _root = new BST(heightImbalance);
    _left = _right = nullptr;
}

Scanner::SNode::~SNode() {
    delete _root; //calls destructor from BST class
}

// Initialize a Scanner object to process an image with 'lines' lines
// of text, each of which is 'range' characters long.  The ASCII
// characters corresponding to the character indices are passed in
// 'chars'.
Scanner::Scanner(int lines, int range, vector<string> chars) {
    _lines = lines;
    _chars = chars;
    pair<int, int> bounds;
    bounds.first = 1 + (_lines * range) - range; 
    bounds.second = (_lines * range);
    //starting bounds 477 - 510
    SNode* prev;
    SNode* curr = nullptr;
    for (int i = 0; i < _lines; i++) {
        prev = new SNode(bounds);
        prev->_right = curr;
        curr = prev;
        bounds.first  -= range;
        bounds.second -= range;
    }
    _forest = prev;
}

Scanner::~Scanner() {
    clear(_forest);
}

Scanner::Scanner(const Scanner& rhs) {
    _forest = nullptr;
    _lines = rhs._lines;
    _chars = rhs._chars;
    copyHelper(_forest, rhs._forest);
}

void Scanner::copyHelper(SNode* &lhs, const SNode* rhs) {
    //base case
    if (rhs == nullptr) {
        lhs = nullptr;
    }
    //recursion
    else {
        lhs = new SNode(rhs->_bounds);
        //calls the ov. assgn. operator from BST class to perform deep copy
        lhs->_root->operator=(*rhs->_root);
        copyHelper(lhs->_left, rhs->_left); 
        copyHelper(lhs->_right, rhs->_right);
    }
}

Scanner& Scanner::operator=(const Scanner& rhs) {
    if (this != &rhs) {
        clear(_forest); //clear lhs's data structure
        _forest = nullptr;
        _lines = rhs._lines;
        _chars = rhs._chars;
        copyHelper(_forest, rhs._forest);
    }
    return *this;
}

// Read the data files and insert into the datastructure.  File in
// 'ascii' contains the scrambled character indices; file 'weights'
// contains the corresponding weights.
bool Scanner::loadFiles(string ascii, string weights) {
    std::ifstream asciiFile, weightFile;
    string tempCh, tempRank, ch, rank;
    asciiFile.open(ascii);
    weightFile.open(weights);
    //check if both files exist;
    if (asciiFile.is_open() && weightFile.is_open()) {
        //read until the end of file
        while (!weightFile.eof() && !asciiFile.eof()) {
            getline(asciiFile, tempCh, '\n');
            getline(weightFile, tempRank, '\n');
            std::stringstream chStream(tempCh);
            std::stringstream rankStream(tempRank);
            while (!chStream.eof() && !rankStream.eof()) {
                //ignore new line character
                if (chStream.peek() == '\n') {
                    chStream.ignore();
                }
                //ignore new line character
                if (rankStream.peek() == '\n') {
                    rankStream.ignore();
                }
                getline(chStream, ch, ',');
                getline(rankStream, rank, ',');
                int intCh = stoi(ch);
                int intRank = stoi(rank);
                insert(intRank, intCh);
            }
        }
        return true;
    }
    cout << "Incorrect file name/file does not exist. Please try again.\n";
    return false;
}

// Insert a node; splay the node that is inserted.
bool Scanner::insert(int weight, int ch) {
    bool inserted = false;
    _forest = splay(_forest, weight);
    inserted = _forest->_root->insert(_chars.at(ch-1), weight);
    return inserted;
}

//brings the SNode with correct bounds to the root of our splay tree
Scanner::SNode* Scanner::splay(SNode* root, int weight) {
    //weight bounds found at root
    if (weight >= root->_bounds.first && weight <= root->_bounds.second) {
        return root;
    }
    //node with bounds that include weight are to the right of subtree root
    if (weight > root->_bounds.second) {
        if (root->_right == nullptr) {
            return root;
        }
        //zag-zag
        if (weight > root->_right->_bounds.second) {
            root->_right->_right = splay(root->_right->_right, weight);
            root = leftRotate(root);
        }
        //zag-zig
        else if (weight < root->_right->_bounds.first) {
            root->_right->_left = splay(root->_right->_left, weight);
            root->_right = rightRotate(root->_right);
        }
        //second rotation if right child exists
        return leftRotate(root);
    }
    //node with bounds that include weight are to the left of subtree root
    else if (weight < root->_bounds.first) {
        if (root->_left == nullptr) {
            return root;
        }
        //zig-zig
        if (weight < root->_left->_bounds.first) {
            root->_left->_left = splay(root->_left->_left, weight);
            root = rightRotate(root);
        }
        //zig-zag
        else if (weight > root->_left->_bounds.second) {  
            root->_left->_right = splay(root->_left->_right, weight);
            root->_left = leftRotate(root->_left);
        }
        //second rotation if left child exists
        return rightRotate(root);
    }
    return root; //bounds which include weight not found
}

//zig (right rotation)
Scanner::SNode* Scanner::rightRotate(SNode* node) {
    //excecute right rotation only if left child exists
    if (node->_left != nullptr){
        SNode* temp = node->_left;
        node->_left = temp->_right;
        temp->_right = node;
        return temp;
    }
    return node;
}

//zag (left rotation)
Scanner::SNode* Scanner::leftRotate(SNode* node) {
    //excecute left rotation only if right child exists
    if (node->_right != nullptr){
        SNode* temp = node->_right;
        node->_right = temp->_left;
        temp->_left = node;
        return temp;
    }
    return node;
}

// Prints the unscrambled ASCII art.  Does an inorder traversal of
// the splay tree, calling the BST dump() method for each BST.
void Scanner::dump() const {
    if (_forest != nullptr) {
        dumpHelper(_forest);
    }
    else {
        cout << "Splay tree is empty.\n";
    }
}

void Scanner::dumpHelper(SNode* node) const {
    if (node == nullptr) {
        return;
    }
    dumpHelper(node->_left);
    if (node->_root != nullptr) {
        node->_root->dump();
    }
    dumpHelper(node->_right);
}

// Prints an inorder traversal of the splay tree, printing the
// bounds (:first) for each node in the splay tree.  Prints
// parentheses to show the structure of the tree.
void Scanner::inorder() const {
    if (_forest != nullptr) {
        inorderHelper(_forest);
    }
    else {
        cout << "Scanner is empty.\n";
    } 
}
//
void Scanner::inorderHelper(SNode* node) const {
    if (node == nullptr) {
        return;
    }
    cout << "(";
    inorderHelper(node->_left);
    cout << node->_bounds.first << ":" << node->_bounds.second;
    inorderHelper(node->_right);
    cout << ")";
}

//destructs the scanner class
void Scanner::clear(SNode* node) {
    if (node == nullptr) {
        return;
    }
    clear(node->_left);
    clear(node->_right);
    delete node;
}