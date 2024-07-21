#ifndef GLOBALS_H
#define GLOBALS_H

#include <atomic>

/**
 * @file globals.h
 * @brief Header file for global variables and data structures used in the producer-consumer project.
 */

/**
 * @brief Forward declaration of the buffer_t struct.
 *
 * This is a forward declaration of the buffer_t struct, which is defined in buffer.h.
 * It allows us to use pointers to buffer_t in this header file without including the full definition.
 */
struct buffer_t;

/**
 * @brief Atomic flag to signal shutdown to all threads.
 *
 * This flag is set to true by the main thread when it's time for the program to terminate.
 * The producer and consumer threads check this flag periodically and exit gracefully when it's set.
 */
extern std::atomic_bool shutdown_flag;

/**
 * @brief Barrier for thread synchronization.
 *
 * This barrier is used to synchronize the startup and shutdown of the producer and consumer threads.
 * It ensures that all threads have reached a certain point before allowing them to proceed.
 */
extern pthread_barrier_t barrier;

/**
 * @struct thread_args
 * @brief Structure to hold arguments passed to thread functions.
 */
struct thread_args {
    buffer_t* buffer;        /**< Pointer to the shared buffer. */
    pthread_barrier_t* barrier; /**< Pointer to the synchronization barrier. */
};

#endif // GLOBALS_H
