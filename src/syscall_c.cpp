
#include "../h/syscall_c.h"

/**
 * Wrapper function for performing a syscall, takes up to 4 generic arguments and a syscall identifier.
 * and performs an environment call instruction, passing the arguments through registers a1..a4
 * @param syscall_id
 * @param a1 first argument for the syscall
 * @param a2 second argument for the syscall
 * @param a3 third argument for the syscall
 * @param a4 fourth argument for the syscall
 * @return return value of the performed syscall, stored in the a0 register.
 */
uint64 perform_syscall(uint64 syscall_id,
                       void *a1 = nullptr,
                       void *a2 = nullptr,
                       void *a3 = nullptr,
                       void *a4 = nullptr) {
  //arguments are passed to the handleSupervisorTrap through registers
  uint64 return_value;
  RiscV::ecall(); // calls the interrupt handler
  return_value = RiscV::r_a0();
  return return_value;
}

void *mem_alloc(size_t size) {
  return (void *) perform_syscall(SyscallID::MEM_ALLOC, (void *) size);
}

int mem_free(void *chunk) {
  return (int) perform_syscall(SyscallID::MEM_FREE, chunk);
}

int thread_create(thread_t *handle, void (*start_routine)(void *), void *arg) {
  uint64 *allocated_stack = nullptr;
  if (start_routine != nullptr) {
    allocated_stack = (uint64 *) mem_alloc(sizeof(uint64) * DEFAULT_STACK_SIZE);
    if (allocated_stack == nullptr) return -1; // stack allocation failed
  }
  return (int) perform_syscall(SyscallID::THREAD_CREATE,
                               (void *) handle,
                               (void *) start_routine,
                               (void *) arg,
                               (void *) allocated_stack);
}

int thread_exit() {
  return perform_syscall(SyscallID::THREAD_EXIT);
}

void thread_dispatch() {
  perform_syscall(SyscallID::THREAD_DISPATCH);
}

void thread_join(thread_t handle) {
  perform_syscall(SyscallID::THREAD_JOIN, handle);
}

int sem_open(sem_t *handle, unsigned int init) {
  return (int) perform_syscall(SyscallID::SEM_OPEN, (void *) handle, (void *) ((uint64) init));
}
int sem_close(sem_t handle) {
  return (int) perform_syscall(SyscallID::SEM_CLOSE, (void *) handle);
}
int sem_wait(sem_t id) {
  return (int) perform_syscall(SyscallID::SEM_WAIT, (void *) id);
}
int sem_signal(sem_t id) {
  return (int) perform_syscall(SyscallID::SEM_SIGNAL, (void *) id);
}

int time_sleep(time_t time) {
  //TODO implement function
  return 0;
}

char getc() {
  return perform_syscall(SyscallID::GETC);
 // return __getc();
}

void putc(char c){
  perform_syscall(SyscallID::PUTC, (void *) ((uint64) c));
  //__putc(c);
}
