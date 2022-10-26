#ifndef _HANDLER_H_
#define _HANDLER_H_

#include "string"
#include "vector"

// Utility Functions
void startTimer(void);
void loggedToFile(std::string);
int getCommandType(std::string);
void initWorkPerThread(int);
void outputAndCalculation(int, std::string, unsigned int, int);
void summaryOutput(int);

// Prodcon Functions
void operations(int);
void* consumer(void*);


#endif