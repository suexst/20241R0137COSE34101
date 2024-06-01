#ifndef SCHEDULER_H
#define SCHEDULER_H

#include <time.h>
#include <stdlib.h>
#include <pthread.h>
#include "process.h"


typedef struct Node {
    Process* process;
    struct Node* next;
} Node;

typedef struct Queue {
    Node* front;
    Node* rear;
    pthread_mutex_t lock;
} Queue;

Queue* createQueue();
void enqueue(Queue* q, Process* process);
void sjf_enqueue(Queue* q, Process* process);
void priority_enqueue(Queue* q, Process* process);
Process* dequeue(Queue* q);
Process* front(Queue* q);

typedef struct Scheduler {
    void (*add_process)(struct Scheduler*, Process*);
    void (*schedule)(struct Scheduler*, int num_thread);
    Process* (*get_next_process)(struct Scheduler*);
    Queue* ready_queue;
    int* gantt_chart;
    int gantt_chart_size;
    time_t init_time;
    double average_response_time;
    double average_turnaround_time;
    pthread_mutex_t lock;
} Scheduler;

Scheduler* create_fcfs_scheduler(int num_thread);
Scheduler* create_sjf_scheduler(int num_thread);
Scheduler* create_srtf_scheduler(int num_thread);
Scheduler* create_np_priority_scheduler(int num_thread);
Scheduler* create_p_priority_scheduler(int num_thread);
Scheduler* create_rr_scheduler(int num_thread);
void destroy_scheduler(Scheduler* scheduler);

#endif // SCHEDULER_H