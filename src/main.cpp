#include "../h/printing.hpp"
#include "../h/riscv.hpp"
#include "../h/syscall_c.h"
#include "../h/MemoryAllocator.hpp"

extern "C" void trapHandler();

void main() {

    uint64 volatile handlerAddress = (uint64) &trapHandler | 0x01; //set the base address for interrupts to trap handler and
    //set mode to 1 (this enables vectored interrupts)
    Riscv::w_stvec(handlerAddress);
   char * addr = (char*)mem_alloc(1024);
    addr[1] = 'a';
    int ret = mem_free(addr);
    printInt(ret);
}
