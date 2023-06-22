#include "../lib/console.h"
#include "../h/MemoryAllocator.hpp"
#include "../h/riscv.hpp"


extern "C" void trapHandler();

void main(){
    /*char* mem = (char*)MemoryAllocator::get().mem_alloc(1024);
    for(int i = 0; i< 1024; i++) mem[i] = 'a';
    MemoryAllocator::get().mem_free(mem);*/
    __asm__ volatile ("csrw stvec, %[vector]" : : [vector]"r"(&trapHandler));
    Riscv::ms_sstatus(0x02);
    while(1){

    }
}
