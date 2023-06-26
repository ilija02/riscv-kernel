#ifndef SYSCALL_C_H
#define SYSCALL_C_H
#include "../lib/hw.h"
#include "../h/syscall_id.hpp"
#include "../h/RiscV.hpp"
#include "../h/TCB.hpp"
#ifdef __cplusplus
extern "C" {
#endif

/**
 * Allocates at least size bytes of memory rounded up to blocks of size MEM_BLOCK_SIZE.
 * @param size minimum number of bytes to allocate
 * @return If allocation is successful returns a pointer to the allocated chunk, else returns null
 */
void *mem_alloc(size_t size);

/**
 * Frees a chunk of memory previously allocated by mem_alloc
 * @return 0 if operations was successful or a value less than 0 if an error occurred
 */
int mem_free(void *);

class _thread;
typedef TCB *thread_t;
/**
 *Starts a thread which runs a function start_routine with argument arg
_thread).
 * @param handle thread handle
 * @param start_routine routine to start
 * @param arg argument which is passed to start_routine
 * @return  If successful , handle gets the value of the newly created thread and the function returns 0, else value less than 0
 */
int thread_create(
    thread_t *handle,
    void(*start_routine)(void *),
    void *arg
);

/**
 * Shuts down the current thread
 * @return If successful 0, else value less than 0
 */
int thread_exit();

/**
 * Potentially takes away the processor from the current thread and gives it to some other (or even same) thread.
 */
void thread_dispatch();

/**
 * Suspends the current thread until the thread with a given handle finishes
 * @param handle thread to wait for
 */
void thread_join(
    thread_t handle
);

class _sem;
typedef _sem *sem_t;
/**
 * Creates a new semaphore
 * @param handle handle of the newly created semaphore
 * @param init initial value of the semaphore
 * @return If successful 0, else value less than 0
 */
int sem_open(
    sem_t *handle,
    unsigned init
);

/**
 * Frees the semaphore with the given handle. All threads waiting on the given semaphore are unblocked and their
 * corresponding wait methods return and error code.
 * @param handle semaphore handle
 * @return If successful 0, else value less than 0
 */
int sem_close(sem_t handle);

/**
 * Wait on the given semaphore
 * @param id semaphore to wait on
 * @return If successful 0, else value less than 0 . Error is also returned if the semaphore is closed while the thread
 * is still waiting.
 */
int sem_wait(sem_t id);

/**
 * signals the current semaphore
 * @param id semaphore to signal
 * @return If successful 0, else value less than 0
 */
int sem_signal(sem_t id);

typedef unsigned long time_t;
/**
 * Sleeps the current thread for a given amount of time
 * @param time time to sleep for in internal timer units
 * @return If successful 0, else value less than 0
 */
int time_sleep(time_t time);

const int EOF = -1;
/**
 * Reads one character from the console buffer. If the console buffer empty the current threads is suspended until a
 * character is available.
 * @return If successful returns the character that has been read, else returns EOF
 */
char getc();

/**
 * Prints the given character to the console.
 * @param c character to print
 */
void putc(char c);

#ifdef __cplusplus
}
#endif
#endif //SYSCALL_C_H
