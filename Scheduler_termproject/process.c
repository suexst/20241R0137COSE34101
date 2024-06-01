#include "process.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdatomic.h>

atomic_int id = 1;

int new_id() {
    return atomic_fetch_add(&id, 1);
}

void print_process(Process *process) {
    if (process) {
        printf("Process ID: %d, Arrival Time: %d, CPU Burst Time: %d, Priority: %d\n",
               process->process_id, process->arrival_time, process->cpu_burst_time, process->priority);
    }
}

Process* create_process() {
    Process *process = (Process *)malloc(sizeof(Process));
    process->process_id = new_id();
    process->arrival_time = rand() % 15;
    process->cpu_burst_time = rand() % 10 + 1;
    process->priority = rand() % 20;
    process->response_time = 0;
    process->turn_around_time = 0;
    print_process(process);
    return process;
}

void destroy_process(Process *process) {
    if (process) {
        free(process);
    }
}