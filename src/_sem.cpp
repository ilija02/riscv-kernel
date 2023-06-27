#include "../h/_sem.hpp"

uint64 _sem::create_semaphore(_sem **handle, uint32 initial_value) {
  _sem* created_semaphore = new _sem(initial_value);
  if (handle != nullptr) *handle = created_semaphore;
  else return -1;
  return 0;
}

void _sem::block() {
  _thread::running->block();
  blocked_threads.push_back(_thread::running);
  _thread::yield();
}

void _sem::unblock() {
  _thread* unblocked_thread = blocked_threads.pop_front();
  unblocked_thread->resume(); // set state to ready
  Scheduler::get().put_tcb(unblocked_thread);
}

int _sem::wait() {
  if (--this->val < 0) block();
  return 0;
}

int _sem::signal() {
  if (++this->val <= 0) this->unblock();
  return 0;
}
