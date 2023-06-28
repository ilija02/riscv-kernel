#include "../h/syscall_cpp.hpp"


void* operator new(size_t size)
{
	return mem_alloc(size);
}

void operator delete(void* chunk)
{
	mem_free(chunk);
}

// TODO: add logic so the run method can be run when it's overridden in the inherited class
class RunWrapper{
 public:
	static void runWrapper(void* arg) {
		Thread* thread = static_cast<Thread*>(arg);
		thread->run();
	}
};

Thread::Thread(void (* body)(void*), void* arg)
{
	this->body = body;
	this->arg = arg;
	this->myHandle = nullptr;
}

Thread::~Thread()
{
	delete myHandle;
}

int Thread::start()
{
	_thread::Task task_to_run =
		this->body== nullptr?RunWrapper::runWrapper:this->body;
	void* argument = this->body== nullptr?this:this->arg;
	return thread_create(&myHandle, task_to_run, argument);
}
void Thread::join()
{
	thread_join(myHandle);
}
void Thread::dispatch()
{
	thread_dispatch();
}

Thread::Thread()
{
	this->myHandle = nullptr;
	this->body = nullptr;
	this->arg  = nullptr;
}

Semaphore::Semaphore(unsigned int init)
{
	sem_open(&myHandle, init);
}

Semaphore::~Semaphore()
{
	sem_close(myHandle);
}

int Semaphore::wait()
{
	return sem_close(myHandle);
}

int Semaphore::signal()
{
	return sem_signal(this->myHandle);
}

char Console::getc()
{
	return __getc();
}
void Console::putc(char c)
{
	__putc(c);
}


