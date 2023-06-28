#ifndef PRINTING_HPP
#define PRINTING_HPP

#include "../h/syscall_c.h"
#include "../lib/console.h"

typedef unsigned long uint64;


void print_string(char const* string);

char* get_string(char* buf, int max);

int string_to_int(const char* s);

void print_int(int xx, int base = 10, int sgn = 0);

#endif // PRINTING_HPP

