#ifndef WORKERS_HPP
#define WORKERS_HPP

extern void workerBodyA(void *);

extern void workerBodyB(void *);

void sem_worker_a(void* );
void sem_worker_b(void* );
#endif //WORKERS_HPP
