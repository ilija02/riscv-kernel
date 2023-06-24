
#include "../h/syscall_c.h"
void *mem_alloc(size_t size) {
    void* allocated_chunk = nullptr;
    Riscv::w_a1(size);
    Riscv::w_a0(SyscallID::MEM_ALLOC);
    Riscv::ecall();
    allocated_chunk = reinterpret_cast<void*> (Riscv::r_a0());
    return allocated_chunk;
}
