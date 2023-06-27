#ifndef UNITTEST_HPP
#define UNITTEST_HPP
#include "../h/MemoryAllocator.hpp"
#include "../h/Dequeue.hpp"
#include "../h/workers.hpp"
#include "../h/_thread.hpp"

extern "C" void trapHandler();
struct BlockHeader {
  BlockHeader *next;
  size_t size_in_bytes;
};

struct TestStruct {
  uint64 x;
  uint64 y;
};

class UnitTest {
public:
  static UnitTest &get();
  static bool test_memory_allocator();
  static bool test_new_delete();
  static bool test_dequeue();
  static bool test_synchronous_context_switching();
  static bool test_thread_create();
private:
  UnitTest();

};

#endif //UNITTEST_HPP
