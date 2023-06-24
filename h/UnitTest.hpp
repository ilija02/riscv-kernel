#ifndef UNITTEST_HPP
#define UNITTEST_HPP
#include "../h/MemoryAllocator.hpp"
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
    static UnitTest& get ();
    static bool test_memory_allocator();
    static bool test_new_delete();
private:
    UnitTest();

};


#endif //UNITTEST_HPP