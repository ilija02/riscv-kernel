#include "../lib/hw.h"
#include "../h/_thread.hpp"
#include "../h/printing.hpp"
#include"../h/_sem.hpp"

extern sem_t semaphore;

static uint64 fibonacci(uint64 n) {
  if (n == 0 || n == 1)
  { return n; }
  if (n % 4 == 0) thread_dispatch();
  return fibonacci(n - 1) + fibonacci(n - 2);
}

void workerBodyA(void *) {
  uint8 i = 0;
  /*
  volatile uint64 dummy = 0;
  for (int i = 0; i < 1000000000; ++i) {
    dummy += i; // Dummy operation to prevent optimization
  }*/ //uncomment this code to test if the timer can interrupt
  for (; i < 3; i++)
  {
    printString("A: i=");
    printInt(i);
    printString("\n");
  }

  printString("A: yield\n");
  __asm__ ("li t1, 7");
  //_thread::yield();
  thread_dispatch();
  uint64 t1 = 0;
  __asm__ ("mv %[t1], t1" : [t1] "=r"(t1));

  printString("A: t1=");
  printInt(t1);
  printString("\n");

  uint64 result = fibonacci(23);
  printString("A: fibonaci=");
  printInt(result);
  printString("\n");

  for (; i < 6; i++)
  {
    printString("A: i=");
    printInt(i);
    printString("\n");
  }
  printString("Worker A done\n");
  thread_exit();
}

void workerBodyB(void *) {
  uint8 i = 10;
  for (; i < 13; i++)
  {
    printString("B: i=");
    printInt(i);
    printString("\n");
  }

  printString("B: yield\n");
  __asm__ ("li t1, 5");
  //_thread::yield();
  thread_dispatch();
  uint64 result = fibonacci(25); //75025
  printString("B: fibonaci=");
  printInt(result);
  printString("\n");

  for (; i < 16; i++)
  {
    printString("B: i=");
    printInt(i);
    printString("\n");
  }
  printString("Worker B done\n");
  //thread_exit();
}

void sem_worker_a(void* ){

  printString("sem_worker_a started. Signaling semaphore. \n");
  sem_signal(semaphore);
  printString("sem_worker_a signaled the semaphore.\n");

}

void sem_worker_b(void *){
  printString("sem_worker_b started. Waiting for semaphore.\n");
  if(semaphore) sem_wait(semaphore);
  printString("sem_worker_b passed the semaphore.\n");
}