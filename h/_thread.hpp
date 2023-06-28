#ifndef BASE_TCB_HPP
#define BASE_TCB_HPP

#include "../lib/hw.h"
#include "../h/Scheduler.hpp"
#include "../h/RiscV.hpp"
#include "../h/print.hpp"

class _thread
{
	friend class _sem;
 public:
	// task is a pointer to a function that has no return value and takes one void* argument
	using Task = void (*)(void*);
	enum ThreadState
	{
		CREATED, READY, RUNNING, SUSPENDED, BLOCKED, FINISHED
	};

	static _thread* running;

	~_thread()
	{
		MemoryAllocator::get().mem_free(allocated_stack);
	}

	static int create_thread(_thread** handle, Task task, void* argument, uint64* allocatedStack);
	static int exit_thread();
	static void yield(); // implemented in _yield.S
	static void dispatch();
	static void enable_user_mode()
	{
		is_user_mode_enabled = true;
	}
	bool is_finished() const
	{
		return this->state == ThreadState::FINISHED;
	}
	bool is_running() const
	{
		return this->state == ThreadState::RUNNING;
	}
	int join();

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

 private:
	struct SavedContext
	{
		uint64 ra;
		uint64 sp;
	};
	// stack is allocated before calling _thread::CreateThread. It is allocated in the C api syscall.
	explicit _thread(_thread::Task task, void* argument, uint64* allocated_stack);

	static void context_switch(SavedContext* old_context, SavedContext* new_context); //implemented in _contextSwitch.s
	static void thread_wrapper();
	static bool switch_to_user_mode(); // calls the necessary assembly instructions to pop the set the appropriate privilege level

	void suspend()
	{
		this->state = ThreadState::SUSPENDED;
	};
	void resume()
	{
		this->state = ThreadState::READY;
	}
	void finish()
	{
		this->state = ThreadState::FINISHED;
	}
	void block()
	{
		this->state = ThreadState::BLOCKED;
	}
	bool is_suspended()
	{
		return this->state == ThreadState::SUSPENDED;
	}
	bool is_blocked()
	{
		return this->state == ThreadState::BLOCKED;
	}

	static bool is_user_mode_enabled;
	ThreadState state = ThreadState::CREATED;
	SavedContext saved_context = { 0, 0 };
	Task task = nullptr;
	uint64* allocated_stack = nullptr;
	_thread* parent_thread = nullptr;
	void* argument;
	bool is_parent_waiting = false;
	bool wait_returns_error =
		false; // if the semaphore closes the wait should return -1 , this attribute is used in sem_close and sem_wait

};

#endif //BASE_TCB_HPP
