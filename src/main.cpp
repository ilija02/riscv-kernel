#include "../h/UnitTest.hpp"
#include "../h/RiscV.hpp"
#include "../h/printing.hpp"
extern "C" void trapHandler();


void print_free_memory(uint64 free_memory_at_start, uint64 free_memory_at_end ){
  printString("start free memory:  ");
  printInt(free_memory_at_start);
  printString("\t");
  printString("end free memory:  ");
  printInt(free_memory_at_end);
  printString("\n");
}
int main() {
  uint64 volatile
      handlerAddress = (uint64) &trapHandler | 0x01; //set the base address for interrupts to trap handler and
  //set mode to 1 (this enables vectored interrupts)
  RiscV::w_stvec(handlerAddress);
  //_thread::set_user_mode();
  //----------------------------------------
  MemoryAllocator &instance = MemoryAllocator::get();
  BlockHeader *free_head = (BlockHeader *) instance.get_free_head();
  size_t free_memory_at_start = free_head->size_in_bytes;
  //--------------------------------------

  UnitTest& TestRunner = UnitTest::get();

   TestRunner.test_synchronous_context_switching();
  //TestRunner.test_new_delete();
  //TestRunner.test_dequeue();
  TestRunner.test_thread_create();
  //TestRunner.test_thread_create();

  print_free_memory(free_memory_at_start, free_head->size_in_bytes);

  return 0;
}


