//
// Created by os on 6/24/23.
//

#include "../h/Scheduler.hpp"

Scheduler &Scheduler::get() {
  static Scheduler instance;
  return instance;
}

TCB *Scheduler::get_tcb() {
  return ready_queue.pop_front();
}

void Scheduler::put_tcb(TCB *tcb) {
  ready_queue.push_back(tcb);
}
