#include "../h/interruptHandler.hpp"
extern "C" void handleSupervisorTrap(){

    /*const uint64 volatile scause = Riscv::r_scause();
    switch (scause){
        case InterruptCause::IRQ_TIMER : {
            __putc('A');
        }
    }
    Riscv::mc_sip(0x02);*/
    __putc('B');
    console_handler();
}

extern "C" void handleTimerTrap(){

}

extern "C" void handleKeyboardTrap(){

}