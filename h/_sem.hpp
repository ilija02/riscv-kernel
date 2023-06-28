#ifndef _SEM_HPP_
#define _SEM_HPP_
#include "Dequeue.hpp"
#include "_thread.hpp"
// factory pattern
class _sem
{
 public:
	static uint64 create_semaphore(_sem** handle, uint32 initial_value = 1);
	void close();
	int wait();
	int signal();
	long value() const
	{
		return val;
	};

	// redirect memory allocation to the MemoryAllocator kernel class
	void* operator new(size_t size)
	{
		return MemoryAllocator::get().mem_alloc(size);
	}
	void operator delete(void* chunk)
	{
		MemoryAllocator::get().mem_free(chunk);
	}
	void* operator new[](size_t size)
	{
		return MemoryAllocator::get().mem_alloc(size);
	}
	void operator delete[](void* chunk)
	{
		MemoryAllocator::get().mem_free(chunk);
	}

 protected:
	void block();
	void unblock();

 private:
	//initial value must be >= 0
	explicit _sem(uint32 initial_value) : val(initial_value)
	{
	}
	int val;
	bool is_closed = false;
	Dequeue<_thread> blocked_threads;
};

#endif //_SEM_HPP_
