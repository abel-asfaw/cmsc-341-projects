// CMSC 341 - Spring 2020 - Project 3

#include "BST.h"

// Constructor for the BNode nested class
BST::BNode::BNode(string ch, int rank, int height, int size) {
    _data = ch;
    _key = rank;
    _height = height;
    _size = size;
    _right = nullptr;
    _left = nullptr;
}

//constructor for the BST class; requires a balance function
BST::BST(balfn_t imbalanced) {
    _root = nullptr;
    _imbalancedPtr = nullptr;
    _imbalanced = imbalanced;
}

BST::~BST() {
    clear(_root);
}

//copy constructor
BST::BST(const BST& rhs) {
    _root = nullptr;
    _imbalancedPtr = nullptr;
    _imbalanced = rhs._imbalanced;
    copy_helper(this->_root, rhs._root);
}

//
void BST::copy_helper(BNode* &lhs, const BNode* rhs) {
    //base case
    if (rhs == nullptr) {
        lhs = nullptr; 
    }
    //recursion
    else {
        lhs = new BNode(rhs->_data, rhs->_key, rhs->_height, rhs->_size);
        copy_helper(lhs->_left, rhs->_left); 
        copy_helper(lhs->_right, rhs->_right);
    }
}

//assignment operator
BST& BST::operator=(const BST& rhs) {
    //prevents self-assignment
    if (this != &rhs) { 
        clear(_root);
        _root = nullptr;
        _imbalancedPtr = nullptr;
        _imbalanced = rhs._imbalanced;
        copy_helper(this->_root, rhs._root);
    }
    return *this;
}

//insert a (character, rank) pair
bool BST::insert(string ch, int rank) {
    bool inserted = false;
    if (_root == nullptr) {
        _root = new BNode(ch, rank);
        inserted = true;
    }
    else {
        inserted = insert_helper(_root, ch, rank);
    }
    //rebalance if imbalance has been detected
    if (_imbalancedPtr != nullptr) {
        rebalance_tree(_root);
        _imbalancedPtr = nullptr;
    }
    return inserted;
}

//inserts (key, data) pair in the tree
bool BST::insert_helper(BNode* node, string ch, int rank) {
    bool inserted = false;
    //search left of node for placement
    if (rank < node->_key) {
        //insert if node doesn't have left child
        if (node->_left == nullptr) {
            node->_left = new BNode(ch, rank);
            node->_height = 1 + max_height(node_height(node->_left), \
					   node_height(node->_right));
            node->_size++;
            return true;
        }
        else {
            inserted = insert_helper(node->_left, ch, rank);
        }
    }
    //search right of node for placement
    else if (rank > node->_key) {
        //insert if node doesn't have right child
        if (node->_right == nullptr) {
            node->_right = new BNode(ch, rank);
            node->_height = 1 + max_height(node_height(node->_left), \
					   node_height(node->_right));
            node->_size++;
            return true;
        }
        else {
            inserted = insert_helper(node->_right, ch, rank);
        }
    }
    //duplicate key
    else {
        return false;
    }
    //update height and size and check for imbalance if new node inserted
    if (inserted) {
        node->_size++;
        node->_height = 1 + max_height(node_height(node->_left), \
				       node_height(node->_right));
        check_imbalance(node);
    }
    return inserted;
}

//marks an imbalance in a tree
void BST::check_imbalance(BNode* root) {
    //node has both left and right children
    if (root->_left != nullptr && root->_right != nullptr) {
        if (_imbalanced(root->_left->_height, root->_right->_height, \
			root->_left->_size, root->_right->_size)) {
            _imbalancedPtr = root;
        }
    }
    //node has right child but no left child
    else if (root->_left == nullptr && root->_right != nullptr) {
        if (_imbalanced(-1, root->_right->_height, 0, root->_right->_size)) {
            _imbalancedPtr = root;
        }
    }
    //node has left child but no right child
    else if (root->_left != nullptr && root->_right == nullptr) {
        if (_imbalanced(root->_left->_height, -1, root->_left->_size, 0)) {
            _imbalancedPtr = root;
        }
    }
}

//gets the height of a node
int BST::node_height(BNode* node) {
    return ( node == nullptr ? -1 : node->_height );
}

//returns the max of left and right height of node
int BST::max_height(int left, int right) {
    return ( left > right ? left : right );
}

//gets the size of a tree
int BST::size() const {
    return ( _root == nullptr ? 0 : _root->_size );
}

//gets the height of the tree
int BST::height() const {
    return ( _root == nullptr ? -1 : _root->_height );
}

//copies the imbalanced subtree into an array in sorted order
void BST::bst_to_array(BNode* node, BNode** array, int &index) {
    if (node == nullptr) {
        return;
    }
    bst_to_array(node->_left, array, index);
    array[index] = node;
    index++;
    bst_to_array(node->_right, array, index);
}

//rebalances node at root of imbalanced subtree
void BST::rebalance_tree(BNode* node) {
    int index = 0; //used for inserting nodes into array
    //takes care of imbalance at the root of the original tree
    if (_imbalancedPtr->_key == _root->_key) {
        int begin = 0, end = _imbalancedPtr->_size - 1;
        _array = new BNode*[_imbalancedPtr->_size];
        bst_to_array(_imbalancedPtr, _array, index);
        BNode* root = array_to_bst(_array, begin, end);
        _root = root;
        delete [] _array;
    }
    //imbalance at subtree
    else if (node->_left != nullptr && node->_left->_key == _imbalancedPtr->_key) {
        int begin = 0, end = _imbalancedPtr->_size - 1;
        _array = new BNode*[_imbalancedPtr->_size];
        bst_to_array(_imbalancedPtr, _array, index);
        BNode* root = array_to_bst(_array, begin, end);
        //connect subtree back to original tree to its parent
        node->_left = root;
        delete [] _array;
    }
    else if (node->_right != nullptr && node->_right->_key == _imbalancedPtr->_key) {
        int begin = 0, end = _imbalancedPtr->_size - 1;
        _array = new BNode*[_imbalancedPtr->_size];
        bst_to_array(_imbalancedPtr, _array, index);
        BNode* root = array_to_bst(_array, begin, end);
        //connect subtree back to original tree to its parent
        node->_right = root;
        delete [] _array;
    }
    else if (_imbalancedPtr->_key < node->_key) {
        rebalance_tree(node->_left);
    }
    else if (_imbalancedPtr->_key > node->_key) {
        rebalance_tree(node->_right);
    }
}

//constructs a balanced tree from array
BST::BNode* BST::array_to_bst(BNode** array, int p, int r) {
    //p = start, q = middle, r = end
    //base case
    if (p > r) {
        return nullptr;
    }
    //middle node becomes the new root
    int q = (p + r)/2;
    array[q]->_height = 0; //reset height of node
    array[q]->_size = 1; //reset size of node
    BNode *root = array[q];
    //build left and right subtree
    root->_left = array_to_bst(array, p, q - 1);
    root->_right = array_to_bst(array, q + 1, r);
    //update height and size of each node
    int left_size = ( root->_left == nullptr ? 0 : root->_left->_size );
    int right_size = ( root->_right == nullptr ? 0 : root->_right->_size );
    root->_size   = 1 + (left_size + right_size);
    root->_height = 1 + max_height(node_height(root->_left), \
				   node_height(root->_right));
    return root;
} 

//print the data using an inorder traversal; if verbose is 'true',
//print the tree structure including sizes, heights, and
//parentheses.
void BST::dump(bool verbose) const {
    if (_root != nullptr) {
        dump_helper(_root, verbose);
        cout << endl;
        //print out size and height if verbose is true
        if (verbose) {
            cout << "size = " << size() << endl;
            cout << "height = " << height() << endl;
        }
    }
    else {
        cout << "The tree is empty.\n";
    }
}

//print tree structure if verbose is true, else just the data
void BST::dump_helper(BNode* node, bool verbose) const {
    if (node == nullptr) {
        return;
    }
    //print tree structure if verbose is true
    if (verbose) {
        cout << "(";
    }
    dump_helper(node->_left, verbose);
    cout << node->_data;
    //print tree structure if verbose is true
    if (verbose) {
        cout << ":" << node->_height << ":" << node->_size;
    }
    dump_helper(node->_right, verbose);
    //print tree structure if verbose is true
    if (verbose) {
        cout << ")";
    }
}

//recursively delete each node using postorder traversal
void BST::clear(BNode* node) {
    if (node == nullptr) {
        return;
    }
    clear(node->_left);
    clear(node->_right);
    delete node;
}
