#ifndef GLOBALS_H
#define GLOBALS_H
#include <atomic>

struct buffer_t;
extern std::atomic_bool shutdown_flag;
extern pthread_barrier_t barrier;
struct thread_args {
    buffer_t* buffer;
    pthread_barrier_t* barrier; 
};


#endif
