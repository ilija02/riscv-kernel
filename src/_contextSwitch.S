.global _ZN7_thread14context_switchEPNS_12SavedContextES1_
.type _ZN7_thread14context_switchEPNS_12SavedContextES1_, @function
_ZN7_thread14context_switchEPNS_12SavedContextES1_: # _thread::context_switch(SavedContext* old_context, SavedContext* new_context)
    sd ra, 0*8(a0) # store old ra
    sd sp, 1*8(a0) # store old sp

    ld ra, 0*8(a1) # restore new ra
    ld sp , 1*8(a1) #restore new sp

    ret