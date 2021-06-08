#include <iostream>
#include "BST.h"
#include <time.h>
using namespace std;

//imbalanced height checker function
bool imbalfn(int leftHeight, int rightHeight, int leftSize, int rightSize) {
    return (leftHeight > rightHeight + 1) || (rightHeight > leftHeight + 1);
}

//imbalanced size checker function
// bool imbalfn(int leftHeight, int rightHeight, int leftSize, int rightSize) {
//     return (leftSize > rightSize + 1) || (rightSize > leftSize + 1);
// }

int main() {
  
    BST bst( &imbalfn );

    cout << "**********************************************************\n";
    cout << "\ninsert return value test:\n";
    cout << "insert('A',  5) [should return 1]: " << bst.insert("A", 5) << endl;
    cout << "insert('B', 10) [should return 1]: " << bst.insert("B", 10) << endl;
    cout << "insert('B', 10) [should return 0]: " << bst.insert("B", 10) << endl;
    cout << "insert('C', 15) [should return 1]: " << bst.insert("C", 15) << endl;
    cout << "insert('D', 20) [should return 1]: " << bst.insert("D", 20) << endl;
    cout << "insert('D', 20) [should return 0]: " << bst.insert("D", 20) << endl;

    cout << "\ninsert E — L\n";
    bst.insert("E", 30);
    bst.insert("F", 40);
    bst.insert("G", 55);
    bst.insert("H", 60);
    bst.insert("I", 65);
    bst.insert("J", 70); 
    bst.insert("K", 75);
    bst.insert("L", 80);
    cout << endl;

    cout << "**********************************************************\n";
    cout << "\ndump test - bst.dump():\n";
    bst.dump();
    cout << endl;

    cout << "**********************************************************\n";
    cout << "\ndump test - bst.dump(true):\n";
    bst.dump(true);
    cout << endl;
 
    cout << "**********************************************************\n";
    cout << "\ncopy constructor test - bst_copy.dump():\n";
    BST bst_copy(bst);
    bst_copy.dump(true);
    cout << endl;

    cout << "**********************************************************\n";
    cout << "\nassignment operator test - bst_assign.dump():\n";
    BST bst_assign = bst_copy;
    bst_assign.dump(true);
    cout << endl;

    cout << "**********************************************************\n";
    cout << "\ndeep copy test - bst.insert('Z', 2) and bst.insert('Q', 500):\n";
    bst.insert("Q", 500);
    bst.insert("Z", 2);
    cout << "bst dump:\n";
    bst.dump(true);
    cout << "\nbst_copy should not change:\n";
    bst_copy.dump(true);
    cout << "\nbst_assign should not change:\n";
    bst_assign.dump(true);
    cout << endl;

    cout << "**********************************************************\n";
    cout << "\nbst.insert('R', 500):\n";
    bst.insert("R", 600);
    cout << "bst dump:\n";
    bst.dump(true);
    cout << endl;

    cout << "**********************************************************\n";
    cout << "\nassign non-empty BST to non-empty BST (bst_copy = bst) (check for memory leaks here):\n";
    bst_copy = bst;
    cout << "bst_copy dump:\n"; 
    bst_copy.dump();
    cout << endl;

    cout << "**********************************************************\n";
    cout << "\nempty_bst dump test:\n";
    BST empty_bst( &imbalfn );
    empty_bst.dump();

    cout << "**********************************************************\n";
    cout << "\nassign empty BST to non-empty BST (bst = empty_bst):\n";
    cout << "bst dump:\n";
    bst = empty_bst;
    bst.dump();

    cout << "**********************************************************\n";
    cout << "\nself-assignment test (bst_copy = bst_copy):\n";
    cout << "bst_copy dump:\n";
    bst_copy = bst_copy;
    bst_copy.dump();
    cout << endl;

    cout << "**********************************************************\n";
    cout << "\ninsert into bst_copy and test for balancing:\n";
    cout << "bst_copy.insert('M', 100) and bst_copy.insert('O', 105)\n";
    bst_copy.insert("M", 100);
    bst_copy.insert("O", 105);
    bst_copy.dump(true);
    cout << endl;

    cout << "**********************************************************\n";
    cout << "\ninsert keys in randomized order test:\n";
    BST bst_rand( &imbalfn );
    bst_rand.insert("K", 75);
    bst_rand.insert("C", 15);
    bst_rand.insert("A", 5);
    bst_rand.insert("E", 30);
    bst_rand.insert("J", 70); 
    bst_rand.insert("F", 40);
    bst_rand.insert("G", 55);
    bst_rand.insert("D", 20);
    bst_rand.insert("B", 10);
    bst_rand.insert("H", 60);
    bst_rand.insert("L", 80);
    bst_rand.insert("I", 65);

    cout << "bst_rand.dump()\n";
    bst_rand.dump();

    cout << "\nbst_rand.dump(true):\n";
    bst_rand.dump(true);
    cout << endl;

    cout << "**********************************************************\n";
    cout << "\nstress test - insert 10,000 nodes\n";
    srand (time(NULL));
    BST new_bst ( &imbalfn );
    char alphabet[26] = {'A', 'B', 'C', 'D', 'E', 'F', 'G', 
                         'H', 'I', 'J', 'K', 'L', 'M', 'N',  
                         'O', 'P', 'Q', 'R', 'S', 'T', 'U', 
                         'V', 'W', 'X', 'Y', 'Z'};
    string data = ""; //string data to be inserted
    int n = 10000; //number of inserts
    for (int i = 0; i < n; i++) {
        //generate random combination of letters w/ length of 3 (eg. ABC, FLJ...)
        for (int i = 0; i < 3; i++) {
            data += alphabet[rand() % 26];
        }
        new_bst.insert(data, rand() % 1000000);
        data = "";
    }
    cout << "\ndump test - bst.dump():\n";
    new_bst.dump();
    cout << endl;
    cout << "\ndump test - bst.dump(true):\n";
    new_bst.dump(true);
    cout << endl; 
    return 0;
}
