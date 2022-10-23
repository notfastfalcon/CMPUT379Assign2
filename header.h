#ifndef _HANDLER_H_
#define _HANDLER_H_

#include "string"

// Utility Functions
void* consume(void*);
void loggedToFile(std::string);
int getCommandType(std::string);

// Prodcon Functions
void operations(int, std::string);

#endif