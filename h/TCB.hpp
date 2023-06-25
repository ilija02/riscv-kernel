#ifndef BASE_TCB_HPP
#define BASE_TCB_HPP

#include "../lib/hw.h"
#include "../h/Scheduler.hpp"

class TCB {
public:
  // task is a pointer to a function that has no return value and takes one void* argument
  using Task = void (*)(void *);
  enum ThreadState { CREATED, FINISHED };
  static TCB *running;
  ~TCB() { MemoryAllocator::get().mem_free(allocated_stack); }
  static TCB *createThread(Task task, void *argument, uint64 *allocatedStack);
  static void yield(); // implemented in _yield.S
  static void dispatch();
  void finish() { this->state = ThreadState::FINISHED; }
private:
  struct SavedContext {
    uint64 ra;
    uint64 sp;
  };

  explicit TCB(TCB::Task task,
               void *argument,
               uint64 *allocated_stack); // stack is allocated before calling TCB::CreateThread. It is allocated in the C api syscall.
  void *operator new(size_t size) { return MemoryAllocator::get().mem_alloc(size); }
  void operator delete(void *chunk) { MemoryAllocator::get().mem_free(chunk); }
  static void context_switch(SavedContext *old_context, SavedContext *new_context); //implemented in _contextSwitch.s
  ThreadState state;
  SavedContext saved_context;
  Task task;
  uint64 *allocated_stack;

};

#endif //BASE_TCB_HPP
