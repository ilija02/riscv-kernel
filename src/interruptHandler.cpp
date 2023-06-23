#include "../h/interruptHandler.hpp"

extern "C" void handleSupervisorTrap(){
    bool processSyscall = false;
    const uint64 volatile scause = Riscv::r_scause();
    switch (scause){
        case InterruptCause::IRQ_ILLEGAL_INSTRUCTION: {
            //illegal instruction
            printString("Illegar instruction.\n");
            break;
        }
        case InterruptCause::IRQ_ILLEGAL_READ_ADDRESS: {
            printString("Illegal read address");
            break;
        }

        case InterruptCause::IRQ_ILLEGAL_WRITE_ADDRESS: {
            printString("Illegal write address");
            break;
        }
        case InterruptCause::IRQ_SYSCALL_KERNEL_MODE:
        case InterruptCause::IRQ_SYSCALL_USER_MODE: processSyscall = true; break;
        default: {
            printString("Unknown ")
        }
    }
    Riscv::mc_sip(0x02);
    console_handler();
}

extern "C" void handleTimerTrap(){

}

extern "C" void handleKeyboardTrap(){

}