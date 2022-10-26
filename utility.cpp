#include "iostream"
#include "string.h"
#include "stdio.h"
#include "stdlib.h"
#include "bits/stdc++.h"
#include "unistd.h"
#include "fcntl.h"
#include "vector"
#include "cmath"
#include "iomanip"
#include "header.h"
using namespace std;

//counters for summary
unsigned int work = 0, ask = 0, receive = 0, complete = 0, sleepCount = 0;
//start value of time
clock_t start;
//total work execution time till now
float totalExecTime;
//saving work done by each thread
vector<int> workPerThread;

//starts the timer for program execution
void startTimer() {
	start = clock();
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


//prints the output of each operation and updates all global counts and times
void outputAndCalculation(int id, string event, unsigned int workCount, int n) {
	totalExecTime = double((clock() - start) / CLOCKS_PER_SEC);
	//printing upto 3 decimal places
	printf("%.3f", totalExecTime); 
	cout << "  ID= " << id;

	if (event == "Ask") {
		ask++;
		cout << "\t" << event << "\n";
	}
	else if (event == "Work" || event == "Receive") {	
		cout << "  Q= " << workCount << "  " << event << "\t" << n << "\n";
		if (event == "Work") {
			work++;
		}
		else {
			receive++;
		}
	}
	else {
		cout << "\t" << event << "\t" << n <<"\n"; 
		if (event == "Sleep") {
			sleepCount++;
		}
		else if (event == "Complete") {
			complete++;
			//count how much work is done by each thread
			workPerThread[id]++;
		}
		else{
			//this is the end event so we do nothing
		}
	}
}

//final summary output
void summaryOutput(int nthreads) {
	cout << "Summary:\n";
	cout << "Work\t\t"<< work << "\n";
	cout << "Ask\t\t"<< ask << "\n";
	cout << "Receive\t\t"<< receive << "\n";
	cout << "Complete\t"<< complete << "\n";
	cout << "Sleep\t\t"<< sleepCount << "\n";
	for (int i = 0; i < nthreads; i++) {
		cout << "Thread " << i+1 << "\t" << workPerThread[i] <<"\n"; 
	}
	if (totalExecTime > 0.0) {
		float transPerSec = (1/totalExecTime) * work;
		cout << "Transactions per second: " << round(transPerSec*100.0)/100.0 << "\n";
	}
	else {
		cout << "Transactions per second: N.A.\n";
	}

}