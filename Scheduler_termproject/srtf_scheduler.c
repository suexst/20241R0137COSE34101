#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <time.h>
#include "scheduler.h"

#define MAX_BURST_TIME 10

time_t init_time;
int gantt_idx = 0;
int count = 0;
int total_response_time = 0;
int total_turn_around_time = 0;
Scheduler* global_scheduler;

void* process_thread(void* arg) {
    Process* process = (Process*)arg;
    //time_t start = time(NULL);
    printf("[pid:%d] Executing Process time : %ld\n", process->process_id, time(NULL) - init_time);
    while(process->cpu_burst_time > 0) {
        if((front(global_scheduler->ready_queue) != NULL) && front(global_scheduler->ready_queue)->cpu_burst_time < process->cpu_burst_time) {
            sjf_enqueue(global_scheduler->ready_queue, process);
            printf("[pid:%d] Process Interrupted\n", process->process_id);
            count--;
            pthread_exit(NULL);
        }
        sleep(1);
        process->cpu_burst_time--;
        global_scheduler->gantt_chart[gantt_idx++] = process->process_id;
    }

    process->turn_around_time = time(NULL) - init_time;

    total_response_time += process->response_time;
    total_turn_around_time += process->turn_around_time;

    printf("[pid:%d] Finished Process time : %d\n", process->process_id, process->turn_around_time);

    free(process);
    pthread_exit(NULL);
}

void srtf_add_process(Scheduler* scheduler, Process* process) {
    pthread_mutex_lock(&scheduler->lock);
    sjf_enqueue(scheduler->ready_queue, process);
    pthread_mutex_unlock(&scheduler->lock);
}

Process* srtf_get_next_process(Scheduler* scheduler) {
    pthread_mutex_lock(&scheduler->lock);
    Process* process = dequeue(scheduler->ready_queue);
    pthread_mutex_unlock(&scheduler->lock);
    return process;
}

void srtf_schedule(Scheduler* scheduler, int num_thread) {
    global_scheduler = scheduler;
    Process* process;
    pthread_t tid;
    total_response_time = 0;
    total_turn_around_time = 0;

    init_time = scheduler->init_time;
    while(count < num_thread) {
        if ((process = srtf_get_next_process(scheduler)) != NULL) {
            if (process->response_time == 0) {
                process->response_time = time(NULL) - scheduler->init_time;
            }
            if (pthread_create(&tid, NULL, process_thread, process) != 0) {
                fprintf(stderr, "Failed to create thread for process [pid:%d]\n", process->process_id);
                free(process);
                continue;
            }
            pthread_join(tid, NULL);

            scheduler->gantt_chart_size = time(NULL) - init_time;

            count++;
        }
    }
    scheduler->average_response_time = (double) total_response_time / count;
    scheduler->average_turnaround_time = (double) total_turn_around_time / count;
}

Scheduler* create_srtf_scheduler(int num_thread) {
    Scheduler* scheduler = (Scheduler*)malloc(sizeof(Scheduler));
    scheduler->gantt_chart = (int*)malloc(num_thread * MAX_BURST_TIME * sizeof(int));
    scheduler->ready_queue = createQueue();
    scheduler->add_process = srtf_add_process;
    scheduler->get_next_process = srtf_get_next_process;
    scheduler->schedule = srtf_schedule;
    scheduler->init_time = time(NULL);
    scheduler->average_response_time = 0;
    scheduler->average_turnaround_time = 0;
    return scheduler;
}