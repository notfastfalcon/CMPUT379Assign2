#include "iostream"
#include "string.h"
#include "stdio.h"
#include "stdlib.h"
#include "fcntl.h"
#include "unistd.h"
#include "vector"
#include "header.h"
using namespace std;

vector<vector<string>> processTable;

void* consume(void* args_p) {
	int* threadID = (int*) args_p; 
    return NULL;
}

void addToProcessTable(vector<string> workEntry) {
	processTable.push_back(workEntry);
}

void loggedToFile(string filename) {
	int outfd;
	// 0666 was included to give rw permission
	if ((outfd = open(filename.c_str(), O_CREAT | O_WRONLY, 0666)) < 0) {
		perror("Open outfd failed!");
	}
	dup2(outfd, STDOUT_FILENO);
	close(outfd);
}

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

void operationOutput() {
	//TODO: print these values as we go with the operations.
	//Add output to vector and send a vector for each op to addToProcessTable().
}

void summaryOutput(int nthreads) {
	int work = 0, ask = 0, receive = 0, complete = 0, sleep = 0;
	//TODO: Calculation for all the above
	cout << "Summary:\n";
	cout << "Work\t"<< work << "\n";
	cout << "Ask\t"<< ask << "\n";
	cout << "Receive\t"<< receive << "\n";
	cout << "Complete\t"<< complete << "\n";
	cout << "Sleep\t"<< sleep << "\n";
	for (int i = 0; i < nthreads; i++) {
		int threadWork = 0;
		//TODO: calculation for threadWork
		cout << "Thread  " << i << "     " << threadWork <<"\n"; 
	}

	//TODO: transPerSec = (1/final time)*TotalWorkDone
	float transPerSec = 0;
	cout << "Transactions per second: " << transPerSec << "\n";

}