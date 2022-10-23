#include "iostream"
#include "string.h"
#include "queue"
#include "pthread.h"
#include "bits/stdc++.h"
#include "header.h"
using namespace std;

extern void Trans(int n);
extern void Sleep(int n);

unsigned int workCount = 0;

void operations(int nthreads) {
	// work queue.. no size restrictions yet, implemented later in code
	queue<string> workQueue;

	//creating threads
	pthread_t threads[nthreads];

	//the use of an array to store the ids was done to avoid losing 'id', as it goes out of scope and all
	//threads are assigned an id = nthreads. Inspired from StackOverFlow Code. 
	//Link:https://stackoverflow.com/questions/68385607/how-to-create-multiple-pthreads-with-a-for-loop
	unsigned int threadID[nthreads];
	for (int id = 0; id < nthreads; id++) {
		threadID[id] = id;
		//passing id as *arg to assign id to threads
		pthread_create(&threads[nthreads], NULL, consume, &threadID[id]);
	}


	string workCommand;
	int cmdType;

	while(getline(cin, workCommand)) {
		cmdType = getCommandType(workCommand);
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
}

int main (int argc, char *argv[]) {
	//default values
	int nthreads = 0, tid = 0;
	string outFile = "prodcon.log";

	//raising error if arguments are incorrect
	if(argc < 2 || argc > 3) {
		cout << "Enter valid arguments!\n";
		exit(0);
	}

	nthreads = atoi(argv[1]);
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
