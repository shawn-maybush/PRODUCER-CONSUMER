#ifndef BUFFER_H
#define BUFFER_H

#include <pthread.h>
#include <semaphore.h>


#define BUFFER_SIZE 5

struct buffer_item {
    int value;
    struct timespec timestamp; 
};

struct buffer_t {
    buffer_item buffer[BUFFER_SIZE];
    int count;
    int head;
    int tail;
    int items_produced;
    int items_consumed;
    double total_turnaround_time;


    sem_t empty;
    sem_t full;
    pthread_mutex_t mutex;
};


void buffer_init(buffer_t* buffer);
int insert_item(buffer_t* buffer, buffer_item item);
int remove_item(buffer_t* buffer, buffer_item* item);

#endif