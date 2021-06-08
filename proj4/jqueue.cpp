// CMSC 341 - Spring 2020 - Project 4
// JQueue: a job queue using a skew heap and function pointers

#include "jqueue.h"
#include <vector>

using std::cout;
using std::endl;
using std::domain_error;

//Constructor: must be provided with a pointer to the prioritization function.
JQueue::JQueue(prifn_t priFn) {
    _size = 0;
    _heap = nullptr;
    priority = priFn;
}

//Destructor: all dynamically-allocated data must be deallocated.
JQueue::~JQueue() {
    clear();
}

//Copy constructor: must make a deep copy of the rhs object
JQueue::JQueue(const JQueue& rhs) {
    _heap = nullptr;
    _size = 0;
    priority = rhs.priority;
    if (rhs._heap != nullptr) {
        copyHelper(this->_heap, rhs._heap);
    }
}

void JQueue::copyHelper(Node* &lhs, const Node* rhs) {
    //base case
    if (rhs == nullptr) {
        return;
    }
    //recursion
    else {
        lhs = new Node(rhs->_job);
        _size++;
        copyHelper(lhs->_left, rhs->_left); 
        copyHelper(lhs->_right, rhs->_right);
    }
}

//Assignment operator: remember to check for self-assignment and-
//to free all dynamically allocated data members of the host
JQueue& JQueue::operator=(const JQueue& rhs) {
    //prevents self-assignment
    if (this != &rhs) { 
        clearHelper(_heap);
        _heap = nullptr;
        _size = 0;
        priority = rhs.priority;
        if (rhs._heap != nullptr) {
            copyHelper(this->_heap, rhs._heap);
        }
    }
    return *this;
}

//Insert a job into the job queue. must maintain the max-heap property.
void JQueue::insertJob(const job_t& input) {
    Node* node = new Node(input);
    _heap = mergeHelper(_heap, node);
    _size++;
}

Node* JQueue::mergeHelper(Node* p1, Node* p2) {
    if (p1 == nullptr) {
        return p2;
    }
    else if (p2 == nullptr) {
        return p1;
    }
    else {
        Node* temp;
        //assume that p1 has higher priority than p2; if not, swap, p1 and p2.
        if (priority(p1->_job) < priority(p2->_job)) {
            temp = p1;
            p1 = p2;
            p2 = temp;
        }
        //swap the left and right subtrees of p1
        temp = p1->_left;
        p1->_left = p1->_right;
        p1->_right = temp;
        //recursively merge p2 and the left subtree of p1
        p1->_left = mergeHelper(p2, p1->_left);
    }
    return p1;
}

//Merge the host queue with the rhs; leaves rhs empty.
//Two skew heaps can only be merged if they have the same priority function.
//If the user attempts to merge queues with different priority functions,
//a domain_error exception should be thrown.
void JQueue::mergeWithQueue(JQueue& rhs) {
    if (priority != rhs.priority) {
        throw domain_error("Cannot merge queues with different \
priority functions.\n");
    }
    //prevent merging with self
    if (_heap != rhs._heap) {
        _heap = mergeHelper(_heap, rhs._heap);
        _size += rhs._size;
        //leave rhs empty
        rhs._heap = nullptr;
        rhs._size = 0;
    }
}

//Extract and return the highest priority job from the job queue
//Must maintain the max-heap property. Should throw a domain_error-
//exception if the queue is empty when the function is called.
job_t JQueue::getNextJob() {
    if (numJobs() == 0) {
        throw domain_error("There are no jobs in the queue.\n");
    }
    Node* temp = _heap;
    job_t job = temp->_job; //extract job
    _heap = mergeHelper(_heap->_left, _heap->_right);
    delete temp;
    temp = nullptr;
    _size--;
    return job;
}

//Return the current number of jobs in the queue.
int JQueue::numJobs() const {
    return _size;
}

//Get the current priority function.
prifn_t JQueue::getPriorityFn() const {
    return priority;
}

//Set a new priority function. Must rebuild the heap!
void JQueue::setPriorityFn(prifn_t priFn) {
    priority = priFn; //change priority function
    int index = 0;
    int size = _size;
    _jobs = new job_t[size];
    //extract every job
    while (numJobs() > 0) {
        job_t job = getNextJob();
        _jobs[index] = job;
        index++;
    }
    //rebuild heap with new priority function
    for (int i = 0; i < size; i++) {
        insertJob(_jobs[i]);
    }
    delete [] _jobs;
}

//Print the contents of the queue using preorder traversal.
//Although the first job printed should have the highest priority,
//the remaining jobs will not necessarily be in priority order.
void JQueue::printJobQueue() const {
    if (numJobs() > 0) {
        cout << "Contents of the job queue:\n";
        printHelper(_heap);
    }
    else {
        cout << "There are no jobs in the queue.\n";
    }
}

void JQueue::printHelper(Node* node) const {
    if (node == nullptr) {
        return;
    }
    cout << "[" << priority(node->_job) << "] " << *node << endl;
    printHelper(node->_left);
    printHelper(node->_right);
}

//Use an inorder traversal to dump the heap and indicate the tree-
//structure (as in the previous project). Represent a node in the-
//tree by its integer priority as computed using the priority function.
void JQueue::dump() const {
    if (numJobs() > 0) {
        dumpHelper(_heap);
    }
    else {
        cout << "There are no jobs in the queue.\n";
    }
}

void JQueue::dumpHelper(Node* node) const {
    if (node == nullptr) {
        return;
    }
    cout << "(";
    dumpHelper(node->_left);
    cout << priority(node->_job);
    dumpHelper(node->_right);
    cout << ")";
}

//Clear the job queue. Heletes all the nodes in the skew heap,
//leaving the skew heap empty.
void JQueue::clear() {
    clearHelper(_heap);
}

void JQueue::clearHelper(Node* node) {
    //clear memory associated with children
    if (node == nullptr) {
        return;
    }
    clearHelper(node->_left);
    clearHelper(node->_right);
    delete node;
    node = nullptr;
}

ostream& operator<<(ostream& sout, const job_t& job) {
  sout << "Job: " << job._name << ", pri: " << job._priority
       << ", u: " << job._user << ", g: " << job._group << ", proc: "
       << job._proc << ", mem: " << job._mem << ", time: " << job._time;
  return sout;
}

ostream& operator<<(ostream& sout, const Node& node) {
  sout << node._job;
  return sout;
}
