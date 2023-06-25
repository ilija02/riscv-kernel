#include "../h/TCB.hpp"

TCB *TCB::running = nullptr;
TCB::TCB(TCB::Task task, void *argument, uint64 *allocated_stack) {
  this->state = ThreadState::CREATED;
  if (task == nullptr) {
    this->allocated_stack = nullptr;
    this->saved_context.sp = 0;
  } else {
    this->allocated_stack = allocated_stack;
    this->saved_context.sp =
        reinterpret_cast<uint64 >(&allocated_stack[DEFAULT_STACK_SIZE]); // since the stack grows downward, set the stack pointer to the top of allocated stack
  }
}

void TCB::dispatch() {
  TCB *old_running = running;
  if (!(old_running->state == ThreadState::FINISHED))
    Scheduler::get().put_tcb(old_running);
  running = Scheduler::get().get_tcb();
  context_switch(&old_running->saved_context, &running->saved_context);
}

TCB *TCB::createThread(TCB::Task task, void *argument, uint64 *allocated_stack) {
  return new TCB(task, argument, allocated_stack);
}

