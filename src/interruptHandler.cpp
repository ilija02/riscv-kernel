#include "../h/interruptHandler.hpp"

extern "C" void handleSupervisorTrap() {
  uint64 volatile processSyscall = 0;
  const uint64 volatile scause = RiscV::r_scause();
  if (scause == InterruptCause::IRQ_ILLEGAL_INSTRUCTION)
  {

  }
  else if (scause == IRQ_ILLEGAL_READ_ADDRESS)
  {

  }
  else if (scause == IRQ_ILLEGAL_WRITE_ADDRESS)
  {

  }
  else if (scause == IRQ_ILLEGAL_INSTRUCTION)
  {

  }
  else if (scause == IRQ_SYSCALL_USER_MODE || scause == IRQ_SYSCALL_KERNEL_MODE)
  {
    processSyscall = 1;
  }
  if (!processSyscall)
  {
    RiscV::mc_sip(RiscV::SIP_SSIE); // clear SSIP bit
    return;
  }

  uint64 volatile syscall_id;
  syscall_id = RiscV::r_a0();

  if (syscall_id == SyscallID::MEM_ALLOC)
  {
    uint64 volatile size = RiscV::r_a1();
    void *volatile allocated_chunk = MemoryAllocator::get().mem_alloc(size);
    RiscV::mc_sip(RiscV::SIP_SSIE);
    RiscV::w_a0(reinterpret_cast<uint64>(allocated_chunk)); // write the return value into a0
    return;
  }
  else if (syscall_id == SyscallID::MEM_FREE)
  {
    void *volatile chunk = reinterpret_cast<void *>(RiscV::r_a1());
    uint64 volatile return_code = MemoryAllocator::get().mem_free(chunk);
    RiscV::w_a0(return_code);
  }

  RiscV::mc_sip(RiscV::SIP_SSIE); // clear SSIP bit
}

extern "C" void handleTimerTrap() {

}

extern "C" void handleKeyboardTrap() {
  console_handler();
}