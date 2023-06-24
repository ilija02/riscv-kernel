.macro PUSH_REGS #REMOVED x10 (a0) register BE CAUTIOS
addi sp, sp, -256
.irp index, 0,1,2,3,4,5,6,7,8,9,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31
sd x\index, \index*8(sp)
.endr
.endm

.macro POP_REGS#REMOVED x10 (a0) register BE CAUTIOS
.irp index, 0,1,2,3,4,5,6,7,8,9,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31
ld x\index, \index*8(sp)
.endr
addi sp, sp, 256
.endm

.extern handleSupervisorTrap
.extern handleTimerTrap
.extern handleKeyboardTrap
.global trapHandler
.align 4
trapHandler:
    j processSyscallAndExceptions #offset 0
    j processTimer #offset 1*4
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    j processKeyboard #offset 9*4
processSyscallAndExceptions:
    # save all registers
    PUSH_REGS
    call handleSupervisorTrap
    csrr t0, sepc #read in saved pc
    addi t0,t0, 4 #increment it by four because the saved address is the ecall instruction
    csrw sepc, t0 #write back the modified pc
    # restore all registers
    POP_REGS
    j return
processTimer:
    # save all registers
    PUSH_REGS
    call handleTimerTrap
    # restore all registers
    POP_REGS
    j return
processKeyboard:
    # save all registers
    PUSH_REGS
    call handleKeyboardTrap
    # restore all registers
    POP_REGS
    #j return
return:
    sret

