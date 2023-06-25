#include "../h/TCB.hpp"

TCB *TCB::running = nullptr;
TCB::TCB(TCB::Task task, void *argument, uint64 *allocated_stack) {
  this->state = ThreadState::CREATED;
  if (task == nullptr)
  {
    this->allocated_stack = nullptr;
    this->saved_context.sp = 0;
    this->saved_context.ra = 0;
  }
  else
  {
    this->allocated_stack = allocated_stack;
    this->saved_context.sp =
        reinterpret_cast<uint64 >(&allocated_stack[DEFAULT_STACK_SIZE]); // since the stack grows downward, set the stack pointer to the top of allocated stackSch
    this->saved_context.ra = (uint64) task;
    Scheduler::get().put_tcb(this);

  }

}

void TCB::dispatch() {
  TCB *old_running = running;
  if (!old_running->is_finished())
    Scheduler::get().put_tcb(old_running);
  TCB::running = Scheduler::get().get_tcb();
  context_switch(&old_running->saved_context, &TCB::running->saved_context);
}

TCB *TCB::create_thread(TCB::Task task, void *argument, uint64 *allocatedStack) {
  return new TCB(task, argument, allocatedStack);
}

