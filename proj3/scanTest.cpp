#include <vector>
#include "Scanner.h"

using namespace std;

int main() {
    // Characters used in the ASCII art, in order.
    vector<string> chars = {"█","░"};

    cout << "**********************************************************\n";
    cout << "\ntest with non-existing files (s.loadFiles('asdfghjkl.txt', 'lkjhgfdsa.txt')):\n\n";
    Scanner s(15, 34, chars);
    s.loadFiles("asdfghjkl.txt", "lkjhgfdsa.txt");
    cout << endl;

    cout << "**********************************************************\n";
    cout << "\ntest with existing files (s.loadFiles('scrambled.txt', 'weights.txt')):\n\n";
    // Scanner with 15 lines, 34 characters per line, 2 characters
    s.loadFiles("scrambled.txt", "weights.txt");
    // Read data files
    s.dump();
    cout << endl;
    s.inorder();
    cout << endl;

    cout << "\n**********************************************************\n";
    cout << "\ntest with different ASCII art (vector<string> chars = {'+','-'}):\n\n";
    vector<string> other_chars = {"-","|"}; 
    Scanner scan(15, 34, other_chars);
    scan.loadFiles("scrambled.txt", "weights.txt");
    scan.dump();
    cout << endl;
    s.inorder();
    cout << endl;

    cout << "\n**********************************************************\n";
    cout << "\ncopy constructor test (copy 's' Scanner) - scan_copy.dump():\n";
    Scanner scan_copy(s);
    scan_copy.dump();
    cout << endl;
    scan_copy.inorder();
    cout << endl;

    cout << "\n**********************************************************\n";
    cout << "\nassignment operator test (copy 'scan' Scanner) - scan_assign.dump():\n";
    Scanner scan_assign(scan);
    scan_assign.dump();
    cout << endl;
    scan_assign.inorder();
    cout << endl;
    
    return 0;
}