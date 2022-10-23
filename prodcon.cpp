#include "iostream"
#include "string.h"
#include "queue"
#include "pthread.h"
#include "bits/stdc++.h"
#include "header.h"
#include "tands.cpp"
using namespace std;

extern void Trans(int n);
extern void Sleep(int n);

void operations(int nthreads, string outFile) {
	// work queue.. no size restrictions yet, implemented later in code
	queue<string> workQueue;


	//creating threads
	pthread_t threads[nthreads];
	for (int id = 0; id < nthreads; id++) {
		//passing id as *arg to assign id to threads
		pthread_create(&threads[nthreads], NULL, consume, &id);
	}


	string workCommand;
	int cmdType = -1;

	while(getline(cin, workCommand)) {
		cmdType = getCommandType(workCommand);
		switch (cmdType) {
			case 0:
				Trans(stoi(workCommand.substr(1)));
			case 1:
				Sleep(stoi(workCommand.substr(1)));
			case -1:
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
	operations(nthreads, outFile);

	return 0;
}