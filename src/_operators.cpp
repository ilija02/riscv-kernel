#include "../h/syscall_c.h"
#include "../h/syscall_cpp.hpp"


// there's no need to define a corresponding header file for this cpp file, as the new and delete are a part of the
// language and are already included. decltype(sizeof(0)) is used to get the appropriate data type for new
//operators. This file specifies the global new and delete operators for array types. Regular new and delete are declared in
// syscall_cpp.cpp

using size_t = decltype(sizeof(0));

void* operator new[](size_t size)
{
	return mem_alloc(size);
}



void operator delete[](void* chunk)

noexcept
{
	mem_free(chunk);
}
