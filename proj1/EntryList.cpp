#include "EntryList.h"

using std::cout;
using std::endl;
using std::range_error;

// Constructor - DO NOT MODIFY
EntryList::EntryList() {
  _array = new Entry[DEFAULT_SIZE];
  _capacity = DEFAULT_SIZE;
  _size = 0;
}

//copy constructor
EntryList::EntryList(const EntryList& rhs) {
    _capacity = rhs._capacity;
    _array = new Entry[_capacity];
    _size = rhs._size;
    for (int i = 0; i < _size; i++){
        _array[i] = rhs._array[i];
    }
} 

//overloaded assignment operator
const EntryList& EntryList::operator=(const EntryList& rhs) {
    if (this != &rhs){
        delete [] _array;
        _capacity = rhs._capacity;
        _array = new Entry[_capacity];
        _size = rhs._size;
        for (int i = 0; i < _size; i++){
            _array[i] = rhs._array[i];
        }
    }
    return *this;
}

//destructor
EntryList::~EntryList() {
    delete [] _array;
}

// Insert the entry e.  The elements of the list must be kept in
// increasing order by vertex.  If inserting the new element would
// exceed the capacity of the EntryList, then the array must be
// expanded, doubling the capacity.  Returns 'true' if new entry
// inserted, 'false' if there is already an entry with the same
// vertex as e.
bool EntryList::insert(const Entry& e) {
    //double capacity if array full
    if (_size == _capacity){
        Entry *temp_array = new Entry[_capacity*2];
        for (int i = 0; i < _size; i++){
            temp_array[i] = _array[i];
        }
        delete [] _array;
        _array = temp_array;
        _capacity *= 2;
    }
    //entry with same vertex already exists in entry list
    for (int i = 0; i < _size; i++){
        if (_array[i]._vertex == e._vertex){
            return false;
        }
    }
    //empty 
    if (_size == 0){
        _array[0] = e;
        _size++;
    }
    //not empty
    else {
        int insrt_indx;
        //find index to insert
        for (insrt_indx = 0; insrt_indx < _size; insrt_indx++){
            if (_array[insrt_indx]._vertex > e._vertex){
                break;
            }
        }
        //shift right leaving space for entry to be inserted
        for (int i = _size - 1; i >= insrt_indx; --i){
            _array[i+1] = _array[i];
        }
        //insert
        _array[insrt_indx] = e;
        _size++;
    }
    return true;
}
  
bool EntryList::update(const Entry& e) {
    for (int i = 0; i < _size; i++){
        if (_array[i]._vertex == e._vertex){
            _array[i] = e;
            return true;
        }
    }
    return false;
}

bool EntryList::getEntry(int vertex, Entry &ret) {
    for (int i = 0; i < _size; i++){
        if (_array[i]._vertex == vertex){
            ret = _array[i];
            return true;
        }
    }
    return false;
}

bool EntryList::remove(int vertex, Entry &ret) {
    bool removed = false;
    int rmv_indx;
    for (rmv_indx = 0; rmv_indx < _size; rmv_indx++){
        if (_array[rmv_indx]._vertex == vertex){
            ret = _array[rmv_indx]; //returns removed entry
            removed = true;
            break;
        }
    }
    //return true if entry will be removed
    if (removed == true){
        for (int i = rmv_indx; i < _size; i++){
            _array[i] = _array[i+1];
        }
        _size--;
        //cut capacity in half if size is less than 1/4th capacity
        //capacity is never reduced below DEFAULT_SIZE
        if (_size < ((0.25)*_capacity) && _capacity != DEFAULT_SIZE){
            Entry *temp_array = new Entry[_capacity/2];
            for (int i = 0; i < _size; i++){
                temp_array[i] = _array[i];
            }
            delete [] _array;
            _array = temp_array;
            _capacity /= 2;
        }
        return true;
    }
    return false;
}

EntryList::Entry& EntryList::at(int indx) const {
    if (indx < 0 || indx > _size){
        throw range_error("EntryList::at(): index must be less than or equal to size");
    }
    return _array[indx];  
}

// dump data structure - DO NOT MODIFY
void EntryList::dump() {
  for (int i = 0; i < _size; i++) {
    cout << "  " << _array[i] << endl;
  }
}

EntryList::Iterator::Iterator(EntryList *EList, int indx) {
    _ELptr = EList;
    _indx = indx;
}

bool EntryList::Iterator::operator!=(const EntryList::Iterator& rhs) {
    return (_ELptr != rhs._ELptr || _indx != rhs._indx);
}

bool EntryList::Iterator::operator==(const EntryList::Iterator& rhs) {
    return (_ELptr == rhs._ELptr && _indx == rhs._indx);
}

void EntryList::Iterator::operator++(int dummy) {
    _indx++;
}

EntryList::Entry EntryList::Iterator::operator*() {
    return _ELptr->_array[_indx];
}

EntryList::Iterator EntryList::begin() {
    return EntryList::Iterator(this, 0);
}

EntryList::Iterator EntryList::end() {
    return EntryList::Iterator(this, _size);
}

// Insertion operator for Entry objects - DO NOT MODIFY
ostream& operator<<(ostream& sout, const EntryList::Entry& e) {
  sout << e._vertex << ": " << e._weight;
  return sout;
}

// A convenient way to write test code for a single class is to write
// a main() function at the bottom of the .cpp file.  Just be sure to
// comment-out main() once you are done testing!

// Following is example test code.  There is no guarantee that it is
// complete -- you are responsbile for thoroughly testing your code.
// In particular, passing these tests does not mean your
// implementation will pass all grading tests.
/*
int main() {
    EntryList el;
    EntryList::Entry e;

    cout << "size: " << el.size() << ", capacity: " << el.capacity() << endl;
    el.dump();
    cout << endl;

    for (int i = 1; i < 13; i++) {
        el.insert( EntryList::Entry((i*5)%13, i) );
    }

    cout << endl;
    cout << "size: " << el.size() << ", capacity: " << el.capacity() << endl;
    el.dump();
    cout << endl;
    for (int i = 1; i < 13; i+=2) {
        el.remove(i, e);
    }

    cout << "size: " << el.size() << ", capacity: " << el.capacity() << endl;
    el.dump();
    cout << endl;


    for (int i = 2; i < 13; i+=2) {
        el.update( EntryList::Entry(i, 2*i) );
    }

    cout << "size: " << el.size() << ", capacity: " << el.capacity() << endl;
    el.dump();
    cout << endl;

    for (int i = 3; i < 13; i*=2) {
        el.remove(i, e);
    }

    cout << "size: " << el.size() << ", capacity: " << el.capacity() << endl;
    el.dump();
    cout << endl;

    cout << "\nPrint using iterator:\n";
    for (auto itr = el.begin(); itr != el.end(); itr++) {
        cout << *itr << endl;
    }


    return 0;
}
//*/
