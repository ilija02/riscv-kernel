#include "../h/UnitTest.hpp"
#include "../h/syscall_c.h"
#include "../h/printing.hpp"
#include "../h/riscv.hpp"

UnitTest &UnitTest::get() {
    static UnitTest instance;
    return instance;
}

UnitTest::UnitTest() {
    uint64 volatile handlerAddress = (uint64) &trapHandler | 0x01; //set the base address for interrupts to trap handler and
    //set mode to 1 (this enables vectored interrupts)
    Riscv::w_stvec(handlerAddress);

}

bool UnitTest::test_memory_allocator() {
    MemoryAllocator& instance  = MemoryAllocator::get();
    BlockHeader* free_head = (BlockHeader*)instance.get_free_head();
    printString("Free head size after initialization: ");
    uint64  old_size = free_head->size_in_bytes;
    printInt(old_size);
    printString("\n");
    void * a1 = mem_alloc(1024);
    void * a2 = mem_alloc(1024);
    void * a3 = mem_alloc(1024);
    mem_free(a1);
    mem_free(a2);
    mem_free(a3);
    free_head = (BlockHeader*)instance.get_free_head();
    printString("Free head size after freeing memory: ");
    uint64  new_size = free_head->size_in_bytes;
    printInt(new_size);
    printString("\n");
    return old_size==new_size;
}
