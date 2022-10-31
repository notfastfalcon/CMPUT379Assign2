#include "iostream"
#include "string.h"
#include "queue"
#include "unistd.h"
#include "pthread.h"
#include "semaphore.h"
#include "bits/stdc++.h"
#include "header.h"
using namespace std;

//MUTEX and Semaphore Variables
pthread_mutex_t queueMutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t outputMutex = PTHREAD_MUTEX_INITIALIZER;
sem_t queueEmpty;
sem_t queueFull;

//tands.cpp functions
extern void Trans(int n);
extern void Sleep(int n);

// work queue.. no size restrictions yet, implemented later in code
queue<int> workQueue;
//boolean variable to track progress
bool done = false;

void producer() {
	string workCommand;
	int cmdType;
	while(getline(cin, workCommand)) {
		//input is guaranteed to be correctly formatted, so no try catch block implemented
		cmdType = getCommandType(workCommand);
		// If command is 'T'
		if(cmdType == 0) {
			int n = stoi(workCommand.substr(1));
			//wait until there is space in queue
			sem_wait(&queueEmpty);
			pthread_mutex_lock(&queueMutex);
			//add work to queue to consume
			workQueue.push(n);
			int workCount = workQueue.size();
			pthread_mutex_unlock(&queueMutex);
			pthread_mutex_lock(&outputMutex);	
			outputAndCalculation(0, "Work", workCount, n);
			pthread_mutex_unlock(&outputMutex);	
			//post for atleast one consumer to take work
			sem_post(&queueFull);
		}
		//If command is 'S'
		else if(cmdType == 1) {
			int n = stoi(workCommand.substr(1));
			pthread_mutex_lock(&outputMutex);
			outputAndCalculation(0, "Sleep", -1, n);
			pthread_mutex_unlock(&outputMutex);
			Sleep(n);
		}
	}
	//after input is read, producer's work is done
	done = true;
	pthread_mutex_lock(&outputMutex);
	outputAndCalculation(0, "End", -1, -1);
	pthread_mutex_unlock(&outputMutex);
}

//consumer thread functions
void* consumer(void* args_p) {
	bool hasWork = false;
	int n = -1, workCount = -1;
	int* threadID = (int*) args_p;
	while(true) {
		//ask for work
		pthread_mutex_lock(&outputMutex);
		outputAndCalculation(*threadID, "Ask", -1, -1);
		pthread_mutex_unlock(&outputMutex);
		
		//if producer is done and queue is empty, consumers can terminate
		if(done && workQueue.size() == 0) {
			//to wake any sleeping consumers
			sem_post(&queueFull);
			break;
		}
		//wait until queue has work
		sem_wait(&queueFull);
		pthread_mutex_lock(&queueMutex);
		//if queue has work then pop work and update variable
		if (workQueue.size() > 0) {	
			//get int value from queue (FIFO) and removing from active work count
			n = workQueue.front();
			workQueue.pop();
			workCount = workQueue.size();
			hasWork = true;
		}
		pthread_mutex_unlock(&queueMutex);
		//if there was work received by consumer then do following
		if(hasWork) {
			//post for producer to know that space was generated
			sem_post(&queueEmpty);
			//command was received
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
		//update flag
		hasWork = false;
	}
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
		exit(0);
	}

	// input handling
	nthreads = atoi(argv[1]);
	
	if (argc == 3) {
		tid = atoi(argv[2]);
	}

	if (tid != 0) {
		outFile = "prodcon." + to_string(tid) + ".log";
	}

	int queueLimit = 2 * nthreads;

	//Mutex and sem init
	pthread_mutex_init(&queueMutex, NULL);
	pthread_mutex_init(&outputMutex, NULL);
	sem_init(&queueEmpty, 0, queueLimit);
	sem_init(&queueFull, 0, 0);

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
	producer();

	//wait for threads
	for (int i = 0; i < nthreads; i++) {
		pthread_join(threads[i], NULL);
	}

	//print the final summary
	pthread_mutex_lock(&outputMutex);
	summaryOutput(nthreads);
	pthread_mutex_unlock(&outputMutex);

	//Mutex and sem Destroy
	pthread_mutex_destroy(&queueMutex);
	pthread_mutex_destroy(&outputMutex);;
	sem_destroy(&queueEmpty);
	sem_destroy(&queueFull);
	return 0;
}
