#ifndef QUEUE_H
#define QUEUE_H

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
typedef struct Node
{
    struct Node *next;
    void *task;
} Node, *pnode;

typedef struct Queue
{
    pnode head;
    pnode tail;
    pthread_mutex_t mutex;
    pthread_cond_t cond;
} Queue, *pqueue;
pqueue createQueue();
void deleteQueue(pqueue queue);
void enqueue(pqueue queue, void *task);
void * dequeue(pqueue queue);
int isEmpty(pqueue queue);
#endif