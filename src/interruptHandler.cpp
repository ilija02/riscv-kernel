#include "../h/interruptHandler.hpp"
extern "C" void handleSupervisorTrap(){

    const uint64 volatile scause = Riscv::r_scause();
    switch (scause){
        case InterruptCause::IRQ_TIMER : {
            __putc('a');
        }
    }
    Riscv::mc_sip(0x02);
}
/*
extern "C" void timerTrap(){

}

extern "C" void keyboardTrap(){

} */