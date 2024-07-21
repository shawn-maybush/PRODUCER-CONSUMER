/**
 * @file buffer.h
 * @brief Header file for the bounded buffer implementation used in the producer-consumer problem.
 */

#ifndef BUFFER_H
#define BUFFER_H

#include <pthread.h>
#include <semaphore.h>

#define BUFFER_SIZE 5  /**< The maximum number of items the buffer can hold. */

/**
 * @struct buffer_item
 * @brief Represents a single item in the buffer, along with its production timestamp.
 */
struct buffer_item {
    int value;             /**< The integer value of the item. */
    struct timespec timestamp;  /**< The time the item was produced. */
};

/**
 * @struct buffer_t
 * @brief Represents the bounded buffer data structure.
 */
struct buffer_t {
    buffer_item buffer[BUFFER_SIZE];     /**< Array to store the buffer items. */
    int count;                          /**< Number of items currently in the buffer. */
    int head;                           /**< Index of the next item to be removed (consumed). */
    int tail;                           /**< Index of the next available slot for insertion (production). */
    int items_produced;                 /**< Total number of items produced. */
    int items_consumed;                 /**< Total number of items consumed. */
    double total_turnaround_time;       /**< Total turnaround time for all consumed items. */

    sem_t empty;                       /**< Semaphore indicating the number of empty slots in the buffer. */
    sem_t full;                        /**< Semaphore indicating the number of full slots in the buffer. */
    pthread_mutex_t mutex;              /**< Mutex for synchronizing access to the buffer. */
};

/**
 * @brief Initializes the buffer structure.
 * @param buffer A pointer to the buffer_t struct to be initialized.
 */
void buffer_init(buffer_t* buffer);

/**
 * @brief Inserts an item into the buffer.
 * @param buffer A pointer to the buffer_t struct.
 * @param item The item to be inserted.
 * @return 0 if the item was inserted successfully, -1 if the buffer is full.
 */
int insert_item(buffer_t* buffer, buffer_item item);

/**
 * @brief Removes an item from the buffer.
 * @param buffer A pointer to the buffer_t struct.
 * @param item A pointer to a buffer_item struct where the removed item will be stored.
 * @return 0 if the item was removed successfully, -1 if the buffer is empty.
 */
int remove_item(buffer_t* buffer, buffer_item* item);

#endif // BUFFER_H
