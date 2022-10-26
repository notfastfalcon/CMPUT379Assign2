#include "iostream"
#include "string.h"
#include "queue"
#include "pthread.h"
#include "bits/stdc++.h"
#include "header.h"
using namespace std;

//MUTEX Variables
pthread_cond_t queueCond = PTHREAD_COND_INITIALIZER;
pthread_mutex_t queueMutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t queueCountMutex = PTHREAD_MUTEX_INITIALIZER;

//tands.cpp functions
extern void Trans(int n);
extern void Sleep(int n);

unsigned int workCount = 0;
unsigned int queueLimit = 0;
// work queue.. no size restrictions yet, implemented later in code
queue<int> workQueue;

void operations(int nthreads) {
	//creating threads
	pthread_t threads[nthreads];

	//the use of an array to store the ids was done to avoid losing 'id', as it goes out of scope and all
	//threads are assigned an id = nthreads. Inspired from StackOverFlow Code. 
	//Link:https://stackoverflow.com/questions/68385607/how-to-create-multiple-pthreads-with-a-for-loop
	unsigned int threadID[nthreads];
	for (int id = 1; id <= nthreads; id++) {
		threadID[id] = id;
		//passing id as *args_p to assign id to threads
		pthread_create(&threads[nthreads], NULL, consumer, &threadID[id]);
	}

	// init vector to store work done by each thread
	initWorkPerThread(nthreads);

	string workCommand;
	int cmdType;
	while(getline(cin, workCommand)) {
		cmdType = getCommandType(workCommand);
		if(cmdType == 0) {
			int n = 0;
			try {
				n = stoi(workCommand.substr(1));
			}
			catch(...) {
				cout << "Invalid arguments\n";
			}

			//adding work to queue
			pthread_mutex_lock(&queueMutex);
			if (workCount > queueLimit) {
				//log ask using n as -1 as 'n' does not exist for ask
				outputAndCalculation(*threadID, "Ask", workCount, n);
				//wait for consumer to consume
				pthread_cond_wait(&queueCond, &queueMutex);
			}
			else {
				//add work to queue to consume
				pthread_mutex_lock(&queueCountMutex);
				workQueue.push(n);
				workCount++;
				pthread_mutex_unlock(&queueCountMutex);
				//signal consumer that work has been added
				pthread_cond_signal(&queueCond);
				outputAndCalculation(0, "Work", workCount, n);
			}
			pthread_mutex_unlock(&queueMutex);
		}
		else if(cmdType == 1) {
			try {
				Sleep(stoi(workCommand.substr(1)));
			}
			catch(...) {
				cout << "Invalid arguments\n";
			}
		}
		else {
			cout << "Invalid commands\n";
		}
	}
	pthread_mutex_lock(&queueMutex);
	outputAndCalculation(0, "End", workCount, -1);
	pthread_mutex_unlock(&queueMutex);
}

//consumer thread functions
void* consumer(void* args_p) {
	int* threadID = (int*) args_p;
	int n = 0;
	pthread_mutex_lock(&queueMutex);
	if (workCount == 0) {
		//log ask using n as -1 as 'n' does not exist for ask
		outputAndCalculation(*threadID, "Ask", workCount, -1);
		//wait for the producer to add work
		pthread_cond_wait(&queueCond, &queueMutex);
	}
	else {
		//get int value from queue (FIFO) and removing from active work count
		pthread_mutex_lock(&queueCountMutex);
		n = workQueue.front();
		workQueue.pop();
		workCount--;
		pthread_mutex_unlock(&queueCountMutex);
		//signal that element from queue was removed
		pthread_cond_signal(&queueCond);
		//command received
		outputAndCalculation(*threadID, "Receive", workCount, n);
		//execute command
		Trans(n);
		//command completed
		outputAndCalculation(*threadID, "Complete", workCount, n);
	}
	pthread_mutex_unlock(&queueMutex);
    return NULL;
}


int main (int argc, char *argv[]) {
	//start timer for the program
	startTimer();

	//default values
	int nthreads = 0, tid = 0;
	string outFile = "prodcon.log";

	//raising error if arguments are incorrect
	if(argc < 2 || argc > 3) {
		cout << "Enter valid arguments!\n";
		exit(0);
	}

	nthreads = atoi(argv[1]);

	//work queue should be able to hold only 2 * nthreads
	queueLimit = 2 * nthreads;
	
	if (argc == 3) {
		tid = atoi(argv[2]);
	}

	if (tid != 0) {
		outFile = "prodcon." + to_string(tid) + ".log";
	}
	//log every STDOUT to file
	loggedToFile(outFile);
	//threads operations
	operations(nthreads);
	//print the summary
	summaryOutput(nthreads);
	exit(0);
	return 0;
}
