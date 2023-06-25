#include "../h/UnitTest.hpp"
#include "../h/printing.hpp"
#include "../h/TCB.hpp"
#include "../h/workers.hpp"
extern "C" void trapHandler();

void main() {
  uint64 volatile
      handlerAddress = (uint64) &trapHandler | 0x01; //set the base address for interrupts to trap handler and
  //set mode to 1 (this enables vectored interrupts)
  RiscV::w_stvec(handlerAddress);

  //UnitTest& TestRunner = UnitTest::get();
  //TestRunner.test_new_delete();
  //TestRunner.test_dequeue();

  TCB *threads[3];
  threads[0] = TCB::create_thread(nullptr, nullptr, nullptr);
  uint64 *stack1 = new uint64[DEFAULT_STACK_SIZE];
  uint64 *stack2 = new uint64[DEFAULT_STACK_SIZE];
  TCB::running = threads[0];
  threads[1] = TCB::create_thread(workerBodyA, nullptr, stack1);
  printString("Thread A created\n");
  threads[2] = TCB::create_thread(workerBodyB, nullptr, stack2);
  printString("Thread B created\n");
  while (!(threads[1]->is_finished() && threads[2]->is_finished())) {
    TCB::yield();
  }

  for (auto &thread: threads) delete thread;
  printString("Finished\n");

}


