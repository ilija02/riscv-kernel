
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
