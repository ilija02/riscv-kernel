#include "../h/UnitTest.hpp"
#include "../h/RiscV.hpp"
extern "C" void trapHandler();

int main() {
  uint64 volatile
      handlerAddress = (uint64) &trapHandler | 0x01; //set the base address for interrupts to trap handler and
  //set mode to 1 (this enables vectored interrupts)
  RiscV::w_stvec(handlerAddress);

  UnitTest& TestRunner = UnitTest::get();
  TestRunner.test_new_delete();
  //TestRunner.test_dequeue();
  TestRunner.test_synchronous_context_switching();

  //TestRunner.test_thread_create();
  //RiscV::ms_sstatus(RiscV::SIP_SSIE);

  //RiscV::mc_sstatus(RiscV::SIP_SSIE);
  return 0;
}


