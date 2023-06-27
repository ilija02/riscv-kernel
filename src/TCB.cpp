#include "../h/TCB.hpp"

TCB *TCB::running = nullptr;
TCB::RunningMode TCB::running_mode = RunningMode::SYSTEM;

TCB::TCB(TCB::Task task, void *argument, uint64 *allocated_stack) {
  if (task == nullptr) return;
  this->parent_thread = TCB::running;
  this->task = task;
  this->argument = argument;
  this->allocated_stack = allocated_stack;
  this->saved_context.sp =
      reinterpret_cast<uint64 >(&allocated_stack[DEFAULT_STACK_SIZE]); // since the stack grows downward, set the stack pointer to the top of allocated stackSch
  this->saved_context.ra = (uint64) TCB::thread_wrapper;
  this->state = ThreadState::READY;
  Scheduler::get().put_tcb(this);
}

void TCB::dispatch() {
  TCB *old_running = TCB::running;
  if (!old_running->is_finished() && !old_running->is_suspended()) {
    Scheduler::get().put_tcb(old_running);
    old_running->state = READY;
  }
  else { // if the thread is finished and the parent is waiting for it, put the parent in the scheduler
    if (old_running->parent_thread && old_running->is_parent_waiting){
        old_running->is_parent_waiting = false;
        Scheduler::get().put_tcb(old_running->parent_thread);
    } //else delete old_running // if automatic deletion of exited threads is required uncomment this line. Note that the explicit deletion is then forbidden.
  }
  TCB::running = Scheduler::get().get_tcb();
  TCB::running->state = ThreadState::RUNNING;
  context_switch(&old_running->saved_context, &TCB::running->saved_context);
}

uint64 TCB::create_thread(TCB **handle, TCB::Task task, void *argument, uint64 *allocatedStack) {
  TCB *created_thread = new TCB(task, argument, allocatedStack);
  if (handle != nullptr) *handle = created_thread;
  if (created_thread == nullptr) return -1; // might be null as we're using our own memory allocator
  return 0;
}

void TCB::thread_wrapper() {
  if (TCB::running_mode == RunningMode::USER)
    TCB::switch_to_user_mode();
    /*if (TCB::switch_to_user_mode()){
      printString("Changed privilege. \n");
    }*/
  TCB::running->task(TCB::running->argument);
  exit_thread();
}

int TCB::exit_thread() {
  if (!TCB::running->is_running()) return -1;
  TCB::running->finish();
  TCB::dispatch();
  return 0;
}

int TCB::join() {
  if (this->parent_thread != TCB::running) return -1;
  if (this->is_finished()) return 0;
  this->is_parent_waiting = true;
  this->parent_thread->suspend();
  TCB::yield();
  return 0;
}

bool TCB::switch_to_user_mode() {
  uint64 volatile sstatus = RiscV::r_sstatus();
  if ( sstatus & RiscV::SSTATUS_SPP ){
    RiscV::mc_sstatus(RiscV::SSTATUS_SPP);
    __asm__ volatile ("csrw sepc, ra");
    __asm__ volatile ("sret");
    return true;
  } else return false; //already in user mode */
 return false;
}

