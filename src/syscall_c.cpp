
#include "../h/syscall_c.h"
void *mem_alloc(size_t size) {
    void* allocated_chunk = nullptr;
    Riscv::w_a1(size); //need to write into a1 first because otherwise the value of size will be lost (that's a0 because its the first arg of mem_alloc)
    Riscv::w_a0(SyscallID::MEM_ALLOC);
    Riscv::ecall();
    allocated_chunk = reinterpret_cast<void*> (Riscv::r_a0());
    return allocated_chunk;
}

int mem_free(void *chunk) {
    int volatile return_code;
    Riscv::w_a1(reinterpret_cast<uint64>(chunk));
    Riscv::w_a0(SyscallID::MEM_FREE);
    Riscv::ecall();
    return_code = Riscv::r_a0();
    return return_code;
}
