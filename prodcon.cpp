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
pthread_mutex_t queueMutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t outputMutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t queueEmptyMutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t queueFullMutex = PTHREAD_MUTEX_INITIALIZER;

//tands.cpp functions
extern void Trans(int n);
extern void Sleep(int n);

// work queue.. no size restrictions yet, implemented later in code
queue<int> workQueue;
//boolean variable to track progress
bool done = false;
//variables to terminate while loop in consumer
int workAdded = 0, workReceived = 0;

void producer(int nthreads) {
	string workCommand;
	int cmdType;
	int queueLimit = 2 * nthreads;
	while(getline(cin, workCommand)) {
		//input is guaranteed to be correctly formatted, so no try catch block implemented
		cmdType = getCommandType(workCommand);
		if(cmdType == 0) {
			if(workQueue.size() > queueLimit) {
				//wait for consumer to consume (i.e. Empty queue out)
				pthread_mutex_lock(&queueFullMutex);
				pthread_cond_wait(&queueEmptyCond, &queueFullMutex);
				pthread_mutex_unlock(&queueFullMutex);
			}
			else {
				pthread_mutex_lock(&queueMutex);
				//add work to queue to consume
				int n = stoi(workCommand.substr(1));
				workQueue.push(n);
				int workCount = workQueue.size();
				pthread_mutex_unlock(&queueMutex);
				workAdded++;
				//signal atleast one consumer that work has been added
				pthread_cond_signal(&queueFullCond);
				pthread_mutex_lock(&outputMutex);
				outputAndCalculation(0, "Work", workCount, n);
				pthread_mutex_unlock(&outputMutex);	
			}
		}
		else if(cmdType == 1) {
			int n = stoi(workCommand.substr(1));
			pthread_mutex_lock(&outputMutex);
			outputAndCalculation(0, "Sleep", -1, n);
			pthread_mutex_unlock(&outputMutex);
			Sleep(n);
		}
		else {
		}
	}
	pthread_mutex_lock(&outputMutex);
	outputAndCalculation(0, "End", -1, -1);
	pthread_mutex_unlock(&outputMutex);
}

//consumer thread functions
void* consumer(void* args_p) {
	int* threadID = (int*) args_p;
	while(!done || workAdded != workReceived) {
		if (workQueue.size() == 0) {
			pthread_mutex_lock(&outputMutex);
			outputAndCalculation(*threadID, "Ask", -1, -1);
			pthread_mutex_unlock(&outputMutex);
			pthread_mutex_lock(&queueEmptyMutex);
			pthread_cond_wait(&queueFullCond, &queueEmptyMutex);
			pthread_mutex_unlock(&queueEmptyMutex);
		}
		else {
			workReceived++;
			pthread_mutex_lock(&queueMutex);
			//get int value from queue (FIFO) and removing from active work count
			int n = workQueue.front();
			workQueue.pop();
			int workCount = workQueue.size();
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
			outputAndCalculation(*threadID, "Complete", -1, n);
			pthread_mutex_unlock(&outputMutex);
		}
	}
    return NULL;
}


int main (int argc, char *argv[]) {
	//start timer for the program
	startTimer();

	//Mutex init
	pthread_mutex_init(&queueMutex, NULL);
	pthread_mutex_init(&outputMutex, NULL);
	pthread_mutex_init(&queueEmptyMutex, NULL);
	pthread_mutex_init(&queueFullMutex, NULL);

	//default values
	int nthreads = 0, tid = 0;
	string outFile = "prodcon.log";

	//raising error if arguments are incorrect
	if(argc < 2 || argc > 3) {
		exit(0);
	}

	nthreads = atoi(argv[1]);
	
	if (argc == 3) {
		tid = atoi(argv[2]);
	}

	if (tid != 0) {
		outFile = "prodcon." + to_string(tid) + ".log";
	}

	//log every STDOUT to file
	loggedToFile(outFile);

	//if no threads required -> exit
	if(nthreads == 0) {
		exit(0);
	}

	// init vector to store work done by each thread
	initWorkPerThread(nthreads);

	//creating threads
	pthread_t threads[nthreads];
	//the use of an array to store the ids was done to avoid losing 'id', as it goes out of scope and all
	//threads are assigned an id = nthreads. Inspired from StackOverFlow Code. 
	unsigned int threadID[nthreads];
	for (int id = 0; id < nthreads; id++) {
		threadID[id] = id + 1;
		//passing id as *args_p to assign id to threads
		pthread_create(&threads[id], NULL, consumer, &threadID[id]);
	}

	//producer operations
	producer(nthreads);

	done = true;


	//wait for threads
	for (int i = 0; i < nthreads; i++) {
		pthread_join(threads[i], NULL);
	}

	//print the final summary
	pthread_mutex_lock(&outputMutex);
	summaryOutput(nthreads);
	pthread_mutex_unlock(&outputMutex);

	//MUTEX Destroy
	pthread_mutex_destroy(&queueMutex);
	pthread_mutex_destroy(&outputMutex);
	pthread_mutex_destroy(&queueEmptyMutex);
	pthread_mutex_destroy(&queueFullMutex);
	
	exit(0);
	return 0;
}
