#ifndef _HANDLER_H_
#define _HANDLER_H_

#include "string"
#include "vector"

// Utility Functions
void* consume(void*);
void addToProcessTable(std::vector<std::string>);
void loggedToFile(std::string);
int getCommandType(std::string);
void operationOutput(void);
void summaryOutput(int);

// Prodcon Functions
void operations(int);


#endif