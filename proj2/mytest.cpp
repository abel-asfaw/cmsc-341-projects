// CMSC 341 - Spring 2020 - Project 2
//
// driver.cpp - a simple driver for the RMQList class
//
// NOTE: This program uses int keys and values.  You should be sure to
// test RMQList with *different* key and value types.
//
// You are allowed to use STL containers and libraries in your test
// program.
 
#include <iostream>
#include <vector>
#include <set>
#include <random>
#include <functional>
#include <algorithm>
#include "rmqlist.h"

using namespace std;

// *******************************************************
// KeyVal: a simple class to store integer key/value pairs
// *******************************************************
class KeyVal {
public:
  KeyVal() { _data = pair<int,int>(0,0); }
  KeyVal(int key, int val) { _data = pair<int,int>(key, val); }
  int getKey() const { return _data.first; }
  int getVal() const { return _data.second; }
private:
  pair<int,int> _data;
};

// Function prototypes
bool operator<(const KeyVal& kv1, const KeyVal& kv2) { return kv1.getKey() < kv2.getKey(); }
int minimum(const vector<KeyVal> &data, int first, int last);
int bsearch(const vector<KeyVal> &A, int key);

// **********************************************
// main() - demonstrates use of the RMQList class
// **********************************************
int main() {

    // A small example.  Insert (keys[i], i) into an RMQList object
    vector<int> keys = {34, 16, 58, -24, 53, 7, 97, 92, -12, 45, 9, 0, -1, 20, 77};

    RMQList<int,int> rmq;

    cout << "\ninsert data:\n";
    for (int i = 0; i < int(keys.size()); i++) {
        cout << "  key = " << keys[i] << ", value = " << i << endl;
        rmq.insert(keys[i], i);
    }

    cout << "\ndump list:\n";
    rmq.dumpList();

    cout << "**********************************************************\n";
    cout << "\nduplicate insert test + bool return test:\n";
    cout << "\ninsert(16, 23) should return 0:  " << rmq.insert(16, 23);
    cout << "\ninsert(99, 2) should return 1:   " << rmq.insert(99, 2);
    cout << "\ninsert(-12, 43) should return 0: " << rmq.insert(-12, 43) << endl;
    cout << "\ndump List:\n";
    rmq.dumpList();

    cout << "**********************************************************\n";
    cout << "\ncopy constructor test (rmq_copy dump):\n";
    cout << "\ndump list:\n";
    RMQList<int,int> rmq_copy(rmq);
    rmq_copy.dumpList();

    cout << "**********************************************************\n";
    cout << "\nassignment operator test (rmq_asgn dump):\n";
    cout << "\ndump list:\n";
    RMQList<int,int> rmq_asgn = rmq;
    rmq_asgn.dumpList();
    
    cout << "**********************************************************\n";
    cout << "\nremove + bool return test for rmq:\n";
    cout << "\nremove(92) should return 1:  " << rmq.remove(92);
    cout << "\nremove(-24) should return 1: " << rmq.remove(-24);
    cout << "\nremove(9) should return 1:   " << rmq.remove(9);
    cout << "\nremove(92) should return 0:  " << rmq.remove(92);
    cout << "\nremove(9) should return 0:   " << rmq.remove(9) << endl;

    cout << "\ndump list:\n";
    rmq.dumpList();

    cout << "**********************************************************\n";
    cout << "\nupdate + bool return test for rmq:\n";
    cout << "\nupdate(77,100) should return 1: " << rmq.update(77,100);
    cout << "\nupdate(7,5555) should return 1: " << rmq.update(7,5555);
    cout << "\nupdate(0,99) should return 1:   " << rmq.update(0,99);
    cout << "\nupdate(2,87) should return 0:   " << rmq.update(2,87);
    cout << "\nupdate(50,7) should return 0:   " << rmq.update(50,7) << endl;

    cout << "\ndump list:\n";
    rmq.dumpList();

    cout << "**********************************************************\n";
    cout << "\ncopy constructor deep copy test for rmq_copy:\n";
    cout << "\ndump list:\n";
    rmq_copy.dumpList();

    cout << "**********************************************************\n";
    cout << "\nassignment operator deep copy test for rmq_asgn:\n";
    cout << "\ndump list:\n";
    rmq_asgn.dumpList();

    cout << "**********************************************************\n";
    cout << "\ntest for correct size for rmq (should return 13):\n";
    cout << rmq.size() << endl;
    cout << "\ndump list:\n";
    rmq.dumpList();

    cout << "**********************************************************\n";
    cout << "\nassign a non-empty list to another non-empty list (rmq_asgn \
to rmq) test (rmq dump):\n";
    rmq = rmq_asgn;
    cout << "\ndump list:\n";
    rmq.dumpList();

    cout << "**********************************************************\n";
    cout << "\nassign an empty list to  non_empty list (rmq_empty1 \
to rmq_copy) test (rmq_copy dump):\n";
    RMQList<int,int> rmq_empty1;
    RMQList<int,int> rmq_empty2;
    rmq_copy = rmq_empty2;
    cout << "\ndump list:\n";
    rmq_copy.dumpList();

    cout << "**********************************************************\n";
    cout << "\nRMQ query:\n";
    cout << "  query(45,77) = " << rmq.query(45, 77) << endl;

    cout << "\ndump RMQ table info:\n";
    rmq.dumpTable();

    cout << "**********************************************************\n";
    cout << "\nRMQ query:\n";
    cout << "  query(0,20) = " << rmq.query(0,20) << endl;

    cout << "\ndump RMQ table info:\n";
    rmq.dumpTable();

    cout << "**********************************************************\n";
    cout << "\nRMQ query:\n";
    cout << "  query(77,97) = " << rmq.query(77,97) << endl;

    cout << "\ndump RMQ table info:\n";
    rmq.dumpTable();

    cout << "**********************************************************\n";
    cout << "\nRMQ query:\n";
    cout << "  query(20,53) = " << rmq.query(20,53) << endl;

    cout << "\ndump RMQ table info:\n";
    rmq.dumpTable();

    cout << "**********************************************************\n";
    cout << "\nRMQ query:\n";
    cout << "  query(-24,-12) = " << rmq.query(-24,-12) << endl;

    cout << "\ndump list:\n";
    rmq.dumpList();

    cout << "**********************************************************\n";
    cout << "\nRMQ query:\n";
    cout << "  query(-12,0) = " << rmq.query(-12,0) << endl;

    cout << "\ndump list:\n";
    rmq.dumpList();

    cout << "**********************************************************\n";
    cout << "\nRMQ query:\n";
    cout << "  query(0,9) = " << rmq.query(0,9) << endl;

    cout << "\ndump list:\n";
    rmq.dumpList();

    cout << "**********************************************************\n";
    cout << "\nremove(92) should return 1:  " << rmq.remove(92);
    cout << "\nremove(-24) should return 1: " << rmq.remove(-24);
    cout << "\nremove(9) should return 1:   " << rmq.remove(9) << endl;

    cout << "\ndump list:\n";
    rmq.dumpList();

    cout << "**********************************************************\n";
    cout << "\nRMQ query:\n";
    cout << "  query(-12,0) = " << rmq.query(-12,0) << endl;

    cout << "\ndump RMQ table info:\n";
    rmq.dumpTable();

    cout << "**********************************************************\n";
    cout << "\nRMQ query on rmq_asgn (test on copied lsit):\n";
    cout << "  query(-12,0) = " << rmq_asgn.query(-12,0) << endl;

    cout << "\ndump rmq_asgn table info:\n";
    rmq_asgn.dumpTable();
    /*
    // this works!
    cout << "\nRMQ query:\n";
    cout << "  query(10,11) should throw exception:\n" << \
rmq.query(10,11) << endl;

    cout << "\ndump list:\n";
    rmq.dumpList();

    // this also works!
    cout << "\nRMQ query:\n";
    cout << "  query(53,20) should throw exception:\n" << \
rmq.query(53,20) << endl;

    // cout << "\ndump RMQ table info:\n";
    // rmq.dumpTable();
    */
    cout << "\nclear and dump list:\n";
    rmq.clear();
    rmq.dumpList();

    // A medium-sized test.  Insert ~1000 entries. Check the result of
    // query() against the "brute force" RMQ solution.
    cout << "*****************************************\n";
    cout << "insert lots of data; check speed of query\n";
    cout << "*****************************************\n";
/*
    const int NUMGEN = 1000;

    cout << "\nInsert " << NUMGEN << " random key/value pairs.\n";

    // This is all random number generation stuff
    // Seed sequences for random number generators
    std::seed_seq seed1 = {31, 41, 59};
    std::seed_seq seed2 = {27, 18, 28};
    // Mersenne twister random number engine
    mt19937 engine1(seed1), engine2(seed2);
    uniform_int_distribution<int> key_dist(-15000, 15000);
    auto key_gen = bind(key_dist, engine1);
    uniform_int_distribution<int> value_dist(0, 50000);
    auto value_gen = bind(value_dist, engine2);

    // Generate the data; store in a vector as well as inserting into
    // the RMQList.  The set is used to check if a key has already
    // occurred so that we don't insert duplicate keys into the vector.

    vector<KeyVal> kv;
    set<int> keySet;

    for ( int i = 0; i < NUMGEN; i++ )  {
        int key = key_gen();                 // generate a key
        int value = value_gen();             // generate a new value
        if ( keySet.count( key ) == 0 ) {    // if it's a new key...
        kv.push_back(KeyVal(key, value));  // ...put it in the vector
        keySet.insert(key);                // ...and in the set
        }
        rmq.insert( key, value );            // insert (key,value) in the
                        // RMQList
    }
    rmq.dumpList();
    std::sort(kv.begin(), kv.end());       // Sort the vector by key
                        // values.  Need to do this
                        // so that it is in the same
                        // order as the RMQList

    cout << "\nTotal of " << kv.size() << " key/value pairs inserted.\n";

    // Test ALL possible queries.  Compare the answer from query() with
    // the brute force computation from minimum().  This can be slow.
    cout << "\nTesting correctness of query()...";
    for (int i = 0; i < int(kv.size()-1); i++) {
        for (int j = i+1; j < int(kv.size()); j++) {
            int rmqq = rmq.query( kv[i].getKey(), kv[j].getKey() );
            int minq = minimum(kv, kv[i].getKey(), kv[j].getKey() );
            if (rmqq != minq) {
            cout << "Fail! (" << i << ", " << j << "); rmq ="
                << rmqq << ", minq = " << minq << endl;
            }
        }   
    }
    cout << "done!\n";

    //rmq.dumpTable();
    */
    return 0;
}

int minimum(const vector<KeyVal>& data, int k1, int k2) {
    int indx1 = bsearch(data, k1);
    int indx2 = bsearch(data, k2);
    if (indx1 > indx2) swap(indx1, indx2);

    int minVal = data[indx1].getVal();
    for (int i = indx1+1; i <= indx2; i++) {
        minVal = ( data[i].getVal() < minVal ? data[i].getVal() : minVal );
    }
    return minVal;
}

int bsearch(const vector<KeyVal>& A, int key) {
    int n = A.size();
    int p = 0, r = n - 1;
    int q;

    while (p != r) {
        q = (p + r) / 2;
        if (key <= A[q].getKey()) {
            r = q;
        } else {
            p = q + 1;
        }
    }

    if (key == A[p].getKey()) {
        return p;
    } else {
        return -p;
    }
}
 
