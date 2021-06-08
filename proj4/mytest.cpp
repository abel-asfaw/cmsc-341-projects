
// CMSC 341 - Spring 2020 - Project 4
// driver.cpp: sample driver for the JQueue class.

#include "jqueue.h"
#include <iostream>
using namespace std;

//
// Priority functions compute an integer priority for a job.  Internal
// computations may be floating point, but must return an integer.
//
// Function definitions are below main()
//

int priorityFn1(const job_t &job);
int priorityFn2(const job_t &job);

int main() {

    // Create a job queue with capacity five and priority function
    // priorityFn1()
    cout << "**********************************************************\n";
    cout << "\ninsert and print test:\n";
    JQueue jobQueue(priorityFn1);

    // Create some test jobs and insert them into the queue
    
    job_t testJob1("test1", 50, 3, 11,  16, 1024, 120);
    job_t testJob2("test2", 50, 7,  5, 128,  512, 300);
    job_t testJob3("test3", 20, 3,  0, 128, 1024, 1800);
    job_t testJob4("test1a", 50, 3, 11,  16, 1024, 120);

    jobQueue.insertJob(testJob3);
    jobQueue.insertJob(testJob1);
    jobQueue.insertJob(testJob2);
    jobQueue.insertJob(testJob4);

    // Print the queue, change priority function, print again, change
    // again, print again.  Changing the priority function should cause
    // the heap to be rebuilt.

    cout << endl;
    jobQueue.printJobQueue();
    cout << "Dump the queue:\n";
    jobQueue.dump();
    cout << endl;

    cout << "\n**********************************************************\n";
    cout << "\ncopy constructor test (jobQueueCopy(jobQueue)) - jobQueueCopy.dump():\n";
    JQueue jobQueueCopy(jobQueue);
    cout << endl;
    jobQueueCopy.printJobQueue();
    cout << "Dump the queue:\n";
    jobQueueCopy.dump();
    cout << endl;

    cout << "\n**********************************************************\n";
    cout << "\nassignment operator test (jobQueueAssign = jobQueue) - \
jobQueueAssign.dump():\n";
    JQueue jobQueueAssign = jobQueue;
    cout << endl;
    jobQueueAssign.printJobQueue();
    cout << "Dump the queue:\n";
    jobQueueAssign.dump();
    cout << endl;
    
    cout << "\n**********************************************************\n";
    cout << "\ndeep copy test - change priority function of jobQueue and check \
whether jobQueueAssign and jobQueueCopy change\n";
    jobQueue.setPriorityFn(priorityFn2);
 
    cout << "\njobQueue dump:\n";
    cout << endl;
    jobQueue.printJobQueue();
    cout << "Dump the queue:\n";
    jobQueue.dump();
    cout << endl;
    
    cout << "\njobQueueCopy dump:\n";
    cout << endl;
    jobQueueCopy.printJobQueue();
    cout << "Dump the queue:\n";
    jobQueueCopy.dump();
    cout << endl;

    cout << "\njobQueueAssign dump:\n";
    cout << endl;
    jobQueueAssign.printJobQueue();
    cout << "Dump the queue:\n";
    jobQueueAssign.dump();
    cout << endl;

    cout << "\n**********************************************************\n";
    cout << "\nassign non-empty queue to another non-empty queue \
(jobQueueAssign = jobQueue) test\n";
    jobQueueAssign = jobQueue;
    cout << "\njobQueueAssign dump:\n";
    cout << endl;
    jobQueueAssign.printJobQueue();
    cout << "Dump the queue:\n";
    jobQueueAssign.dump();
    cout << endl;

    cout << "\njobQueue dump:\n";
    cout << endl;
    jobQueue.printJobQueue();
    cout << "Dump the queue:\n";
    jobQueue.dump();
    cout << endl;

    cout << "\n**********************************************************\n";
    cout << "\nassign empty queue to non-empty queue \
(jobQueueAssign = emptyJobQueue):\n";
    JQueue emptyJobQueue(priorityFn1);
    jobQueueAssign = emptyJobQueue;
    cout << "\njobQueueAssign dump:\n";
    cout << endl;
    jobQueueAssign.printJobQueue();
    cout << "Dump the queue:\n";
    jobQueueAssign.dump();
    cout << endl;

    cout << "\n**********************************************************\n";
    cout << "\nself-assignment test - should do nothing (jobQueue = jobQueue):\n";
    jobQueue = jobQueue;
    cout << "\njobQueue dump:\n";
    cout << endl;
    jobQueue.printJobQueue();
    cout << "Dump the queue:\n";
    jobQueue.dump();
    cout << endl;

    cout << "\n**********************************************************\n";
    cout << "\nsetPriorityFn() test:\n";
    jobQueue.setPriorityFn(priorityFn1);
    cout << endl;
    jobQueue.printJobQueue();
    cout << "Dump the queue:\n";
    jobQueue.dump();
    cout << endl;


    cout << "\n**********************************************************\n";
    cout << "\nmergeWithQueue() test - merge newQueue with jobQueue:\n";
    JQueue newQueue(priorityFn1);
    newQueue.mergeWithQueue(jobQueue);
    cout << endl;
    newQueue.printJobQueue();
    cout << "Dump the queue:\n";
    newQueue.dump();
    // cout << endl;

    cout << "\n**********************************************************\n";
    // Remove and print jobs in priority order
    cout << "\ngetNextJob() test:\n";
    while (newQueue.numJobs() > 0) {
        job_t job = newQueue.getNextJob();
        cout << "[" << priorityFn1(job) << "] " << job << endl;
    }
    
    cout << "\n**********************************************************\n";
    cout << "\nstress test - insert 1,000 jobs\n";
    JQueue stressQueue(priorityFn1);
    int n = 1000;
    srand (time(NULL));
    for (int i = 0; i < n; i++) {
        job_t job(std::to_string(i+1), rand()%100+1, rand()%100+1, rand()%100+1, \
		  rand()%255+1, rand()%65535+1, rand()%172800+1);
        stressQueue.insertJob(job);
    }
    cout << endl;
    stressQueue.printJobQueue();
    cout << "Dump the queue:\n";
    stressQueue.dump();
    cout << endl;
    return 0;
}

int priorityFn1(const job_t &job) {
    float pri;

    // 100 (regular user) + 100 (regular group) + job priority
    pri = 200 + job._priority;

    // If special user (7 or 23) add 200 
    if (job._user == 7 || job._user == 23) pri += 200;

    // If special group (0 or 11) add 100
    if (job._group == 0 || job._group == 11) pri += 100;

    // Subtract weighted proc, mem, and time
    //   1.0 * proc, 0.01 * mem, (1/3600) * time
    pri = pri - job._proc - 0.01*job._mem - (1.0/3600.0)*job._time;

    return int(pri);
}

int priorityFn2(const job_t&job) {
    return int(job._time);
}
