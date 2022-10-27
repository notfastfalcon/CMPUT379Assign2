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
clock_t startTime;
//total work execution time till now
float totalExecTime;
//saving work done by each thread
vector<int> workPerThread;

//starts the timer for program execution
void startTimer() {
	startTime = clock();
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
	for(int i = 1; i <= nthreads; i++) {
		//initially work done by each thread is 0
		workPerThread.push_back(0);
	}
}


//prints the output of each operation and updates all global counts and times
void outputAndCalculation(int id, string event, unsigned int workCount, int n) {
	totalExecTime = float(clock() - startTime) / float(CLOCKS_PER_SEC);
	//printing upto 3 decimal places
	printf("%.3f", totalExecTime); 
	cout << " ID= " << id;

	if (event == "Ask" || event == "End") {
		cout << "\t " << event << "\n";
		if (event == "Ask"){
			ask++;
		}
		
	}
	else if (event == "Work" || event == "Receive") {	
		cout << " Q= " << workCount << " " << event;
		if (event == "Work") {
			cout << "\t\t" << n << "\n";
			work++;
		}
		else {
			cout << "\t" << n << "\n";
			receive++;
		}
	}
	else {
		cout << "\t " << event; 
		if (event == "Sleep") {
			cout << "\t\t" << n << "\n";
			sleepCount++;
		}
		else {
			cout << "\t" << n << "\n";
			complete++;
			//count how much work is done by each thread
			workPerThread[id]++;
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
	for (int i = 1; i <= nthreads; i++) {
		cout << "Thread " << i << "\t" << workPerThread[i] <<"\n"; 
	}
	if (totalExecTime > 0.0) {
		float transPerSec = (1/totalExecTime) * work;
		cout << "Transactions per second: ";
		printf("%.2f\n", transPerSec);
	}
	else {
		cout << "Transactions per second: N.A.\n";
	}

}