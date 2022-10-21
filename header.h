#ifndef _HANDLER_H_
#define _HANDLER_H_

#include "string"

// Producer Functions
void* produce(void*);

// Consumer Functions
void* consume(void*);

// Utility Functions
void loggedToFile(std::string);

// Prodcon Functions
void operations(int, std::string);

#endif