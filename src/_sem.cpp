#include "../h/_sem.hpp"

uint64 _sem::create_semaphore(_sem **handle, uint64 initial_value) {
  _sem* created_semaphore = new _sem(initial_value);
  if (handle != nullptr) *handle = created_semaphore;
  else return -1;
  return 0;
}

void _sem::block() {

}

void _sem::unblock() {

}
int _sem::wait() {

  return 0;
}
int _sem::signal() {

  return 0;
}
