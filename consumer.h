#ifndef CONSUMER_H
#define CONSUMER_H

#include "buffer.h"
#include "globals.h"

/**
 * @file consumer.h
 * @brief Header file for the consumer thread functionality in the producer-consumer problem.
 */

/**
 * @brief The consumer thread function.
 *
 * This function continuously attempts to remove items from the buffer and consume them
 * (in this case, by printing them to the console/file). It waits for the 'full' semaphore
 * to be signaled before attempting to remove an item, ensuring that the buffer is not empty.
 * It also checks the `shutdown_flag` to exit gracefully when the program is terminated.
 * 
 * @param param A void pointer to the thread arguments (`thread_args` struct), containing:
 *   - `buffer_t* buffer`: A pointer to the shared buffer.
 *   - `pthread_barrier_t* barrier`: A pointer to the barrier used for synchronization.
 * @return NULL (The function doesn't return a meaningful value).
 */
void* consumer(void* param);

#endif // CONSUMER_H

