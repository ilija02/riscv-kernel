#include "../h/_thread.hpp"

_thread* _thread::running = nullptr;
bool _thread::is_user_mode_enabled = false;

_thread::_thread(_thread::Task task, void* argument, uint64* allocated_stack)
{
	if (task == nullptr) return;
	this->parent_thread = _thread::running;
	this->task = task;
	this->argument = argument;
	this->allocated_stack = allocated_stack;
	this->saved_context.sp =
		reinterpret_cast<uint64 >(&allocated_stack[DEFAULT_STACK_SIZE]); // since the stack grows downward, set the stack pointer to the top of allocated stackSch
	this->saved_context.ra = (uint64)_thread::thread_wrapper;
	this->state = ThreadState::READY;
	Scheduler::get().put_tcb(this);
}

void _thread::dispatch()
{
	_thread* old_running = _thread::running;
	if (!old_running->is_finished() && !old_running->is_suspended() && !old_running->is_blocked())
	{
		Scheduler::get().put_tcb(old_running);
		old_running->resume();
	}
	else
	{ // if the thread is finished and the parent is waiting for it, put the parent in the scheduler
		if (old_running->parent_thread && old_running->is_parent_waiting && !old_running->is_blocked())
		{
			old_running->is_parent_waiting = false;
			old_running->parent_thread->resume();
			Scheduler::get().put_tcb(old_running->parent_thread);
		} //else delete old_running // if automatic deletion of exited threads is required uncomment this line. Note that the explicit deletion is then forbidden.
	}
	_thread::running = Scheduler::get().get_tcb();
	_thread::running->state = ThreadState::RUNNING;
	context_switch(&old_running->saved_context, &_thread::running->saved_context);
}

int _thread::create_thread(_thread** handle, _thread::Task task, void* argument, uint64* allocatedStack)
{
	_thread* created_thread = new _thread(task, argument, allocatedStack);
	if (handle != nullptr) *handle = created_thread;
	if (created_thread == nullptr) return -1; // might be null as we're using our own memory allocator
	return 0;
}

void _thread::thread_wrapper()
{
	if (is_user_mode_enabled)
		_thread::switch_to_user_mode();
	_thread::running->task(_thread::running->argument);
	thread_exit();
}

int _thread::exit_thread()
{
	if (!_thread::running->is_running()) return -1;
	_thread::running->finish();
	_thread::dispatch();
	return 0;
}

int _thread::join()
{
	if (this->parent_thread != _thread::running) return -1;
	if (this->is_finished()) return 0;
	this->is_parent_waiting = true;
	this->parent_thread->suspend();
	_thread::yield();
	return 0;
}

bool _thread::switch_to_user_mode()
{
	RiscV::w_a0(SyscallID::SWITCH_TO_USER);
	RiscV::ecall();
	return true;
}

