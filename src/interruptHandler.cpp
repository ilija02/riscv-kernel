#include "../h/interruptHandler.hpp"

extern "C" void handleSupervisorTrap() {
    uint64 volatile processSyscall = 0;
    const uint64 volatile scause = Riscv::r_scause();
    if (scause == InterruptCause::IRQ_ILLEGAL_INSTRUCTION) {

    } else if (scause == IRQ_ILLEGAL_READ_ADDRESS) {

    } else if (scause == IRQ_ILLEGAL_WRITE_ADDRESS) {

    } else if (scause == IRQ_ILLEGAL_INSTRUCTION) {

    } else if (scause == IRQ_SYSCALL_USER_MODE || scause == IRQ_SYSCALL_KERNEL_MODE) {
        processSyscall = 1;
    }
    if (!processSyscall){
        Riscv::mc_sip(0x02); // clear SSIP bit
        return;
    }

    uint64 volatile syscall_id;
    Riscv::r_a0();

    Riscv::mc_sip(0x02); // clear SSIP bit
}

extern "C" void handleTimerTrap() {

}

extern "C" void handleKeyboardTrap() {

}