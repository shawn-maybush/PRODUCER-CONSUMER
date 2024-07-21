#include "buffer.h"
#include "globals.h"
#include <unistd.h>
#include <sys/syscall.h>
#include <iostream>
#include <tuple>

void buffer_init(buffer_t* buffer) {
    buffer->count = 0;
    buffer->head = 0;
    buffer->tail = 0;
    buffer->items_consumed = 0;
    buffer->items_produced = 0;
    buffer->total_turnaround_time = 0.0;

    sem_init(&buffer->empty, 0, BUFFER_SIZE);
    sem_init(&buffer->full, 0, 0);
    pthread_mutex_init(&buffer->mutex, NULL);
}

int insert_item(buffer_t* buffer, buffer_item item) {

    clock_gettime(CLOCK_MONOTONIC, &item.timestamp);
    sem_wait(&buffer->empty);
    pthread_mutex_lock(&buffer->mutex);
    pid_t tid = syscall(SYS_gettid);

    if (!shutdown_flag.load()){

        buffer->buffer[buffer->tail] = item;
        buffer->tail = (buffer->tail + 1) % BUFFER_SIZE;
        buffer->count++;
        buffer->items_produced++;
        std::cout << "Producer: Thread ID (TID) = " << tid << " | Produced " << item.value <<std::endl;
        pthread_mutex_unlock(&buffer->mutex);
        sem_post(&buffer->full);
        return 0;
    } else {

        std::cout << "Producer: Thread ID (TID) = " << tid << " | Interupted" << std::endl;

        pthread_mutex_unlock(&buffer->mutex);
        sem_post(&buffer->full);
        return 1;

    }
}

int remove_item(buffer_t* buffer, buffer_item* item) {
    struct timespec removeTime;
    sem_wait(&buffer->full);
    pthread_mutex_lock(&buffer->mutex);
    pid_t tid = syscall(SYS_gettid);

    if (!shutdown_flag.load()){

        *item = buffer->buffer[buffer->head];
        buffer->head = (buffer->head + 1) % BUFFER_SIZE;
        buffer->count--;
        buffer->items_consumed++;
        clock_gettime(CLOCK_MONOTONIC, &removeTime);
        double turnaround = (removeTime.tv_sec - item->timestamp.tv_sec) + 
                            (removeTime.tv_nsec - item->timestamp.tv_nsec) / 1e9;
        buffer->total_turnaround_time += turnaround;
        std::cout << "Consumer: Thread ID (TID) = " << tid << " | Consumed " << item->value <<std::endl;
        
        pthread_mutex_unlock(&buffer->mutex);
        sem_post(&buffer->empty);
        return 0;
    } else {
        
        std::cout << "Consumer: Thread ID (TID) = " << tid << " | Interupted" << std::endl;
        pthread_mutex_unlock(&buffer->mutex);
        sem_post(&buffer->empty);
        return 1;
        
    }
}