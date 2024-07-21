#include "producer.h"
#include "buffer.h" 
#include "globals.h"
#include <sys/syscall.h>
#include <iostream>
#include <unistd.h>



void *producer(void *param) {
    thread_args* args = (thread_args*) param;
    buffer_t *buffer = args->buffer;
    buffer_item item;
    pid_t tid = syscall(SYS_gettid);

    pthread_barrier_wait(args->barrier);


    while (!shutdown_flag.load()) {
        sleep(rand() % 5); 
        item.value = rand();


        if (insert_item(buffer, item)) {
            if (!shutdown_flag.load()) {
                std::cerr<< "Producer: Thread ID (TID) = " << tid << " | Error inserting item " << item.value << " (buffer full)" << std::endl;
            } 
        }
    }


    pthread_exit(0);
}
