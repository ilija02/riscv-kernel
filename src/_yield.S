.extern _ZN7_thread8dispatchEv
.global _ZN7_thread5yieldEv
.type _ZN7_thread5yieldEv, @function
_ZN7_thread5yieldEv:
    addi sp, sp, -256
    .irp index, 1,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31 # no need to save sp, ra needs to be saved
    sd x\index, \index*8(sp)
    .endr

    call _ZN7_thread8dispatchEv

    .irp index, 1,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31
    ld x\index, \index*8(sp)
    .endr
    addi sp, sp, 256

    ret