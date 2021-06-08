#include <iostream>
#include "Graph.h"

using std::cout;
using std::endl;
using std::exception;
using std::invalid_argument

// Constructor - DO NOT MODIFY
Graph::Graph(int n) {
  if (n <= 0)
    throw invalid_argument("Graph::Graph(): number of vertices must be positive");

  _rows = new EntryList*[n];
  for (int i = 0; i < n; i++) {
    _rows[i] = new EntryList();
  }
  _numVert = n;
  _numEdge = 0;
}

//copy constructor
Graph::Graph(const Graph& G) {
    _numVert = G._numVert;
    _numEdge = G._numEdge;
    _rows = new EntryList*[_numVert];
    for (int i = 0; i < _numVert; i++) {
       _rows[i] = new EntryList(*G._rows[i]);
    }
}

//assignment operator
const Graph& Graph::operator=(const Graph& rhs) {
    if (this != &rhs){
        for (int i = 0; i < _numVert; i++){
            delete _rows[i];
        }
        delete [] _rows;
        _numVert = rhs._numVert;
        _numEdge = rhs._numEdge;
        _rows = new EntryList*[_numVert];
        for (int i = 0; i < _numVert; i++) {
            _rows[i] = new EntryList(*rhs._rows[i]);
        }
    }
    return *this;
}

//destructor
Graph::~Graph() {
    for (int i = 0; i < _numVert; i++){
        delete _rows[i];
    }
    delete [] _rows;
}

// Number of vertices - DO NOT MODIFY
int Graph::numVert() const {
    return _numVert;
}

// Number of edges - DO NOT MODIFY
int Graph::numEdge() const {
    return _numEdge;
}

// Add an edge between u and v with weight x.  Throws
// invalid_argument if u or v is not a valid vertex number.
void Graph::addEdge(int u, int v, weight_t x) {
    if (u < 0 || v < 0 || u >= _numVert || v >= _numVert){ 
        throw invalid_argument("Graph::addEdge(): u or v is not a valid vertex number\n");
    }
    _rows[u]->insert( EntryList::Entry(v, x) );
    _numEdge++;
}

// Remove the edge (u, v).  Returns 'true' if an edge is removed;
// 'false' if there is no edge (u,v). Throws invalid_argument if u
// or v is not a valid vertex number
bool Graph::removeEdge(int u, int v) {
    if (u < 0 || v < 0 || u >= _numVert || v >= _numVert){ 
        throw invalid_argument("Graph::removeEdge(): u or v is not a valid vertex number\n");
    }
    EntryList::Entry e;
    bool removed = _rows[u]->remove(v, e);
    if (removed == true){
        --_numEdge;
        return true;
    }
    return false;
}

// Dump the graph - DO NOT MODIFY
void Graph::dump() const {
    cout << "Dump of graph (numVert = " << _numVert
        << ", numEdge = " << _numEdge << ")" << endl;

    for (int i = 0; i < _numVert; i++) {
        cout << "Row " << i << ": \n";
        _rows[i]->dump();
    }
}

// Edge Iterator constructor.
//   if Gptr is nullptr, create an unitialized iterator.
//   if Gptr points to a host Graph object:
//      if enditr == false, create a begin() iterator.
//      if enditr == true, create an end() iterator.
Graph::EgIterator::EgIterator(Graph *Gptr, bool enditr) {
    _Gptr = Gptr;
    if (Gptr == nullptr){
        //create uninitialized iterator
        _row = 0;
        _itr = EntryList::Iterator();
    }
    else{
        //true [end()]
        if (enditr == true){
            //create iterator at the beginning of edge
            int indx = Gptr->numVert() - 1;
            while (indx >= 0){
                if (Gptr->_rows[indx]->size() != 0){
                    _row = indx + 1;
                    break;
                }
                indx--;
            }
            _itr = Gptr->_rows[_row]->end();
        }
        //false [begin()]
        else{
            int indx = 0;
            while (indx < Gptr->numVert()){
                if (Gptr->_rows[indx]->size() != 0){
                    _row = indx;
                    break;
                }
                indx++;
            }
            _itr = Gptr->_rows[_row]->begin();
        }
    }
}
  
// Return edge at iterator location as a tuple (u, v, weight).
// Throws invalid_argument if the iterator is uninitialized or if
// derefrence of _itr failes.
tuple<int,int,weight_t> Graph::EgIterator::operator*() {
    if (_itr == nullptr){ 
        throw invalid_argument("EgIterator::operator*(): Iterator is uninitialized\n");
    }
    int u, v;
    weight_t x;  
    u = this->_row;
    v = (*_itr).EntryList::Entry::getVertex();
    x = (*_itr).EntryList::Entry::getWeight();
    return std::make_tuple(u, v, x);
}

// Compare two iterators.  Mainly used to end a for loop using the
// iterator.
bool Graph::EgIterator::operator!=(const EgIterator& rhs) {
    return (_Gptr != rhs._Gptr || _row != rhs._row || _itr != rhs._itr);
}

// Advanced the iterator to the next edge; if already at the end()
// iterator, leave unchanged.  Throws invalid_argument if the
// iterator is uninitialized.
void Graph::EgIterator::operator++(int dummy) {
    if (_itr == nullptr){ 
        throw invalid_argument("EgIterator::operator++(): Iterator is uninitialized\n");
    }
    EntryList::Iterator temp_itr = _itr;
    temp_itr++; //prevents the iterator from moving beyong EntryList bounds

    //not at the end of row and not at the end of EntryList
    if ((_row <= _Gptr->_numVert - 1) && (temp_itr != _Gptr->_rows[_row]->end())){
        _itr++;
    }
    //not at the end of row but at the end of EntryList
    else if ((_row <= _Gptr->_numVert - 1) && (temp_itr == _Gptr->_rows[_row]->end())){
        _row++;
        _itr = _Gptr->_rows[_row]->begin();
    }
    //end of row and not at the end of EntryList
    else if ((_row == _Gptr->_numVert - 1) && (temp_itr != _Gptr->_rows[_row]->end())){
        _itr++;
    }
    //end of row and the end of EntryList
    else if ((_row == _Gptr->_numVert - 1) && (temp_itr == _Gptr->_rows[_row]->end())){
        _row++;
    }
}

// Create an initial edge Iterator.
Graph::EgIterator Graph::egBegin() {
    return Graph::EgIterator(this, false);
}
 // Create an end edge Iterator.
Graph::EgIterator Graph::egEnd() {
    return Graph::EgIterator(this, true);
}
// Constructor for iterator for vertices adjacent to vertex v.
    //   If Gptr == nullptr, creat an uninitialized iterator.
    //   If Gptr points to a host Graph object:
    //      If enditr == true, create an end iterator for row v.
    //      If enditr == false, crete a begin iterator for row v.
    // Throws invalid_argument if v is not a valid vertex number.
Graph::NbIterator::NbIterator(Graph *Gptr, int v, bool enditr) {
    //initialize member variables
    _Gptr = Gptr;
    _row = v;
    //create uninitialized iterator
    if (Gptr == nullptr){
        _row = 0;
        _itr = EntryList::Iterator();
    }
    //set iterator to point to end or beginning of list
    else{
        if (v < 0 || v >= _Gptr->_numVert){
            throw invalid_argument("NbIterator::NbIterator(): v is not a valid node index");
        }
        //true [end()]
        if (enditr == true){
            //create iterator at the end of neibhor
            _itr = Gptr->_rows[_row]->end();
        }
        //false [begin()]
        else{
            //create iterator at the beginning of neighbor
            _itr = Gptr->_rows[_row]->begin();
        }
    }
}

// Compare iterators.
bool Graph::NbIterator::operator!=(const NbIterator& rhs) {
    return (_Gptr != rhs._Gptr || _row != rhs._row || _itr != rhs._itr);
}

// Advance iterator to the next neighbor.
void Graph::NbIterator::operator++(int dummy) {
    _itr++;
}

// Return neighbor and weight at current iterator position. Throws
// invalid_argument if the iterator is null or invalid.
pair<int,weight_t> Graph::NbIterator::operator*() {
    if (_itr == nullptr || _row < 0 || _row > _Gptr->_numVert){
        throw invalid_argument("NbIterator::operator*(): iterator is null or invalid\n");
    }
    int v;
    weight_t x;
    v = (*_itr).EntryList::Entry::getVertex();
    x = (*_itr).EntryList::Entry::getWeight();
    return std::make_pair(v, x);
}

// Make an initial neighbor iterator for row v.  Throws
// invalid_argument if v is not a valid node index.
Graph::NbIterator Graph::nbBegin(int v) {
    if (v < 0 || v > _numVert){
        throw invalid_argument("Graph::nbBegin(): v is not a valid node index");
    }
    return Graph::NbIterator(this, v, false);
}

// Make an end neighbor iterator. Throws invalid_argument if v is
// not a valid node index.
Graph::NbIterator Graph::nbEnd(int v) {
    if (v < 0 || v > _numVert){
        throw invalid_argument("Graph::nbEnd(): v is not a valid node index");
    }
    return Graph::NbIterator(this, v, true);
}