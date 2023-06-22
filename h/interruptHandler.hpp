#ifndef  INTERRUPT_HANDLER_HPP
#define  INTERRUPT_HANDLER_HPP
#include "../lib/hw.h"
#include "../h/riscv.hpp"
#include "../lib/console.h"

enum InterruptCause:uint64 {
    IRQ_TIMER = 0x8000000000000001UL, // sent as software interrupt from the most privileged mode
    IRQ_HARDWARE = 0x8000000000000009UL,  //keyboard
    IRQ_ILLEGAL_INSTRUCTION = 0x0000000000000002UL,
    IRQ_ILLEGAL_READ_ADDRESS = 0x0000000000000005UL,
    IRQ_ILLEGAL_WRITE_ADDRESS = 0x0000000000000007UL,
    IRQ_SYSCALL_USERMODE = 0x0000000000000008UL,
    IRQ_SYSCALL_KERNELMODE = 0x0000000000000009UL
};

enum SyscallID:uint64 {
    MEM_ALLOC = 0x01,
    MEM_FREE = 0x02,
    THREAD_CREATE = 0x011,
    THREAD_EXIT = 0x12,
    THREAD_DISPATCH = 0x13,
    THREAD_JOIN = 0x14,
    SEM_OPEN = 0x21,
    SEM_CLOSE = 0x22,
    SEM_WAIT = 0x23,
    SEM_SIGNAL = 0x24,
    TIME_SLEEP = 0x31,
    GETC = 0x41,
    PUTC = 0X42
};
#endif