#include "../lib/hw.h"
#include "../h/_thread.hpp"
#include "../h/print.hpp"
#include"../h/_sem.hpp"

extern sem_t semaphore;

static uint64 fibonacci(uint64 n)
{
	if (n == 0 || n == 1)
	{ return n; }
	if (n % 4 == 0) thread_dispatch();
	return fibonacci(n - 1) + fibonacci(n - 2);
}

void workerBodyA(void*)
{
	uint8 i = 0;
	/*
	volatile uint64 dummy = 0;
	for (int i = 0; i < 1000000000; ++i) {
	  dummy += i; // Dummy operation to prevent optimization
	}*/ //uncomment this code to test if the timer can interrupt
	for (; i < 3; i++)
	{
		print_string("A: i=");
		print_int(i);
		print_string("\n");
	}

	print_string("A: yield\n");
	__asm__ ("li t1, 7");
	//_thread::yield();
	thread_dispatch();
	uint64 t1 = 0;
	__asm__ ("mv %[t1], t1" : [t1] "=r"(t1));

	print_string("A: t1=");
	print_int(t1);
	print_string("\n");

	uint64 result = fibonacci(23);
	print_string("A: fibonaci=");
	print_int(result);
	print_string("\n");

	for (; i < 6; i++)
	{
		print_string("A: i=");
		print_int(i);
		print_string("\n");
	}
	print_string("Worker A done\n");
	thread_exit();
}

void workerBodyB(void*)
{
	uint8 i = 10;
	for (; i < 13; i++)
	{
		print_string("B: i=");
		print_int(i);
		print_string("\n");
	}

	print_string("B: yield\n");
	__asm__ ("li t1, 5");
	//_thread::yield();
	thread_dispatch();
	uint64 result = fibonacci(25); //75025
	print_string("B: fibonaci=");
	print_int(result);
	print_string("\n");

	for (; i < 16; i++)
	{
		print_string("B: i=");
		print_int(i);
		print_string("\n");
	}
	print_string("Worker B done\n");
	//thread_exit();
}

void sem_worker_a(void*)
{
	print_string("sem_worker_a signaling the semaphore. \n");
	sem_signal(semaphore);
	print_string("sem_worker_a signaled the semaphore.\n");
}

void sem_worker_b(void*)
{
	print_string("sem_worker_b started. Waiting for semaphore and running a loop.\n");
	volatile uint64 dummy = 0;
	for (int i = 0; i < 1000000000; ++i) {
		dummy += i; // Dummy operation to prevent optimization
	}
	if (semaphore) sem_wait(semaphore);
	print_string("sem_worker_b passed the semaphore.\n");
}