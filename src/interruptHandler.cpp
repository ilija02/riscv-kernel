#include "../h/interruptHandler.hpp"

inline void print_diagnostics(){
  const uint64 volatile scause = RiscV::r_scause();
  const uint64 volatile stval = RiscV::r_stval();
  const uint64 volatile sstatus = RiscV::r_sstatus();
  const uint64 volatile sepc = RiscV::r_sepc();
  printString("Unknown exception cause. \n");
  printString("stval: ");
  printInt(stval, 16);
  printString("\n");
  printString("scause: ");
  printInt(scause, 16);
  printString("\n");
  printString("sstatus: ");
  printInt(sstatus, 16);
  printString("\n");
  printString("sepc: ");
  printInt(sepc, 16);
  printString("\n");
}
// takes "implicit" arguments as they are in registers a0 to a7
extern "C" uint64 handleSupervisorTrap(uint64 syscall_id, void *a1, void *a2, void *a3, void *a4) {
  uint64 volatile processSyscall = 0;
  uint64 volatile ret_val;
  const uint64 volatile scause = RiscV::r_scause();
  if (scause == InterruptCause::IRQ_ILLEGAL_INSTRUCTION) {
    printString("Exception: Illegal instruction\n");
  } else if (scause == IRQ_ILLEGAL_READ_ADDRESS) {
    printString("Exception: Illegal read address\n");
  } else if (scause == IRQ_ILLEGAL_WRITE_ADDRESS) {
    printString("Exception: Illegal write address\n");
  } else if (scause == IRQ_SYSCALL_USER_MODE || scause == IRQ_SYSCALL_KERNEL_MODE) {
    processSyscall = 1;
    //if (scause==IRQ_SYSCALL_USER_MODE) printString("User mode happened. \n");
  }

  if (!processSyscall) print_diagnostics(); //exception happened
  // Note: The return value of the corresponding kernel functions is implicitly stored in a0, and then collected from a0 by corresponding C api call.
  if (syscall_id == SyscallID::MEM_ALLOC)
    ret_val = (uint64) MemoryAllocator::get().mem_alloc((uint64) a1); //a1 - size param

  else if (syscall_id == SyscallID::MEM_FREE)
    ret_val = MemoryAllocator::get().mem_free(a1); //a1 - chunk

  else if (syscall_id == SyscallID::THREAD_CREATE)
    // a1 - handle, a2 - start_routine, a3 - argument, a4 - allocated stack
    ret_val = TCB::create_thread((TCB **) a1, (TCB::Task) a2, a3, (uint64 *) a4);
  else if (syscall_id == SyscallID::THREAD_EXIT)
    ret_val = TCB::exit_thread();
  else if (syscall_id == SyscallID::THREAD_DISPATCH) {
    uint64 volatile sepc = RiscV::r_sepc();
    uint64 volatile sstatus = RiscV::r_sstatus();
    TCB::dispatch();
    RiscV::w_sepc(sepc);
    RiscV::w_sstatus(sstatus);
  } else if (syscall_id == SyscallID::THREAD_JOIN) {
    if (a1 == nullptr) return ret_val;
    uint64 volatile sepc = RiscV::r_sepc();
    uint64 volatile sstatus = RiscV::r_sstatus();
    ret_val = ((TCB *) a1)->join();
    RiscV::w_sepc(sepc);
    RiscV::w_sstatus(sstatus);
  }

  //a1 - thread to perform a join on

  // There's no need to clear the SSIP bit as in other handlers because interrupt bit is not set.
  return ret_val;
}


extern "C" void handleTimerTrap() {
  static uint64 counter = 0;
  static uint64  seconds = 0;
  const uint64 volatile scause = RiscV::r_scause();
  if (scause == IRQ_TIMER) {
    if (counter == 10) {
      seconds++;
      printString("Timer: ");
      printInt(seconds);
      printString("s \n");
      counter = 0;
    }
    counter++;
    RiscV::mc_sip(RiscV::SIP_SSIE);
  }

}

extern "C" void handleKeyboardTrap() {
  console_handler();
}