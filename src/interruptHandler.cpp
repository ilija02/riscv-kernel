#include "../h/interruptHandler.hpp"

// takes "implicit" arguments as they are in registers a0 to a7
extern "C" void handleSupervisorTrap(uint64 syscall_id,  void* a1, void* a2, void* a3, void* a4) {
  uint64 volatile processSyscall = 0;
  const uint64 volatile scause = RiscV::r_scause();
  if (scause == InterruptCause::IRQ_ILLEGAL_INSTRUCTION) {

  } else if (scause == IRQ_ILLEGAL_READ_ADDRESS) {
    printString("Exception: Illegal read address\n");
  } else if (scause == IRQ_ILLEGAL_WRITE_ADDRESS) {
    printString("Exception: Illegal write address\n");
  } else if (scause == IRQ_ILLEGAL_INSTRUCTION) {
    printString("Exception: Illegal instruction\n");
  } else if (scause == IRQ_SYSCALL_USER_MODE || scause == IRQ_SYSCALL_KERNEL_MODE) {
    processSyscall = 1;
  } else {
    printString("Unknown exception cuase. \n");
  }

  if (!processSyscall) {
    RiscV::mc_sip(RiscV::SIP_SSIE); // clear SSIP bit
    return;
  }

  if (syscall_id == SyscallID::MEM_ALLOC) {
    void *volatile allocated_chunk = MemoryAllocator::get().mem_alloc((uint64)a1); //a1 - size param
    RiscV::w_a0(reinterpret_cast<uint64>(allocated_chunk)); // write the return value into a0
    //return;
  } else if (syscall_id == SyscallID::MEM_FREE) {
    uint64 volatile return_code = MemoryAllocator::get().mem_free(a1); //a1 - chunk
    RiscV::w_a0(return_code);
  } else if (syscall_id == SyscallID::THREAD_CREATE) {

  }
  RiscV::mc_sip(RiscV::SIP_SSIE); // clear SSIP bit
}

uint64 counter = 0;
extern "C" void handleTimerTrap() {

  const uint64 volatile scause = RiscV::r_scause();
  if (scause == IRQ_TIMER) {
    if (counter == 10) {
      __putc('A');
      counter = 0;
    }
    counter++;
    RiscV::mc_sip(RiscV::SIP_SSIE);
  }

}

extern "C" void handleKeyboardTrap() {
  console_handler();
}