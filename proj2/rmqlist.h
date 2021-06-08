// CMSC 341 - Spring 2020 - Project 2
#ifndef _RMQLIST_H
#define _RMQLIST_H

#include <iostream>
#include <stdexcept>
#include <cmath>
#include <iomanip>

using std::swap;
using std::ostream;
using std::cout;
using std::endl;
using std::sqrt;
using std::range_error;
using std::invalid_argument;

// Macro for a two-argument min function
#define MIN(a, b)  ((a) < (b) ? (a) : (b))

// forward declarations
template <class K, class V> class RMQList;
template <class K, class V> class Node;
template <class K, class V> ostream& operator<<(ostream &sout, const Node<K,V> &x);

// *********************************************
// Node - node class for the RMQList linked list
//   key and value are templated (types K and V)
// *********************************************

template <class K, class V>
class Node {
  friend RMQList<K, V>;
public:
  Node(K key = K(), V value = V(), Node<K, V> *next = nullptr){
    _key = key;
    _value = value;
    _next = next;
  }
  friend ostream& operator<< <K,V>(ostream &sout, const Node<K,V> &x);
private:
  K _key;
  V _value;
  Node *_next;
};

// Overloaded insertion operator for Node
template <class K, class V>
ostream& operator<<(ostream &sout, const Node<K,V> &x){
  sout << "Key: " << x._key << ", Value: " << x._value;
  return sout;
}

// *******************************************************
// RMQList - list container (linked list) with RMQ support
//   key and value are templated (types K and V)
// *******************************************************

template <class K, class V>
class RMQList {
public:
  // Create an empty RMQList object
  RMQList();
  
  // Destructor, Copy Constructor, Assignment Operator
  ~RMQList();
  RMQList(const RMQList<K,V> &rhs);
  const RMQList<K,V>& operator=(const RMQList<K,V> &rhs);
  
  // In-line function. Returns the size (number of elements).
  int size() const { return _listSize; }

  // In-line function. Returns true if the list is empty; false
  // otherwise.
  bool empty() const { return _head == nullptr; }
  
  // Insert an element into the list; list must be kept in increasing
  // order by key; duplicate keys are not allowed, so return false if
  // there is already an entry with the specified key, true otherwise.
  // Should check if key > largest current key and, if so, append to
  // the end of the list without iteration.
  bool insert(const K& key, const V& value);

  // Remove an element from the list; return false if no element
  // exists with the specified key value, true otherwise
  bool remove(const K& key);

  // Update value for the element with given key; return false if
  // there is no element with the given key, true otherwise
  bool update(const K& key, const V& value);
  
  // RMQ Query for k1 to k2.  Throws exceptions (see documentation).
  V query(const K& k1, const K& k2);

  // Dump the list entries
  void dumpList() const;
  
  // Dump the RMQ info and table.  What gets dumped depends on which
  // RMQ method is used.
  void dumpTable() const;

  // Clear the data data strucures
  void clear();
  
private:
  Node<K,V> *_head;
  Node<K,V> *_tail;
  int _listSize;
  // *******************************************
  V **_minArray;
  int _minCapacity;
  bool _listUpdated; //let's us know whether our list has changed
  Node<K,V> **_nodeArray;
  int _nodeArrayCapacity;
  const int _defaultSize = 40;
  // *******************************************
  void precompute();
  void updateMinCapacity();
  int findKeyIndex(const K& key);
  // *******************************************
};

//Overloaded constructor
template <class K, class V>
RMQList<K,V>::RMQList(){
    //initialize linked list
    _head = nullptr;
    _tail = nullptr;
    _listSize = 0;
    _listUpdated = false;
    //initialize array of node pointers
    _nodeArray = new Node<K,V>*[_defaultSize];
    _nodeArrayCapacity = _defaultSize;
    //initialize 2D array of mins
    _minArray = new V*[_defaultSize];
    for (int i = 0; i < _defaultSize; i++){
        _minArray[i] = new V[_defaultSize]; 
    }
    _minCapacity = _defaultSize;
} 

// Destructor
template <class K, class V>
RMQList<K,V>::~RMQList(){
    clear();
    //destruct _minArray
    for (int i = 0; i < _minCapacity; i++){
        delete [] _minArray[i];
    }
    delete [] _minArray;
    //destruct _nodeArray
    delete [] _nodeArray;
}

// Copy constructor
template <class K, class V>
RMQList<K,V>::RMQList(const RMQList<K,V> &rhs){
    //only copy if source isn't empty
    if (!rhs.empty()){
        // **** prevents conditional jump ****
        _minArray = new V*[_defaultSize];
        for (int i = 0; i < _defaultSize; i++){
            _minArray[i] = new V[_defaultSize]; 
        }
        _minCapacity = _defaultSize;
        _nodeArray = new Node<K,V>*[_defaultSize];
        _nodeArrayCapacity = _defaultSize;
        // ***********************************
        _head = nullptr;
        _tail = nullptr;
        _listSize = 0;
        Node<K,V> *curr = rhs._head;
        Node<K,V> *node = nullptr;
        while (curr != nullptr){
            node = new Node<K,V>(curr->_key, curr->_value);
            //insert begin
            if (empty()){
                _head = node;
                _tail = node;
            }
            //insert end
            else{
                _tail->_next = node;
                _tail = node;
            }
            _listSize++;
            _listUpdated = true;
            curr = curr->_next;
        }
    }
}

// Overloaded assignment operator
template <class K, class V>
const RMQList<K,V>& RMQList<K,V>::operator<<(const RMQList<K,V> &rhs){
    //self-assingment prevention
    if (this != &rhs){
        //only copy if source isn't empty
        if (!rhs.empty()){
            clear();
            Node<K,V> *curr = rhs._head;
            Node<K,V> *node = nullptr;
            while (curr != nullptr){
                node = new Node<K,V>(curr->_key, curr->_value);
                //insert begin
                if (empty()){
                    _head = node;
                    _tail = node;
                }
                //insert end
                else{
                    _tail->_next = node;
                    _tail = node;
                }
                _listSize++;
                _listUpdated = true;
                curr = curr->_next;
            }
        }
    }
    return *this;
}

// Insert an element into the list; list must be kept in increasing
// order by key; duplicate keys are not allowed, so return false if
// there is already an entry with the specified key, true otherwise.
// Should check if key > largest current key and, if so, append to
// the end of the list without iteration.
template <class K, class V>
bool RMQList<K,V>::insert(const K& key, const V& value){
    //return false if there is already an entry with the specified key
    Node<K,V> *curr = _head;
    while (curr != nullptr){
        if (curr->_key == key){
            return false;
        }
        curr = curr->_next;
    }
    Node<K,V> *node = new Node<K,V>(key, value);
    //insert when empty
    if (empty()){
        _head = node;
        _tail = node;
    }
    //insert when one or more entry in list
    else{
        //insert (append) end
        if (_tail->_key < key){
            _tail->_next = node;
            _tail = node;
        }
        //insert begin or middle
        else{
            Node<K,V> *prev = _head;
            Node<K,V> *curr = _head;
            while (curr != nullptr){
                //keeps entries sorted by increasing order
                if (curr->_key > key){
                    //insert beginning
                    if (curr == _head){
                        node->_next = _head;
                        _head = node;
                        break;
                    }
                    //insert middle
                    node->_next = curr;
                    prev->_next = node;
                    break;
                }
                prev = curr;
                curr = curr->_next;
            }
        }
    }
    _listSize++;
    _listUpdated = true;
    return true;
}

// Remove an element from the list; return false if no element
// exists with the specified key value, true otherwise
template <class K, class V>
bool RMQList<K,V>::remove(const K& key){
    if (!empty()){
        Node<K,V> *prev = _head;
        Node<K,V> *curr = _head;
        while (curr != nullptr){
            if (curr->_key == key){
                //delete begin
                if (curr == _head){
                    _head = curr->_next;
                }
                //delete elsewhere
                else{
                    prev->_next = curr->_next;
                }
                delete curr;
                _listSize--;
                _listUpdated = true;
                return true;
            }
            prev = curr;
            curr = curr->_next;
        }
    }
    return false;
}

// Update value for the element with given key; return false if
// there is no element with the given key, true otherwise
template <class K, class V>
bool RMQList<K,V>::update(const K& key, const V& value){
    Node<K,V> *curr = _head;
    while (curr != nullptr){
        if (curr->_key == key){
            curr->_value = value;
            _listUpdated = true;
            return true;
        }
        curr = curr->_next;
    }
    return false;
}

// Increase the capacity of _minArray when its full. Resizes
// to the size of the linked list
template <class K, class V>
void RMQList<K,V>::updateMinCapacity(){
    //initialize temp_array with bigger capacity
    V **temp_array = new V*[_listSize];
    for (int i = 0; i < _listSize; i++){
        temp_array[i] = new V[_listSize]; 
    }
    //copy contents of _minArray into temp_array
    for (int i = 0; i < _minCapacity; i++){
        for (int j = 0; j < _minCapacity; j++){
            temp_array[i][j] = _minArray[i][j];
        }
    }
    //clear _minArray then assign temp_array to _minArray
    for (int i = 0; i < _minCapacity; i++){
        delete [] _minArray[i];
    }
    delete [] _minArray;
    _minArray = temp_array;
    _minCapacity = _listSize;
}

// Precomputes all possible minimums for all ranges
template <class K, class V>
void RMQList<K,V>::precompute(){
    if (_listSize > _minCapacity){
        updateMinCapacity();
    }
    //update capacity of _nodeArray
    if (_listSize > _nodeArrayCapacity){
        Node<K,V> **temp_array = new Node<K,V>*[_listSize];
        for (int i = 0; i < _nodeArrayCapacity; i++){
            temp_array[i] = _nodeArray[i];
        }
        delete [] _nodeArray;
        _nodeArray = temp_array;
        _nodeArrayCapacity = _listSize;
    }
    //point _nodeArray to the nodes of the linked list
    //for easier access of indeces
    int i = 0;
    Node<K,V> *curr = nullptr;
    for (curr = _head; curr != nullptr; curr = curr->_next){
        _nodeArray[i] = curr;
        i++;
    }
    //initialize _minArray for every interval
    V min;
    for (int i = 0; i < _listSize; i++){
        min = _nodeArray[i]->_value;
        for (int j = i; j < _listSize; j++){
            if (min < _nodeArray[j]->_value){
                _minArray[i][j] = min;
            }
            else{
                min = _nodeArray[j]->_value;
                _minArray[i][j] = min;
            }
        }
    }
} 

// Perform binary search to find the index at which a key lies
template <class K, class V>
int RMQList<K,V>::findKeyIndex(const K& key){
    // p is first indx, r is last indx, q is middle indx
    int p = 0, r = _listSize - 1, q;
    while (p <= r){
        q = (p+r)/2;
        //cut array in half, search to the right
        if (_nodeArray[q]->_key < key){
            p = q+1; 
        }
        else if (_nodeArray[q]->_key == key){
            return q;
        }
        //cut array in half, search to the left
        else{
            r = q-1;
        }
    }
    return -1;
}

// RMQ Query for k1 to k2. Throws exceptions (see documentation).
template <class K, class V>
V RMQList<K,V>::query(const K& k1, const K& k2){
    if (k1 > k2){
        throw range_error("RMQList<K,V>::query(): k1 cannot be \
greater than k2.");
    }
    if (empty()){
        throw invalid_argument("RMQList<K,V>::query(): list \
is empty.");
    }
    //only recomputes if the list has changed
    if (_listUpdated){
        precompute();
        _listUpdated = false;
    }
    int i = findKeyIndex(k1), j = findKeyIndex(k2);
    if (i == -1 || j == -1){
        throw invalid_argument("RMQList<K,V>::query(): one or both \
of the keys are invalid.");
    }
    return _minArray[i][j]; 
} 

// Dump the list entries
template <class K, class V>
void RMQList<K,V>::dumpList() const {
    Node<K,V> *curr = _head;
    if (!empty()){
        while (curr != nullptr){
            cout << "Key: " << curr->_key << ", Value: " << \
            curr->_value << endl;
            curr = curr->_next;
        }
        cout << endl;
    }
    else{
        cout << "Your list is empty\n";
    }
}
 
// Dump the RMQ info and table.  What gets dumped depends on which
// RMQ method is used.
template <class K, class V>
void RMQList<K,V>::dumpTable() const {
    for (int i = 0; i < _listSize; i++){
        for (int j = 0; j < _listSize; j++){
            if (i > j){
                cout << std::setw(3) << "|" << std::setw(3);
            }
            else{
                cout << std::setw(3) << _minArray[i][j] << std::setw(3);
            }
        }
        cout << endl;
    }
    cout << endl;
}

// Clear the data data strucures
template <class K, class V>
void RMQList<K,V>::clear(){
    Node<K,V> *curr = _head;
    Node<K,V> *next = _head;
    while (curr != nullptr){
        next = curr->_next;
        delete curr;
        curr = next;
        _listSize--;
    }
    _head = nullptr;
    _tail = nullptr;
    _listUpdated = true;
}

#endif
