#include "../lib/console.h"
#include "../h/MemoryAllocator.hpp"
#include "../h/riscv.hpp"


extern "C" void trapHandler();

void main() {
    /*char* mem = (char*)MemoryAllocator::get().mem_alloc(1024);
    for(int i = 0; i< 1024; i++) mem[i] = 'a';
    MemoryAllocator::get().mem_free(mem);*/
    uint64 volatile handlerAddress = (uint64) &trapHandler | 0x01; //set the base address for interrupts to trap handler and
    //set mode to 1 (this enables vectored interrupts)
    __asm__ volatile ("csrw stvec, %[vector]" : : [vector]"r"(handlerAddress));
    //Riscv::ms_sstatus(0x02);
    __putc('A');
    __asm__ volatile ("ecall");
}
