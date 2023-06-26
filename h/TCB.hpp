#ifndef BASE_TCB_HPP
#define BASE_TCB_HPP

#include "../lib/hw.h"
#include "../h/Scheduler.hpp"

class TCB {
public:
  // task is a pointer to a function that has no return value and takes one void* argument
  using Task = void (*)(void *);
  enum ThreadState {
    CREATED, READY, RUNNING, SUSPENDED, FINISHED
  };
  static TCB *running;

  ~TCB() { MemoryAllocator::get().mem_free(allocated_stack); }

  static uint64 create_thread(TCB** handle, Task task, void *argument, uint64 *allocatedStack);
  static void exit_thread();
  static void yield(); // implemented in _yield.S

  static void dispatch();

  void finish() { this->state = ThreadState::FINISHED; }

  bool is_finished() const { return this->state == ThreadState::FINISHED; }
  bool is_running() const {return this->state == ThreadState::RUNNING; }
  void *operator new(size_t size) { return MemoryAllocator::get().mem_alloc(size); }

  void operator delete(void *chunk) { MemoryAllocator::get().mem_free(chunk); }

private:
  struct SavedContext {
    uint64 ra;
    uint64 sp;
  };
  // stack is allocated before calling TCB::CreateThread. It is allocated in the C api syscall.
  explicit TCB(TCB::Task task,
               void *argument,
               uint64 *allocated_stack);

  static void context_switch(SavedContext *old_context, SavedContext *new_context); //implemented in _contextSwitch.s
  static void thread_wrapper();
  ThreadState state = ThreadState::CREATED;
  SavedContext saved_context = {0,0};
  Task task = nullptr;
  void* argument;
  uint64 *allocated_stack = nullptr;
  TCB** my_handle; // my_handle is populated in create_thread
};

#endif //BASE_TCB_HPP