#include "iostream"
#include "string.h"
#include "stdio.h"
#include "stdlib.h"
#include "fcntl.h"
#include "unistd.h"
#include "header.h"
using namespace std;

void* consume(void* args_p) {
	int* threadID = (int*) args_p; 
	cout << "Hello from threadID" << "\t" << threadID << "\n";
    return NULL;
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