#include <iostream>
#include <tuple>
#include "Graph.cpp"
#include "EntryList.cpp"

using namespace std;

int main() {
    // ENTRYLIST TEST
    cout << "*********ENTRYLIST TEST*********\n";
    EntryList el;
    EntryList::Entry e;

    cout << "size: " << el.size() << ", capacity: " << el.capacity() << endl;
    el.dump();
    cout << endl;

    //test insert function
    for (int i = 1; i < 13; i++) {
        el.insert( EntryList::Entry((i*5)%13, i) );
    }

    cout << endl;
    cout << "size: " << el.size() << ", capacity: " << el.capacity() << endl;
    el.dump(); 
    cout << endl;

    //copy constructor test
    cout << "copy constructor test\n";
    EntryList fl(el);

    cout << "size: " << fl.size() << ", capacity: " << fl.capacity() << endl;
    fl.dump();
    cout << endl;

    //assignment operator test
    cout << "assignment operator test\n";
    EntryList rl = el;

    cout << "size: " << rl.size() << ", capacity: " << rl.capacity() << endl;
    rl.dump();
    cout << endl;

    //copy and assignment function correctly when the source object is empty
    cout << "copy and assignment function correctly when the source object is empty\n";
    EntryList ll;
    EntryList ml = ll;

    cout << "size: " << ml.size() << ", capacity: " << ml.capacity() << endl;
    ml.dump();
    cout << endl;

    //test for duplicates
    cout << "duplicate insert test:\n";
    for (int i = 1; i < 13; i++) {
        el.insert( EntryList::Entry((i*5)%13, i) );
    }

    //getEntry() returns the desired entry in the ret reference variable
    cout << "getEntry() returns the desired entry in the ret reference variable\n";
    EntryList::Entry m;
    el.getEntry(1, m);
    cout << "m:\n";
    cout << m << endl;


    cout << "size: " << el.size() << ", capacity: " << el.capacity() << endl;
    el.dump();
    cout << endl;

    //test remove function
    for (int i = 1; i < 13; i+=2) {
        el.remove(i, e);
    }

    cout << "size: " << el.size() << ", capacity: " << el.capacity() << endl;
    el.dump();
    cout << endl;

    //test update function
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

    // GRAPH TEST
    cout << "*********GRAPH TEST*********\n";
    // G has 5 vertices numbered 0 thru 4
    Graph G(5) ; 

    // add some edges
    G.addEdge(3,4,11);
    G.addEdge(1,4,12);
    G.addEdge(0,3,13);
    G.addEdge(0,4,14);
    G.addEdge(0,1,15);
    G.addEdge(1,2,16);
    G.addEdge(2,4,17);

    // dump the graph
    G.dump();
    
    //remove test
    cout << "\nRemove edges (3,4), (0,4), and (1,2)...\n";
    G.removeEdge(3,4);
    G.removeEdge(0,4);
    G.removeEdge(1,2);
 
    //dump
    G.dump();

    //assignment operator test
    cout << "\nGraph F(5) = G:\n";
    Graph F(5);
    F = G;
    F.dump();

    //copy constructor test
    cout << "\nGraph R(G):\n";
    Graph R(G);
    R.dump(); 
    
    Graph::NbIterator nit;

    for (int i = 0; i < G.numVert(); i++) {
        cout << "\nThe neighbors of vertex " << i << " are:\n";
        for (nit = G.nbBegin(i); nit != G.nbEnd(i) ; nit++) {
            cout << "(" << (*nit).first << ", " << (*nit).second << ")" << " ";
        }
        cout << endl;
    }

    // Use the edge iterator.  Iterate over all edges and print them.

    Graph::EgIterator eit;
    std::tuple<int,int,int> edge;

    cout << "\nnumVert = " << G.numVert() << ", numEdge = " << G.numEdge() << endl;
    cout << "The edges in the graph are:\n";
    for (eit = G.egBegin(); eit != G.egEnd(); eit++) {

        edge = *eit;   // get current edge

        // the two data members of a pair are first and second
        //
        cout << "(" << get<0>(edge) << ", "
        << get<1>(edge) << ", "
        << get<2>(edge) << ") ";

    }
    cout << endl ;

    // Remove some edges and then print all the edges again

    cout << "\nRemove edges (3,4), (0,4), and (1,2)...\n";
    G.removeEdge(3,4);
    G.removeEdge(0,4);
    G.removeEdge(1,2);

    cout << "\nnumVert = " << G.numVert() << ", numEdge = " << G.numEdge() << endl;
    cout << "The edges in the graph are:\n";
    for (eit = G.egBegin(); eit != G.egEnd(); eit++) {
        edge = *eit;   // get current edge

        // the two data members of a pair are first and second
        //
        cout << "(" << get<0>(edge) << ", "
        << get<1>(edge) << ", "
        << get<2>(edge) << ") ";

    }
    G.dump();
    cout << endl ;

    //remove from first row test
    G.removeEdge(0,1);
    G.removeEdge(0,3);
    
    cout << "\nnumVert = " << G.numVert() << ", numEdge = " << G.numEdge() << endl;
    cout << "The edges in the graph are:\n";
    for (eit = G.egBegin(); eit != G.egEnd(); eit++) {
        edge = *eit;   // get current edge

        // the two data members of a pair are first and second
        //
        cout << "(" << get<0>(edge) << ", "
        << get<1>(edge) << ", "
        << get<2>(edge) << ") ";

    }
    G.dump();
    cout << endl ;

    return 0;
}