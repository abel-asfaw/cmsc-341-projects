// CMSC 341 - Spring 2020 - Project 0
// stack.h: a templated, linked list-based stack implementation

// Assignment:
//   (1) Complete the copy constructor
//   (2) Complete the assignment operator
//   (3) Complete the destructor
//   (4) Write a test program (mytest.cpp) to test copy and assignment
//   (5) Verify destructor by running the test program in Valgrind

#ifndef _STACK_H
#define _STACK_H

#include <iostream>
#include <exception>
using namespace std;

using std::ostream;
using std::cout;
using std::endl;
using std::range_error;

// Forward declarations
template <class T> class Stack;
template <class T> class Node;
template <class T> ostream& operator<<(ostream&, const Node<T>&);

// Node class for linked list
template <class T>
class Node {

  friend Stack<T>;
  
public:
  Node(T data = T(), Node<T> *next = nullptr) {
    _data = data;
    _next = next;
  }

  friend ostream& operator<< <T>(ostream &sout, const Node<T> &x);
  
private:
  T _data;
  Node *_next;
};

// Overloaded insertion operator.  Must be overloaded for the template
// class T, or this won't work!
template <class T>
ostream& operator<<(ostream &sout, const Node<T> &x) {
  sout << "Data: " << x._data;
  return sout;
}


// Stack class.  Linked-list implementation of a stack. Uses the Node
// class.
template <class T>
class Stack {
public:
  // Constructor
  Stack();

  // Copy constructor, assignment operator, and destructor
  // DO NOT IMPLEMENT HERE.  SEE BELOW.
  Stack(const Stack &rhs);
  const Stack& operator=(const Stack& rhs);
  ~Stack();

  // Stack operations: push(), top(), and pop()
  void push(const T& data);
  const T& top() const;
  void pop();

  // Helpful functions
  bool empty() const;  // Returns 'true' if stack is empty
  void dump() const;   // Dump contents of the linked list

private:
  Node<T> *_head;
  // ***************************************************
  // Any private helper functions must be delared here!
  // ***************************************************
  
};

template <class T>
Stack<T>::Stack() {
  _head = nullptr;
}

//copy constructor
template <class T>
Stack<T>::Stack(const Stack<T>& rhs) {
    _head = nullptr;
    Node<T>* curr = rhs._head;
    //avoid copy if source stack is empty
    if (rhs._head != nullptr){
        while (curr != nullptr){
            //insert beginning
            if (empty()){
                Node<T> *node = new Node<T>(curr->_data);
                node->_next = _head;
                _head = node;
            }
            //insert end
            else{
                Node<T> *node = new Node<T>(curr->_data);
                Node<T> *curr = _head;
                Node<T> *prev = _head;
                while (curr != nullptr){
                    prev = curr;
                    curr = curr->_next;
                }
                node->_next = curr;
                prev->_next = node;
            }
            curr = curr->_next;
        }
    }
}

//copy assignment operator
template <class T>
const Stack<T>& Stack<T>::operator=(const Stack<T>& rhs) {
    //self-assignment guard
    if (this != &rhs){
        _head = nullptr;
        Node<T>* curr = nullptr;
        curr = rhs._head;
        //avoid copy if source stack is empty
        if (curr != nullptr){
            while (curr != nullptr){
                //insert beginning
                if (empty()){
                    Node<T> *node = new Node<T>(curr->_data);
                    node->_next = _head;
                    _head = node;
                }
                //insert end
                else{
                    Node<T> *node = new Node<T>(curr->_data);
                    Node<T> *curr = _head;
                    Node<T> *prev = _head;
                    while (curr != nullptr){
                        prev = curr;
                        curr = curr->_next;
                    }
                    node->_next = curr;
                    prev->_next = node;
                }
                curr = curr->_next;
            }
        }
    }
    return *this;
}

//destructor
template <class T>
Stack<T>::~Stack() {
    Node<T>* curr = _head;
    Node<T>* next = _head;
    while (curr != nullptr){
        next = curr->_next;
        delete curr;
        curr = next;
    }
    _head = nullptr;
}

template <class T>
void Stack<T>::push(const T& data) {
  Node<T> *node = new Node<T>(data);
  node->_next = _head;
  _head = node;
}

template <class T>
const T& Stack<T>::top() const {
  if ( empty() ) {
    throw range_error("Stack<T>::top(): attempt to read empty stack.");
  }

  return _head->_data;
}

template <class T>
void Stack<T>::pop() {
  if ( empty() ) {
    throw range_error("Stack<T>::pop(): attempt to pop from an empty stack.");
  }

  Node<T> *node = _head->_next;
  delete _head;
  _head = node;
}

template <class T>
bool Stack<T>::empty() const {
  return _head == nullptr;
}

template <class T>
void Stack<T>::dump() const {
  Node<T> *nodePtr = _head;
  while ( nodePtr != nullptr ) {
    cout << nodePtr->_data << endl;
    nodePtr = nodePtr->_next;
  }
}

#endif
