#include <pthread.h>
#include <unistd.h>
#include <vector>
#include <time.h>
#include <fstream>
#include <iostream>
#include <iomanip>
#include <signal.h>
#include "buffer.h"
#include "producer.h"
#include "consumer.h"
#include "globals.h"

std::atomic_bool shutdown_flag(false);

pthread_barrier_t barrier;

int main(int argc, char *argv[])
{
    if (argc != 4) {
        std::cerr << "Usage: " << argv[0] << "sleep_time num_producers num_consumers" << std::endl;
        return 1;
    }

    std::stringstream filename;
    filename << "sleep_time_" << argv[1] << "_producers_" << argv[2] << "_consumers_" << argv[3] << "_output.txt";

    std::ofstream outfile;
    outfile.open(filename.str(), std::ios_base::out | std::ios_base::trunc);

    if (!outfile.is_open()) {
        fprintf(stderr, "Error opening output file!\n");
        return 1;
    }

    std::streambuf *coutbuf = std::cout.rdbuf();
    std::cout.rdbuf(outfile.rdbuf());

    struct timespec start, end;

    

    clock_gettime(CLOCK_MONOTONIC, &start);

    int sleep_time = atoi(argv[1]);
    int num_producers = atoi(argv[2]);
    int num_consumers = atoi(argv[3]);

    pthread_barrier_init(&barrier, NULL, num_producers + num_consumers + 1);
    buffer_t buffer;
    buffer_init(&buffer);

    thread_args args = {&buffer, &barrier};

    std::vector<pthread_t> producers(num_producers);
    std::vector<pthread_t> consumers(num_consumers);

    for (int i = 0; i < num_producers; ++i)
    {
        pthread_create(&producers[i], NULL, producer, &args);
    }

    for (int i = 0; i < num_consumers; ++i)
    {
        pthread_create(&consumers[i], NULL, consumer, &args);
    }

    pthread_barrier_wait(&barrier);

    sleep(sleep_time);

    clock_gettime(CLOCK_MONOTONIC, &end);

    
    shutdown_flag.store(true);

    for(int i = 0; i < num_producers; ++i){
        sem_post(&buffer.empty);
    }
    for(int i = 0; i < num_consumers; ++i){
        sem_post(&buffer.full);
    }

    for (int i = 0; i < num_producers; ++i) {
        pthread_join(producers[i], NULL); 
    }
    for (int i = 0; i < num_consumers; ++i) {
        pthread_join(consumers[i], NULL);
    }

    sem_destroy(&buffer.empty);
    sem_destroy(&buffer.full);
    pthread_mutex_destroy(&buffer.mutex);
    pthread_barrier_destroy(&barrier);

    

    double total_time = (end.tv_sec - start.tv_sec) + 
                             (end.tv_nsec - start.tv_nsec) / 1e9;
    double production_rate = buffer.items_produced / total_time; 
    double consumption_rate = buffer.items_consumed / total_time;
    double avg_turn_around_time = buffer.total_turnaround_time / buffer.items_consumed;


    std::cout << std::fixed << std::setprecision(2);

    std::cout<< "\nSleep Time: "<< sleep_time << " | Producers: " << num_producers << " | Consumers: " << num_consumers << std::endl;
    std::cout << "Total Time: " << total_time << " sec" << std::endl;
    std::cout << "Total Items Produced: " << buffer.items_produced << std::endl;
    std::cout << "Total Items Consumed: " << buffer.items_consumed << std::endl;
    std::cout << "Production rate: " << production_rate << " items/sec" << std::endl;
    std::cout << "Consumption rate: " << consumption_rate << " items/sec" << std::endl;

    std::cout << std::fixed << std::setprecision(4);
    std::cout << "Average Item Turnaround: " << avg_turn_around_time << " sec" << std::endl;

    std::cout.rdbuf(coutbuf);
    outfile.close();

    return 0;    
}
