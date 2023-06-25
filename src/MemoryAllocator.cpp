#include "../h/MemoryAllocator.hpp"

void MemoryAllocator::initialize() {
  this->end_address = (size_t) (HEAP_END_ADDR);
  this->start_address = (size_t) (HEAP_START_ADDR);
  if (this->start_address % MEM_BLOCK_SIZE != 0)
    // round up the start address to the next block
    this->start_address = this->start_address + MEM_BLOCK_SIZE -
        (this->start_address % MEM_BLOCK_SIZE);

  if (this->end_address < this->start_address ||
      (this->end_address - start_address) < sizeof(BlockHeader))
  {
    // error check if the heap boundaries are correct and if the first block
    // can
    this->free_head = nullptr;
    return;
  }
  this->free_head = reinterpret_cast<BlockHeader *>(
      this->start_address); // store the header information at the beggining
  // of the block
  this->free_head->next = nullptr;
  this->free_head->size_in_bytes =
      this->end_address - this->start_address - sizeof(BlockHeader);
}

bool MemoryAllocator::try_to_merge(BlockHeader *header) {
  if (header == nullptr || header->next == nullptr)
    return false;
  if ((char *) header + header->size_in_bytes + sizeof(BlockHeader) !=
      (char *) header->next)
    return false;
  header->size_in_bytes += header->next->size_in_bytes + sizeof(BlockHeader);
  header->next = header->next->next; // join chunks
  return true;
}

MemoryAllocator &MemoryAllocator::get() {
  static MemoryAllocator instance;
  return instance;
}

void *MemoryAllocator::mem_alloc(size_t number_of_bytes) {
  if (!this->free_head || number_of_bytes <= 0)
    return nullptr;

  // round up the number of blocks
  size_t number_of_blocks =
      (number_of_bytes + MEM_BLOCK_SIZE - 1) / MEM_BLOCK_SIZE;
  size_t rounded_number_of_bytes = number_of_blocks * MEM_BLOCK_SIZE;

  BlockHeader *current = this->free_head, *previous = nullptr;
  // find a free chunk of memory using first-fit algorithm
  while (current != nullptr)
  {
    if (current->size_in_bytes >= rounded_number_of_bytes)
      break;
    previous = current;
    current = current->next;
  }
  if (!current)
    return nullptr; // didn't find a chunk with enough free memory

  size_t remaining_size = current->size_in_bytes - rounded_number_of_bytes;
  if (remaining_size > sizeof(BlockHeader) + MEM_BLOCK_SIZE)
  {
    current->size_in_bytes = rounded_number_of_bytes;
    BlockHeader *new_header = reinterpret_cast<BlockHeader *>(
        (char *) current + sizeof(BlockHeader) + rounded_number_of_bytes);

    if (previous)
      previous->next = new_header;
    else
      this->free_head = new_header;
    new_header->next = current->next;
    new_header->size_in_bytes = remaining_size - sizeof(BlockHeader);
  }
  else
  {
    // Available memory chunk couldn't be fragmented, use the whole chunk
    if (previous)
      previous->next = current->next;
    else
      this->free_head = current->next;
  }
  current->next = nullptr;
  return (char *) current + sizeof(BlockHeader);
}

long MemoryAllocator::mem_free(void *allocated_chunk) {
  if (allocated_chunk == nullptr)
    return -1;
  if ((size_t) allocated_chunk < this->start_address ||
      (size_t) allocated_chunk >= end_address)
    return -2; // error: given address is not in the bounds of available
  // space for allocation

  BlockHeader *header = reinterpret_cast<BlockHeader *>(
      (char *) allocated_chunk -
          sizeof(BlockHeader)); // get the header for the allocated data chunk
  BlockHeader *current = this->free_head > header
                         ? nullptr
                         : this->free_head; // list of headers is ordered
  // by address, so if the
  // header has a smaller address it should come first

  while (current && current->next != nullptr && current->next < header)
    current = current->next;
  // current will point to the previous chunk or the chunk itself / null, and
  // current->next will be the next chunk after the header
  if (current == nullptr)
  {
    // there were no free chunks
    header->next = this->free_head; // could be nullptr/free_head
    this->free_head = header;
  }
  else
  {
    header->next = current->next;
    current->next = header;
  }

  try_to_merge(header);  // try to merge with the next segment
  try_to_merge(current); // the to merge previous with header
  return 0;
}
