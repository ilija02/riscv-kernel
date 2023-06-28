#include "../h/interruptHandler.hpp"

extern "C" void haltProcessor(); // defined in halt.S

inline void print_diagnostics()
{
	const uint64 volatile scause = RiscV::r_scause();
	const uint64 volatile stval = RiscV::r_stval();
	const uint64 volatile sstatus = RiscV::r_sstatus();
	const uint64 volatile sepc = RiscV::r_sepc();
	print_string("stval: ");
	print_int(stval, 16);
	print_string("\n");
	print_string("scause: ");
	print_int(scause, 16);
	print_string("\n");
	print_string("sstatus: ");
	print_int(sstatus, 16);
	print_string("\n");
	print_string("sepc: ");
	print_int(sepc, 16);
	print_string("\n");
	haltProcessor();
}

// takes "implicit" arguments as they are in registers a0 to a7
extern "C" uint64 handleSupervisorTrap(uint64 syscall_id, void* a1, void* a2, void* a3, void* a4)
{
	uint64 volatile processSyscall = 0;
	uint64 volatile ret_val;
	const uint64 volatile scause = RiscV::r_scause();
	if (scause == InterruptCause::IRQ_ILLEGAL_INSTRUCTION)
	{
		print_string("Exception: Illegal instruction\n");
	}
	else if (scause == IRQ_ILLEGAL_READ_ADDRESS)
	{
		print_string("Exception: Illegal read address\n");
	}
	else if (scause == IRQ_ILLEGAL_WRITE_ADDRESS)
	{
		print_string("Exception: Illegal write address\n");
	}
	else if (scause == IRQ_SYSCALL_USER_MODE || scause == IRQ_SYSCALL_KERNEL_MODE)
		processSyscall = 1;
	//if (scause==IRQ_SYSCALL_USER_MODE) print_string("User mode happened. \n");

	if (!processSyscall) print_diagnostics(); //exception happened
	// Note: The return value of the corresponding kernel functions is implicitly stored in a0, and then collected from a0 by corresponding C api call.
	if (syscall_id == SyscallID::MEM_ALLOC)
		ret_val = (uint64)MemoryAllocator::get().mem_alloc((uint64)a1); //a1 - size param

	else if (syscall_id == SyscallID::MEM_FREE)
		ret_val = MemoryAllocator::get().mem_free(a1); //a1 - chunk

	else if (syscall_id == SyscallID::THREAD_CREATE)
		// a1 - handle, a2 - start_routine, a3 - argument, a4 - allocated stack
		ret_val = _thread::create_thread((_thread**)a1, (_thread::Task)a2, a3, (uint64*)a4);

	else if (syscall_id == SyscallID::THREAD_EXIT)
		ret_val = _thread::exit_thread();
	else if (syscall_id == SyscallID::THREAD_DISPATCH)
	{
		SAVE_SSTATUS_SEPC({ _thread::dispatch(); })
	}
	else if (syscall_id == SyscallID::THREAD_JOIN)
	{
		if (a1 == nullptr) return ret_val;
		SAVE_SSTATUS_SEPC({ ret_val = ((_thread*)a1)->join(); })
	}
	else if (syscall_id == SyscallID::SEM_OPEN)
		ret_val = _sem::create_semaphore((_sem**)a1, (uint64)a2);
	else if (syscall_id == SyscallID::SEM_CLOSE){
		((sem_t)a1)->close();
		ret_val = MemoryAllocator::get().mem_free((sem_t)a1);
	}

	else if (syscall_id == SyscallID::SEM_WAIT)
	{
		SAVE_SSTATUS_SEPC({ ret_val = ((sem_t)a1)->wait(); })
	}
	else if (syscall_id == SyscallID::SEM_SIGNAL)
	{
		SAVE_SSTATUS_SEPC({ ret_val = ((sem_t)a1)->signal(); })
	}
	else if (syscall_id == SyscallID::SWITCH_TO_USER)
		RiscV::mc_sstatus(RiscV::SSTATUS_SPP);

	// There's no need to clear the SSIP bit as in other handlers because interrupt bit is not set.
	return ret_val;
}


extern "C" void handleTimerTrap()
{
	static uint64 counter = 0;
	static uint64 seconds = 0;
	const uint64 volatile scause = RiscV::r_scause();
	if (scause == IRQ_TIMER)
	{
		if (counter == 10)
		{
			seconds++;
			print_string("Timer: ");
			print_int(seconds);
			print_string("s \n");
			counter = 0;
		}
		counter++;
		RiscV::mc_sip(RiscV::SIP_SSIE);
	}

}

extern "C" void handleKeyboardTrap()
{
	console_handler();
}