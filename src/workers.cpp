#include "../lib/hw.h"
#include "../h/TCB.hpp"
#include "../h/printing.hpp"

static uint64 fibonacci(uint64 n) {
  if (n == 0 || n == 1)
  { return n; }
  if (n % 4 == 0) TCB::yield();
  return fibonacci(n - 1) + fibonacci(n - 2);
}

void workerBodyA(void *) {
  uint8 i = 0;
  for (; i < 3; i++)
  {
    printString("A: i=");
    printInt(i);
    printString("\n");
  }

  printString("A: yield\n");
  __asm__ ("li t1, 7");
  TCB::yield();

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

  TCB::running->finish();
  TCB::yield();
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
  TCB::yield();

  uint64 result = fibonacci(30);
  printString("B: fibonaci=");
  printInt(result);
  printString("\n");

  for (; i < 16; i++)
  {
    printString("B: i=");
    printInt(i);
    printString("\n");
  }

  TCB::running->finish();
  TCB::yield();
}