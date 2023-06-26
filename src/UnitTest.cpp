#include "../h/UnitTest.hpp"
#include "../h/syscall_c.h"
#include "../h/printing.hpp"
#include "../h/RiscV.hpp"

UnitTest &UnitTest::get() {
  static UnitTest instance;
  return instance;
}

UnitTest::UnitTest() {

}

bool UnitTest::test_memory_allocator() {
  MemoryAllocator &instance = MemoryAllocator::get();
  BlockHeader *free_head = (BlockHeader *) instance.get_free_head();
  printString("Free head size after initialization: ");
  uint64 old_size = free_head->size_in_bytes;
  printInt(old_size);
  printString("\n");
  void *a1 = mem_alloc(1024);
  void *a2 = mem_alloc(1024);
  void *a3 = mem_alloc(1024);
  mem_free(a1);
  mem_free(a2);
  mem_free(a3);
  free_head = (BlockHeader *) instance.get_free_head();
  printString("Free head size after freeing memory: ");
  uint64 new_size = free_head->size_in_bytes;
  printInt(new_size);
  printString("\n");
  return old_size == new_size;
}

bool UnitTest::test_new_delete() {

  MemoryAllocator &instance = MemoryAllocator::get();
  BlockHeader *free_head = (BlockHeader *) instance.get_free_head();
  printString("Free head size after initialization: ");
  uint64 old_size = free_head->size_in_bytes;
  printInt(old_size);
  printString("\n");

  TestStruct *t = new TestStruct();
  t->x = 5;
  t->y = 10;

  uint64 *arr = new uint64[1000];
  for (int i = 0; i < 1000; i++)
  {
    arr[i] = i;
  }
  free_head = (BlockHeader *) instance.get_free_head();
  printString("Free head size after allocating a struct and an array: ");
  uint64 allocated_size = free_head->size_in_bytes;
  printInt(allocated_size);
  printString("\n");

  printString("Number of bytes allocated: ");
  printInt(old_size - allocated_size);
  printString("\n");
  delete t;
  delete[] arr;

  free_head = (BlockHeader *) instance.get_free_head();
  printString("Free head size after freeing memory: ");
  uint64 new_size = free_head->size_in_bytes;
  printInt(new_size);
  printString("\n");
  printString("Test of new and delete completed.\n");

  return old_size == new_size;
}

bool UnitTest::test_dequeue() {
  uint64 x = 5, y = 10, z = 15, w = 20;
  Dequeue<uint64> dequeue;

  //5 10
  dequeue.push_back(&x);
  dequeue.push_back(&y);
  uint64 *tmp = dequeue.front();
  printInt(*tmp);
  tmp = dequeue.back();
  printInt(*tmp);
  //20 15 5 10
  dequeue.push_front(&z);
  dequeue.push_front(&w);
  tmp = dequeue.front();
  printInt(*tmp);
  tmp = dequeue.back();
  printInt(*tmp);

  dequeue.pop_front();
  tmp = dequeue.front();
  printInt(*tmp);

  dequeue.pop_back();
  tmp = dequeue.back();
  printInt(*tmp);
  return true;
}
bool UnitTest::test_synchronous_context_switching() {
  printString("------ Testing: test_synchronous_context_switching --------\n");
  TCB *threads[3];
  TCB::create_thread(&threads[0], nullptr, nullptr, nullptr);
  uint64 *stack1 = new uint64[DEFAULT_STACK_SIZE];
  uint64 *stack2 = new uint64[DEFAULT_STACK_SIZE];
  TCB::running = threads[0];
  TCB::create_thread(&threads[1],workerBodyA, nullptr, stack1);
  printString("Thread A created\n");
  TCB::create_thread(&threads[2], workerBodyB, nullptr, stack2);
  printString("Thread B created\n");
  while (!(threads[1]->is_finished() && threads[2]->is_finished())) {
    TCB::yield();
  }

  for (auto &thread: threads) delete thread;
  printString("----- Finished test: test_synchronous_context_switching -----\n");
  return true;
}
bool UnitTest::test_thread_create() {
  printString("------Testing: test_thread_create --------\n");
  thread_t threads[3] = {nullptr};
  printString("Address of threads[0] :");
  printInt((uint64)&threads[0], 16);
  printString("\n");
  if (thread_create(&threads[0], nullptr, nullptr) < 0){
    printString("Failed creating main thread.");
    return false;
  }
  TCB::running = threads[0];
  if (thread_create(&threads[1], workerBodyA, nullptr) < 0){
    printString("Failed creating A thread.");
    return false;
  }
  printString("Thread A created\n");

  if (thread_create(&threads[2], workerBodyB, nullptr) < 0 ){
    printString("Failed creating B thread.");
  }
  printString("Thread B created\n");
  while (!(threads[1]->is_finished() && threads[2]->is_finished())) {
    TCB::yield();
  }

  for (auto &thread: threads) delete thread;
  printString("----- Finished test: test_thread_create -----\n");
  return true;
}
