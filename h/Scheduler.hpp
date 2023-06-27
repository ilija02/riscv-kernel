#ifndef SCHEDULER_HPP
#define SCHEDULER_HPP

#include "Dequeue.hpp"

class _thread;

class Scheduler {
public:
  static Scheduler &get();
  _thread *get_tcb();
  void put_tcb(_thread *tcb);
private:
  Scheduler() = default;
  Scheduler(const Scheduler &) = delete; // delete copy constructor because of Singleton pattern
  Scheduler &operator=(const Scheduler &) = delete; // delete copy assignment because of Singleton pattern
  Dequeue<_thread> ready_queue;
};

#endif //SCHEDULER_HPP
