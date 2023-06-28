#ifndef UNITTEST_HPP
#define UNITTEST_HPP
#include "../h/MemoryAllocator.hpp"
#include "../h/Dequeue.hpp"
#include "../h/workers.hpp"
#include "../h/_thread.hpp"
#include  "../h/_sem.hpp"
extern "C" void trapHandler();
struct BlockHeader
{
	BlockHeader* next;
	size_t size_in_bytes;
};

struct TestStruct
{
	uint64 x;
	uint64 y;
};

class UnitTest
{
 public:
	static UnitTest& get();
	bool test_memory_allocator();
	bool test_new_delete();
	bool test_dequeue();
	bool test_synchronous_context_switching();
	bool test_thread_create();
	bool test_semaphore();
 private:
	UnitTest() = default;

};

#endif //UNITTEST_HPP
