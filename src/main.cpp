#include "../h/RiscV.hpp"
#include "../h/_thread.hpp"

extern "C" void trapHandler();
extern void userMain();

void user_main_wrapper(void*){
	userMain();
}

int main()
{
	uint64 volatile
		handlerAddress = (uint64)&trapHandler | 0x01; //set the base address for interrupts to trap handler and
	//set mode to 1 (this enables vectored interrupts)
	RiscV::w_stvec(handlerAddress);
	_thread* kernel_thread, *user_main_thread;
	thread_create(&kernel_thread, nullptr, nullptr);
	_thread::running = kernel_thread;
	thread_create(&user_main_thread, user_main_wrapper, nullptr);

	_thread::enable_user_mode();

	//while(!user_main_thread->is_finished()) thread_dispatch();
	thread_join(user_main_thread);
	delete user_main_thread;
	delete kernel_thread;

	return 0;
}


