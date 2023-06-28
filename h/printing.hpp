#ifndef PRINTING_HPP
#define PRINTING_HPP

#include "../h/syscall_c.h"
#include "../lib/console.h"
typedef unsigned long uint64;

void printString(char const* string);

char* getString(char* buf, int max);

int stringToInt(const char* s);

void printInt(int xx, int base = 10, int sgn = 0);

#endif // PRINTING_HPP

