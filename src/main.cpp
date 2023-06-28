#include "../h/RiscV.hpp"
#include "../h/_thread.hpp"

extern "C" void trapHandler();

void print_free_memory(uint64 free_memory_at_start, uint64 free_memory_at_end)
{
	print_string("start free memory:  ");
	print_int(free_memory_at_start);
	print_string("\t");
	print_string("end free memory:  ");
	print_int(free_memory_at_end);
	print_string("\n");
}

int main()
{
	uint64 volatile
		handlerAddress = (uint64)&trapHandler | 0x01; //set the base address for interrupts to trap handler and
	//set mode to 1 (this enables vectored interrupts)
	RiscV::w_stvec(handlerAddress);
	_thread::enable_user_mode(); // all threads created after this call will be user threads.



	return 0;
}


