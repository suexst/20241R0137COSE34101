#include <time.h>
#include <stdio.h>
#include <pthread.h>
#include "process.h"
#include "scheduler.h"

#define NUM_PROCESSES 20

int compare_arrival_time(const void* a, const void* b) {
    Process* process_a = *(Process**)a;
    Process* process_b = *(Process**)b;
    return process_a->arrival_time - process_b->arrival_time;
}

void* add_processes(void* arg) {
    Scheduler* scheduler = (Scheduler*)arg;
    Process* processes[NUM_PROCESSES];

    for (int i = 0; i < NUM_PROCESSES; ++i) {
        processes[i] = create_process();
    }
    
    qsort(processes, NUM_PROCESSES, sizeof(Process*), compare_arrival_time);

    time_t init_time = time(NULL);
    for (int i = 0; i < NUM_PROCESSES; ++i) {
        Process* process = processes[i];
        while (process->arrival_time > time(NULL) - init_time) {};
        scheduler->add_process(scheduler, process);
    }

    pthread_exit(NULL);
}

void* schedule_processes(void* arg) {
    Scheduler* scheduler = (Scheduler*)arg;
    scheduler->schedule(scheduler, NUM_PROCESSES);
    pthread_exit(NULL);
}

int main() {
    Scheduler* scheduler;

#if SCHEDULER_TYPE == 1
    scheduler = create_fcfs_scheduler(NUM_PROCESSES);
#elif SCHEDULER_TYPE == 2
    scheduler = create_sjf_scheduler(NUM_PROCESSES);
#elif SCHEDULER_TYPE == 3
    scheduler = create_srtf_scheduler(NUM_PROCESSES);
#elif SCHEDULER_TYPE == 4
    scheduler = create_np_priority_scheduler(NUM_PROCESSES);
#elif SCHEDULER_TYPE == 5
    scheduler = create_p_priority_scheduler(NUM_PROCESSES);
#elif SCHEDULER_TYPE == 6
    scheduler = create_rr_scheduler(NUM_PROCESSES);
#else
    fprintf(stderr, "Unsupported scheduler type\n");
    return 1;
#endif

    pthread_t add_thread, schedule_thread;
    pthread_create(&add_thread, NULL, add_processes, scheduler);
    pthread_create(&schedule_thread, NULL, schedule_processes, scheduler);

    pthread_join(add_thread, NULL);
    pthread_join(schedule_thread, NULL);

    printf("Average Response Time : %.1f\n", scheduler->average_response_time);
    printf("Average Turnaround Time : %.1f\n", scheduler->average_turnaround_time);

    int* gantt = scheduler->gantt_chart;
    printf(" | p%d |", gantt[0]);
    for(int i=1; i<scheduler->gantt_chart_size; ++i) {
        if(gantt[i] > 0)
            printf(" p%d |", gantt[i]);
    }
    printf("\n");

    destroy_scheduler(scheduler);

    return 0;
}