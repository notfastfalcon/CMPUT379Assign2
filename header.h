#ifndef _HANDLER_H_
#define _HANDLER_H_

#include "string"
#include "vector"

// Utility Functions
void* consume(void*);
void loggedToFile(std::string);
int getCommandType(std::string);
void initWorkPerThread(int);
void operationOutput(void);
void summaryOutput(int);

// Prodcon Functions
void operations(int);


#endif