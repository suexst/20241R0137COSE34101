#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <time.h>
#include "scheduler.h"

#define MAX_BURST_TIME 10

time_t init_time;
int gantt_idx = 0;

// process->priority - 숫자가 작을수록 우선순위 높음

void* process_thread(void* arg) {
    Process* process = (Process*)arg;
    printf("[pid:%d] Executing Process time : %ld\n", process->process_id, time(NULL) - init_time);
    sleep(process->cpu_burst_time);

    process->turn_around_time = time(NULL) - init_time;

    printf("[pid:%d] Finished Process time : %d\n", process->process_id, process->turn_around_time);

    pthread_exit(NULL);
}

void np_priority_add_process(Scheduler* scheduler, Process* process) {
    pthread_mutex_lock(&scheduler->lock);
    priority_enqueue(scheduler->ready_queue, process);
    pthread_mutex_unlock(&scheduler->lock);

    //printf("[pid:%d] Add process time : %ld\n", process->process_id, time(NULL) - scheduler->init_time);
}

Process* np_priority_get_next_process(Scheduler* scheduler) {
    pthread_mutex_lock(&scheduler->lock);
    Process* process = dequeue(scheduler->ready_queue);
    pthread_mutex_unlock(&scheduler->lock);
    return process;
}

void np_priority_schedule(Scheduler* scheduler, int num_thread) {
    Process* process;
    pthread_t tid;
    int count = 0;
    int total_response_time = 0;
    int total_turn_around_time = 0;
    init_time = scheduler->init_time;
    while(count < num_thread) {
        if ((process = np_priority_get_next_process(scheduler)) != NULL) {
            if (process->response_time == 0) {
                process->response_time = time(NULL) - scheduler->init_time;
            }
            if (pthread_create(&tid, NULL, process_thread, process) != 0) {
                fprintf(stderr, "Failed to create thread for process [pid:%d]\n", process->process_id);
                free(process);
                continue;
            }
            pthread_join(tid, NULL);

            for(int i=0; i<process->cpu_burst_time; i++) {
                scheduler->gantt_chart[gantt_idx++] = process->process_id;
            }

            total_response_time += process->response_time;
            total_turn_around_time += process->turn_around_time;

            scheduler->gantt_chart_size = time(NULL) - init_time;

            free(process);
            count++;
        }
    }
    scheduler->average_response_time = (double) total_response_time / count;
    scheduler->average_turnaround_time = (double) total_turn_around_time / count;
}

Scheduler* create_np_priority_scheduler(int num_thread) {
    Scheduler* scheduler = (Scheduler*)malloc(sizeof(Scheduler));
    scheduler->gantt_chart = (int*)malloc(num_thread * MAX_BURST_TIME * sizeof(int));
    scheduler->ready_queue = createQueue();
    scheduler->add_process = np_priority_add_process;
    scheduler->get_next_process = np_priority_get_next_process;
    scheduler->schedule = np_priority_schedule;
    scheduler->init_time = time(NULL);
    scheduler->average_response_time = 0;
    scheduler->average_turnaround_time = 0;
    return scheduler;
}