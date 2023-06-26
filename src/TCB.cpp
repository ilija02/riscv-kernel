#include "../h/TCB.hpp"

TCB *TCB::running = nullptr;
TCB::TCB(TCB::Task task, void *argument, uint64 *allocated_stack) {
  if (task == nullptr) return;
  this->task = task;
  this->argument = argument;
  this->allocated_stack = allocated_stack;
  this->saved_context.sp =
      reinterpret_cast<uint64 >(&allocated_stack[DEFAULT_STACK_SIZE]); // since the stack grows downward, set the stack pointer to the top of allocated stackSch
  this->saved_context.ra = (uint64) task;
  this->state = ThreadState::READY;
  Scheduler::get().put_tcb(this);
}

void TCB::dispatch() {
  TCB *old_running = TCB::running;
  if (!old_running->is_finished()){
    Scheduler::get().put_tcb(old_running);
    old_running->state = READY;
  }

  TCB::running = Scheduler::get().get_tcb();
  TCB::running->state = RUNNING;
  context_switch(&old_running->saved_context, &TCB::running->saved_context);
}

uint64 TCB::create_thread(TCB **handle, TCB::Task task, void *argument, uint64 *allocatedStack) {
  TCB *created_thread = new TCB(task, argument, allocatedStack);
  if (handle != nullptr) *handle = created_thread;
  if (created_thread == nullptr) return -1; // might be null as we're using our own memory allocator
  return 0;
}
void TCB::thread_wrapper() {

}

