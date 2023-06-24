#include "../h/syscall_c.h"

// there's no need to define a corresponding header file for this cpp file, as the new and delete are a part of the
// language and are already included. decltype(sizeof(0)) is used to get the appropriate data type for new
//operators

using param_type = decltype(sizeof(0));

void *operator new(param_type size) {
    return mem_alloc(size);
}

void *operator new[](param_type size) {
    return mem_alloc(size);
}


void operator delete(void *chunk)

noexcept {
mem_free(chunk);
}

void operator delete[](void *chunk)

noexcept {
mem_free(chunk);
}