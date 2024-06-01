#ifndef PROCESS_H
#define PROCESS_H

#include <stdatomic.h>

typedef struct Process {
    int process_id;
    int arrival_time;
    int cpu_burst_time;
    int priority;
    int response_time;
    int turn_around_time;
} Process;

extern atomic_int id;

Process* create_process();
void print_process(Process *process);
void destroy_process(Process *process);
int new_id();

#endif