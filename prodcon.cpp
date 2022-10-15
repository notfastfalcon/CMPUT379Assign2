#include "iostream"
#include "string.h"
#include "pthread.h"
#include "bits/stdc++.h"
#include "header.h"
using namespace std;

void input(int argc, char *argv[]) {
	
}


int main (int argc, char *argv[]) {
	int nthreads = 0, tid = 0;
	string outFile = "prodcon.log";

	nthreads = atoi(argv[1]);
	if (argc == 3) {
		tid = atoi(argv[2]);
	}

	if (tid != 0) {
		outFile = "prodcon." + to_string(tid) + ".log";
	}

	cout<< outFile << "\n";
	return 0;
}