#include "iostream"
#include "string.h"
#include "queue"
#include "unistd.h"
#include "pthread.h"
#include "bits/stdc++.h"
#include "header.h"
using namespace std;

//MUTEX Variables
pthread_cond_t queueEmptyCond = PTHREAD_COND_INITIALIZER;
pthread_cond_t queueFullCond = PTHREAD_COND_INITIALIZER;
pthread_mutex_t queueEmptyMutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t queueFullMutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t queueMutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t outputMutex = PTHREAD_MUTEX_INITIALIZER;

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
	for (int id = 0; id < nthreads; id++) {
		threadID[id] = id + 1;
		//passing id as *args_p to assign id to threads
		pthread_create(&threads[id], NULL, consumer, &threadID[id]);
	}
	// init vector to store work done by each thread
	initWorkPerThread(nthreads);

	string workCommand;
	int cmdType;
	while(getline(cin, workCommand)) {
		cmdType = getCommandType(workCommand);
		if(cmdType == 0) {
			try {
				pthread_mutex_lock(&queueEmptyMutex);
				while (workCount == queueLimit) {
					//wait for consumer to consume
					pthread_cond_wait(&queueEmptyCond, &queueEmptyMutex);
				}
				pthread_mutex_unlock(&queueEmptyMutex);
				pthread_mutex_lock(&queueMutex);
				//add work to queue to consume
				int n = stoi(workCommand.substr(1));
				workQueue.push(n);
				workCount++;
				pthread_mutex_unlock(&queueMutex);
				//signal atleast one consumer that work has been added
				pthread_cond_signal(&queueFullCond);
				pthread_mutex_lock(&outputMutex);
				outputAndCalculation(0, "Work", workCount, n);
				pthread_mutex_unlock(&outputMutex);	
			}
			catch(...) {
				cout << "Invalid arguments\n";
			}
		}
		else if(cmdType == 1) {
			try {
				pthread_mutex_lock(&queueMutex);
				int n = stoi(workCommand.substr(1));
				Sleep(n);
				pthread_mutex_unlock(&queueMutex);
				pthread_mutex_lock(&outputMutex);
				outputAndCalculation(0, "Sleep", workCount, n);
				pthread_mutex_unlock(&outputMutex);
			}
			catch(...) {
				cout << "Invalid arguments\n";
			}
		}
		else {
			cout << "Invalid commands\n";
		}
	}
	pthread_mutex_lock(&outputMutex);
	outputAndCalculation(0, "End", workCount, -1);
	pthread_mutex_unlock(&outputMutex);

	for (int i = 0; i < nthreads; i++) {
		pthread_join(threads[i], NULL);
	}
	
}

//consumer thread functions
void* consumer(void* args_p) {
	int* threadID = (int*) args_p;
	pthread_mutex_lock(&queueFullMutex);
	while(workCount == 0) {
		//log ask using n as -1 as 'n' does not exist for ask
		pthread_mutex_lock(&outputMutex);
		outputAndCalculation(*threadID, "Ask", workCount, -1);
		pthread_mutex_unlock(&outputMutex);
		//wait for the producer to add work
		pthread_cond_wait(&queueFullCond, &queueFullMutex);
	}
	pthread_mutex_unlock(&queueFullMutex);
	pthread_mutex_lock(&queueMutex);
	//get int value from queue (FIFO) and removing from active work count
	int n = workQueue.front();
	workQueue.pop();
	workCount--;
	pthread_mutex_unlock(&queueMutex);
	//signal producer that space was generated
	pthread_cond_signal(&queueEmptyCond);

	//command received
	pthread_mutex_lock(&outputMutex);
	outputAndCalculation(*threadID, "Receive", workCount, n);
	pthread_mutex_unlock(&outputMutex);
	//execute command
	Trans(n);
	//command completed
	pthread_mutex_lock(&outputMutex);
	outputAndCalculation(*threadID, "Complete", workCount, n);
	pthread_mutex_unlock(&outputMutex);
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
	pthread_mutex_lock(&outputMutex);
	summaryOutput(nthreads);
	pthread_mutex_unlock(&outputMutex);
	exit(0);
	return 0;
}
