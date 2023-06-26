#include "../h/interruptHandler.hpp"

// takes "implicit" arguments as they are in registers a0 to a7
extern "C" void handleSupervisorTrap(uint64 syscall_id, void *a1, void *a2, void *a3, void *a4) {
  uint64 volatile processSyscall = 0;
  const uint64 volatile scause = RiscV::r_scause();
  if (scause == InterruptCause::IRQ_ILLEGAL_INSTRUCTION) {
    printString("Exception: Illegal instruction\n");
  } else if (scause == IRQ_ILLEGAL_READ_ADDRESS) {
    printString("Exception: Illegal read address\n");
  } else if (scause == IRQ_ILLEGAL_WRITE_ADDRESS) {
    printString("Exception: Illegal write address\n");
  } else if (scause == IRQ_SYSCALL_USER_MODE || scause == IRQ_SYSCALL_KERNEL_MODE) {
    processSyscall = 1;
  } else {
    printString("Unknown exception cause. \n");
  }

  if (!processSyscall) return;
  // Note: The return value of the corresponding kernel functions is implicitly stored in a0, and then collected from a0 by corresponding C api call.
  if (syscall_id == SyscallID::MEM_ALLOC)
    MemoryAllocator::get().mem_alloc((uint64) a1); //a1 - size param

  else if (syscall_id == SyscallID::MEM_FREE)
    MemoryAllocator::get().mem_free(a1); //a1 - chunk

  else if (syscall_id == SyscallID::THREAD_CREATE)
    // a1 - handle, a2 - start_routine, a3 - argument, a4 - allocated stack
    TCB::create_thread((TCB **) a1, (TCB::Task) a2, a3, (uint64 *) a4);

  // There's no need to clear the SSIP bit as in other handlers because interrupt bit is not set.
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