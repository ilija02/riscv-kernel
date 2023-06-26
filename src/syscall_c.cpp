
#include "../h/syscall_c.h"
void *mem_alloc(size_t size) {
  void *allocated_chunk = nullptr;
  RiscV::w_a1(size); //need to write into a1 first because otherwise the value of size will be lost (that's a0 because its the first arg of mem_alloc)
  RiscV::w_a0(SyscallID::MEM_ALLOC);
  RiscV::ecall();
  allocated_chunk = reinterpret_cast<void *> (RiscV::r_a0());
  return allocated_chunk;
}

int mem_free(void *chunk) {
  int volatile return_code;
  RiscV::w_a1(reinterpret_cast<uint64>(chunk));
  RiscV::w_a0(SyscallID::MEM_FREE);
  RiscV::ecall();
  return_code = RiscV::r_a0();
  return return_code;
}
int thread_create(thread_t *handle, void (*start_routine)(void *), void *arg) {
  uint64 * allocated_stack = nullptr;
  if (start_routine != nullptr) {
    allocated_stack = (uint64 *)mem_alloc(sizeof(uint64)*DEFAULT_STACK_SIZE);
    if (allocated_stack == nullptr) return -1; // stack allocation failed
  }
  RiscV::w_a4(reinterpret_cast<uint64>(allocated_stack));
  RiscV::w_a3(reinterpret_cast<uint64>(arg));
  RiscV::w_a2(reinterpret_cast<uint64>(start_routine));
  RiscV::w_a1(reinterpret_cast<uint64>(handle));
  RiscV::w_a0(SyscallID::THREAD_CREATE);
  RiscV::ecall();
  uint64 volatile ret_val = RiscV::r_a0();
  if (ret_val != 0) return -2; // thread creation failed
  return 0; // thread creation was successful
}
