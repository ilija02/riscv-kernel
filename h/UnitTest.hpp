#ifndef UNITTEST_HPP
#define UNITTEST_HPP
#include "../h/MemoryAllocator.hpp"
extern "C" void trapHandler();
struct BlockHeader {
    BlockHeader *next;
    size_t size_in_bytes;
};

class UnitTest {
public:
    static UnitTest& get ();
    static bool test_memory_allocator();
private:
    UnitTest();

};


#endif //UNITTEST_HPP
