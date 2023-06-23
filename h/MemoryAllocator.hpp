#ifndef MEMORY_ALLOCATOR_HPP
#define MEMORY_ALLOCATOR_HPP

#include "../lib/hw.h"

class MemoryAllocator {
    // singleton class used for managing memory allocations
public:
    MemoryAllocator(const MemoryAllocator &) =
    delete; // delete the copy constructor
    MemoryAllocator &operator=(const MemoryAllocator &) =
    delete;                    // delete the copy assignment operator
    static MemoryAllocator &get(); // returns the MemoryAllocator instance
    void *mem_alloc(
            size_t number_of_bytes); // allocates a new chunk of memory                                      // using the first fit algorithm
    long mem_free(void *allocated_chunk); // frees the chunk of memory previously allocated with mem_alloc
private:
    struct BlockHeader {
        BlockHeader *next;
        size_t size_in_bytes;
    };

    MemoryAllocator() { this->initialize(); };

    ~MemoryAllocator() = default;

    void initialize();

    bool try_to_merge(BlockHeader *header); // tries to merge the current free block with the next one

    BlockHeader *free_head;
    size_t start_address;
    size_t end_address;
};

#endif