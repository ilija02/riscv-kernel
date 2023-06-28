#ifndef  INTERRUPT_HANDLER_HPP
#define  INTERRUPT_HANDLER_HPP
#include "../lib/hw.h"
#include "../h/RiscV.hpp"
#include "../h/print.hpp"
#include "../h/syscall_id.hpp"
#include "../h/MemoryAllocator.hpp"
#include "../h/_thread.hpp"
#include "../h/_sem.hpp"

#define SAVE_SSTATUS_SEPC(code) \
    uint64 volatile sepc = RiscV::r_sepc(); \
    uint64 volatile sstatus = RiscV::r_sstatus(); \
    code \
    RiscV::w_sepc(sepc); \
    RiscV::w_sstatus(sstatus);

enum InterruptCause : uint64
{
	IRQ_TIMER = 0x8000000000000001UL, // sent as software interrupt from the most privileged mode
	IRQ_HARDWARE = 0x8000000000000009UL,  //keyboard
	IRQ_ILLEGAL_INSTRUCTION = 0x0000000000000002UL,
	IRQ_ILLEGAL_READ_ADDRESS = 0x0000000000000005UL,
	IRQ_ILLEGAL_WRITE_ADDRESS = 0x0000000000000007UL,
	IRQ_SYSCALL_USER_MODE = 0x0000000000000008UL,
	IRQ_SYSCALL_KERNEL_MODE = 0x0000000000000009UL
};

#endif