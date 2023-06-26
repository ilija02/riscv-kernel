#include "../h/UnitTest.hpp"
#include "../h/RiscV.hpp"
#include "../h/printing.hpp"
extern "C" void trapHandler();

int main() {
  uint64 volatile
      handlerAddress = (uint64) &trapHandler | 0x01; //set the base address for interrupts to trap handler and
  //set mode to 1 (this enables vectored interrupts)
  RiscV::w_stvec(handlerAddress);

  MemoryAllocator &instance = MemoryAllocator::get();
  BlockHeader *free_head = (BlockHeader *) instance.get_free_head();
  printString("Available heap memory at start: ");
  printInt(free_head->size_in_bytes);
  printString("\n");

  UnitTest& TestRunner = UnitTest::get();
  TestRunner.test_synchronous_context_switching();
  //TestRunner.test_new_delete();
  //TestRunner.test_dequeue();


  TestRunner.test_thread_create();
  //RiscV::ms_sstatus(RiscV::SIP_SSIE);

  //RiscV::mc_sstatus(RiscV::SIP_SSIE);
  printString("Available heap memory at end: ");
  printInt(free_head->size_in_bytes);
  printString("\n");
  return 0;
}


