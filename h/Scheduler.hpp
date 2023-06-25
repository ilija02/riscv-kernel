#ifndef SCHEDULER_HPP
#define SCHEDULER_HPP

#include "Dequeue.hpp"

class TCB;

class Scheduler {
public:
  static Scheduler &get();
  TCB *get_tcb();
  void put_tcb(TCB *tcb);
private:
  Scheduler() = default;
  Scheduler(const Scheduler &) = delete; // delete copy constructor because of Singleton pattern
  Scheduler &operator=(const Scheduler &) = delete; // delete copy assignment because of Singleton pattern
  Dequeue<TCB> ready_queue;
};

#endif //SCHEDULER_HPP
