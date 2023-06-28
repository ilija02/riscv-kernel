#include "../h/UnitTest.hpp"

UnitTest& UnitTest::get()
{
	static UnitTest instance;
	return instance;
}

sem_t semaphore = nullptr;

bool UnitTest::test_memory_allocator()
{
	MemoryAllocator& instance = MemoryAllocator::get();
	BlockHeader* free_head = (BlockHeader*)instance.get_free_head();
	print_string("Free head size after initialization: ");
	uint64 old_size = free_head->size_in_bytes;
	print_int(old_size);
	print_string("\n");
	void* a1 = mem_alloc(1024);
	void* a2 = mem_alloc(1024);
	void* a3 = mem_alloc(1024);
	mem_free(a1);
	mem_free(a2);
	mem_free(a3);
	free_head = (BlockHeader*)instance.get_free_head();
	print_string("Free head size after freeing memory: ");
	uint64 new_size = free_head->size_in_bytes;
	print_int(new_size);
	print_string("\n");
	return old_size == new_size;
}

bool UnitTest::test_new_delete()
{

	MemoryAllocator& instance = MemoryAllocator::get();
	BlockHeader* free_head = (BlockHeader*)instance.get_free_head();
	print_string("Free head size after initialization: ");
	uint64 old_size = free_head->size_in_bytes;
	print_int(old_size);
	print_string("\n");

	TestStruct* t = new TestStruct();
	t->x = 5;
	t->y = 10;

	uint64* arr = new uint64[1000];
	for (int i = 0; i < 1000; i++)
	{
		arr[i] = i;
	}
	free_head = (BlockHeader*)instance.get_free_head();
	print_string("Free head size after allocating a struct and an array: ");
	uint64 allocated_size = free_head->size_in_bytes;
	print_int(allocated_size);
	print_string("\n");

	print_string("Number of bytes allocated: ");
	print_int(old_size - allocated_size);
	print_string("\n");
	delete t;
	delete[] arr;

	free_head = (BlockHeader*)instance.get_free_head();
	print_string("Free head size after freeing memory: ");
	uint64 new_size = free_head->size_in_bytes;
	print_int(new_size);
	print_string("\n");
	print_string("Test of new and delete completed.\n");

	return old_size == new_size;
}

bool UnitTest::test_dequeue()
{
	uint64 x = 5, y = 10, z = 15, w = 20;
	Dequeue<uint64> dequeue;

	//5 10
	dequeue.push_back(&x);
	dequeue.push_back(&y);
	uint64* tmp = dequeue.front();
	print_int(*tmp);
	tmp = dequeue.back();
	print_int(*tmp);
	//20 15 5 10
	dequeue.push_front(&z);
	dequeue.push_front(&w);
	tmp = dequeue.front();
	print_int(*tmp);
	tmp = dequeue.back();
	print_int(*tmp);

	dequeue.pop_front();
	tmp = dequeue.front();
	print_int(*tmp);

	dequeue.pop_back();
	tmp = dequeue.back();
	print_int(*tmp);
	dequeue.pop_front();
	dequeue.pop_back();
	dequeue.pop_back();
	dequeue.pop_back();
	return true;
}

bool UnitTest::test_synchronous_context_switching()
{
	print_string("------ Testing: test_synchronous_context_switching --------\n");
	thread_t threads[3];
	_thread::create_thread(&threads[0], nullptr, nullptr, nullptr);
	uint64* stack1 = new uint64[DEFAULT_STACK_SIZE];
	uint64* stack2 = new uint64[DEFAULT_STACK_SIZE];
	_thread::running = threads[0];
	_thread::create_thread(&threads[1], workerBodyA, nullptr, stack1);
	print_string("Thread A created\n");
	_thread::create_thread(&threads[2], workerBodyB, nullptr, stack2);
	print_string("Thread B created\n");
	/*while (!(threads[1]->is_finished() && threads[2]->is_finished())) {
	_thread::yield();
  }*/
	threads[1]->join();
	threads[2]->join();
	for (auto& thread : threads) delete thread;
	print_string("----- Finished test: test_synchronous_context_switching -----\n");
	return true;
}

bool UnitTest::test_thread_create()
{
	print_string("------Testing: test_thread_create --------\n");
	thread_t threads[3] = { nullptr };

	if (thread_create(&threads[0], nullptr, nullptr) < 0)
	{
		print_string("Failed creating main thread.");
		return false;
	}
	_thread::running = threads[0];
	if (thread_create(&threads[1], workerBodyA, nullptr) < 0)
	{
		print_string("Failed creating A thread.");
		return false;
	}
	print_string("Thread A created\n");

	if (thread_create(&threads[2], workerBodyB, nullptr) < 0)
	{
		print_string("Failed creating B thread.");
	}
	print_string("Thread B created.\n");

	/*while (!(threads[1]->is_finished() && threads[2]->is_finished() )) {
	  thread_dispatch();
	}*/
	thread_join(threads[1]);
	thread_join(threads[2]);
	for (auto& thread : threads) delete thread;
	print_string("----- Finished test: test_thread_create -----\n");
	return true;
}

bool UnitTest::test_semaphore()
{
	print_string("------Testing: test_semaphore --------\n");
	thread_t threads[3] = { nullptr };

	//_sem::create_semaphore(&semaphore, 0);
	sem_open(&semaphore, 0);
	if (thread_create(&threads[0], nullptr, nullptr) < 0)
	{
		print_string("Failed creating main thread.");
		return false;
	}
	_thread::running = threads[0];

	if (thread_create(&threads[1], sem_worker_b, nullptr) < 0)
	{
		print_string("Failed creating B thread.");
		return false;
	}
	print_string("Thread B created\n");
	if (thread_create(&threads[2], sem_worker_a, nullptr) < 0)
	{
		print_string("Failed creating A thread.");
		return false;
	}
	print_string("Thread A created\n");


//  while (!(threads[1]->is_finished() && threads[2]->is_finished())) {
//    thread_dispatch();
//  }
	/* doesn't work when using join, fix later*/
	thread_join(threads[1]);
	thread_join(threads[2]);

	for (auto& thread : threads) delete thread;
	print_string("----- Finished test: test_semaphore -----\n");

	sem_close(semaphore);
	return true;

}
