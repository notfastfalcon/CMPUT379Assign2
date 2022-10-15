#include "iostream"
#include "string.h"
#include "stdio.h"
#include "stdlib.h"
#include "fcntl.h"
#include "unistd.h"
#include "header.h"
using namespace std;

void loggedToFile(string filename) {
	int outfd;
	// 0666 was included to give rw permission
	if ((outfd = open(filename.c_str(), O_CREAT | O_WRONLY, 0666)) < 0) {
		perror("Open outfd failed!");
	}
	dup2(outfd, STDOUT_FILENO);
	close(outfd);
}