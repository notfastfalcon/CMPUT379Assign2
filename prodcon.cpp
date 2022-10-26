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

//tands.cpp functions
extern void Trans(int n);
extern void Sleep(int n);

unsigned int workCount = 0;
unsigned int queueLimit = 0;

void operations(int nthreads) {
	// work queue.. no size restrictions yet, implemented later in code
	queue<string> workQueue;

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
		try {
			if(cmdType == 0) {
				Trans(stoi(workCommand.substr(1)));
			}
			else if(cmdType == 1) {
				Sleep(stoi(workCommand.substr(1)));
			}
			else {
				cout << "Invalid Command\n";
			}
		}
		//cath all exceptions that could be raised by stoi
		catch(...) {
			cout << "Please enter as T<int> or S<int>\n";
		}
	}
}

//consumer thread functions
void* consumer(void* args_p) {
	int* threadID = (int*) args_p;
	pthread_mutex_lock(&queueMutex);
	while (workCount == 0) {
		outputAndCalculation(*threadID, "Ask", workCount, 0);
		pthread_cond_wait(&queueCond, &queueMutex);
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
	loggedToFile(outFile);
	operations(nthreads);
	summaryOutput(nthreads);
	exit(0);

	return 0;
}
