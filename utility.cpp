#include "iostream"
#include "string.h"
#include "stdio.h"
#include "stdlib.h"
#include "fcntl.h"
#include "unistd.h"
#include "vector"
#include "cmath"
#include "header.h"
using namespace std;

//counters for summary
unsigned int work = 0, ask = 0, receive = 0, complete = 0, sleepCount = 0;
//total exec time
float timeCount = 0.000;
//saving work done by each thread
vector<int> workPerThread;

//thread function
void* consume(void* args_p) {
	int* threadID = (int*) args_p; 
    return NULL;
}

//redirects output to log file
void loggedToFile(string filename) {
	int outfd;
	// 0666 was included to give rw permission
	if ((outfd = open(filename.c_str(), O_CREAT | O_WRONLY, 0666)) < 0) {
		perror("Open outfd failed!");
	}
	dup2(outfd, STDOUT_FILENO);
	close(outfd);
}

//gets the command type entered
int getCommandType(string workCommand) {
	//default case handling
	int type = -1;
	if(workCommand[0] == 'T') {
		type = 0;
	}
	else if(workCommand[0] == 'S') {
		type = 1;
	}
	return type;
}

// initialize the vector after threads are created, before any work is assigned
void initWorkPerThread(int nthreads) {
	for(int i = 0; i < nthreads; i++) {
		//initially work done by each thread is 0
		workPerThread.push_back(0);
	}
}

//prints the output of each operation
void operationOutput() {
	//TODO: print these values as we go with the operations.
	//Update the global variables everytime.
	//add to workPerThread (workPerThread[id] ++);
}

//final summary output
void summaryOutput(int nthreads) {
	cout << "Summary:\n";
	cout << "Work\t"<< work << "\n";
	cout << "Ask\t"<< ask << "\n";
	cout << "Receive\t"<< receive << "\n";
	cout << "Complete\t"<< complete << "\n";
	cout << "Sleep\t"<< sleepCount << "\n";
	for (int i = 0; i < nthreads; i++) {
		cout << "Thread  " << i << "     " << workPerThread[i] <<"\n"; 
	}
	if (timeCount > 0.0) {
		float transPerSec = (1/timeCount) * work;
		cout << "Transactions per second: " << round(transPerSec*100.0)/100.0 << "\n";
	}
	else {
		cout << "Transactions per second: N.A.\n";
	}

}