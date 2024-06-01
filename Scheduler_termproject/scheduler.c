#include <stdlib.h>
#include "scheduler.h"

Queue* createQueue() {
    Queue* q = (Queue*)malloc(sizeof(Queue));
    q->front = q->rear = NULL;
    pthread_mutex_init(&q->lock, NULL);
    return q;
}

void enqueue(Queue* q, Process* process) {
    pthread_mutex_lock(&q->lock);

    Node* temp = (Node*)malloc(sizeof(Node));
    temp->process = process;
    temp->next = NULL;
    if (q->rear == NULL) {
        q->front = q->rear = temp;
    } else {
        q->rear->next = temp;
        q->rear = temp;
    }

    pthread_mutex_unlock(&q->lock);
}

void sjf_enqueue(Queue* q, Process* process) {
    pthread_mutex_lock(&q->lock);

    Node* temp = (Node*)malloc(sizeof(Node));
    temp->process = process;
    temp->next = NULL;

    if (q->front == NULL) {
        q->front = q->rear = temp;
    } else if (q->front->process->cpu_burst_time > process->cpu_burst_time) {
        temp->next = q->front;
        q->front = temp;
    } else {
        Node* current = q->front;
        while (current->next != NULL && current->next->process->cpu_burst_time <= process->cpu_burst_time) {
            current = current->next;
        }
        temp->next = current->next;
        current->next = temp;

        if (temp->next == NULL) {
            q->rear = temp;
        }
    }

    pthread_mutex_unlock(&q->lock);
}

void priority_enqueue(Queue* q, Process* process) {
    pthread_mutex_lock(&q->lock);

    Node* temp = (Node*)malloc(sizeof(Node));
    temp->process = process;
    temp->next = NULL;

    if (q->front == NULL) {
        q->front = q->rear = temp;
    } else if (q->front->process->priority > process->priority) {
        temp->next = q->front;
        q->front = temp;
    } else {
        Node* current = q->front;
        while (current->next != NULL && current->next->process->priority <= process->priority) {
            current = current->next;
        }
        temp->next = current->next;
        current->next = temp;

        if (temp->next == NULL) {
            q->rear = temp;
        }
    }

    pthread_mutex_unlock(&q->lock);
}

Process* dequeue(Queue* q) {
    pthread_mutex_lock(&q->lock);

    if (q->front == NULL) {
        pthread_mutex_unlock(&q->lock);
        return NULL;
    }
    Node* temp = q->front;
    Process* process = temp->process;
    q->front = q->front->next;
    if (q->front == NULL) {
        q->rear = NULL;
    }
    free(temp);

    pthread_mutex_unlock(&q->lock);
    return process;
}

Process* front(Queue* q) {
    pthread_mutex_lock(&q->lock);
    if (q->front == NULL) {
        pthread_mutex_unlock(&q->lock);
        return NULL;
    }
    Process* process = q->front->process;
    pthread_mutex_unlock(&q->lock);
    return process;
}

void destroyQueue(Queue* q) {
    pthread_mutex_lock(&q->lock);
    while (q->front != NULL) {
        Node* temp = q->front;
        q->front = q->front->next;
        free(temp);
    }
    pthread_mutex_unlock(&q->lock);
    pthread_mutex_destroy(&q->lock);
    free(q);
}

void destroy_scheduler(Scheduler* scheduler) {
    pthread_mutex_lock(&scheduler->lock);
    Process* current;
    while ((current = dequeue(scheduler->ready_queue)) != NULL) {
        free(current);
    }
    destroyQueue(scheduler->ready_queue);
    pthread_mutex_unlock(&scheduler->lock);
    pthread_mutex_destroy(&scheduler->lock);
    free(scheduler->gantt_chart);
    free(scheduler);
}