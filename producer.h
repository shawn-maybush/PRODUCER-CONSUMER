#ifndef PRODUCER_H
#define PRODUCER_H

#include "buffer.h"
#include "globals.h"

/**
 * @file producer.h
 * @brief Header file for the producer thread functionality in the producer-consumer problem.
 */

/**
 * @brief The producer thread function.
 *
 * This function continuously generates random integer values and attempts to insert them into the shared buffer.
 * It waits for the 'empty' semaphore to be signaled before attempting to insert an item, ensuring that the buffer is not full.
 * It also checks the `shutdown_flag` to exit gracefully when the program is terminated.
 * 
 * @param param A void pointer to the thread arguments (`thread_args` struct), containing:
 *   - `buffer_t* buffer`: A pointer to the shared buffer.
 *   - `pthread_barrier_t* barrier`: A pointer to the barrier used for synchronization.
 * @return NULL (The function doesn't return a meaningful value).
 */
void* producer(void* param);

#endif // PRODUCER_H
