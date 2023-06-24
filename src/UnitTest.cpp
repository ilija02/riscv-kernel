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

bool UnitTest::test_new_delete() {

    MemoryAllocator& instance = MemoryAllocator::get();
    BlockHeader* free_head = (BlockHeader*)instance.get_free_head();
    printString("Free head size after initialization: ");
    uint64  old_size = free_head->size_in_bytes;
    printInt(old_size);
    printString("\n");

    TestStruct *t = new TestStruct();
    t->x = 5;
    t->y = 10;

    uint64 *arr = new uint64 [1000];
    for (int i = 0; i <1000; i++){
        arr[i] = i;
    }
    free_head = (BlockHeader*)instance.get_free_head();
    printString("Free head size after allocating a struct and an array: ");
    uint64  allocated_size = free_head->size_in_bytes;
    printInt(allocated_size);
    printString("\n");

    printString("Number of bytes allocated: ");
    printInt(old_size-allocated_size);
    printString("\n");
    delete t;
    delete[] arr;

    free_head = (BlockHeader*)instance.get_free_head();
    printString("Free head size after freeing memory: ");
    uint64  new_size = free_head->size_in_bytes;
    printInt(new_size);
    printString("\n");
    printString("Test of new and delete completed.\n");

    return old_size==new_size;
}
