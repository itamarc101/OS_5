// PART B:
#include "queue.h"
#include <time.h>

pqueue createQueue()
{
    pqueue queue = malloc(sizeof(Queue));
    if (queue == NULL)
    {
        perror("malloc");
        exit(1);
    }

    queue->head = NULL;
    queue->tail = NULL;
    pthread_mutex_init(&queue->mutex, NULL);
    pthread_cond_init(&queue->cond, NULL);

    return queue;
}

void deleteQueue(pqueue queue)
{
    pthread_mutex_lock(&queue->mutex);

    pnode save = queue->head;
    while (save != NULL)
    {
        pnode next = save->next;
        free(save);
        save = next;
    }

    pthread_mutex_unlock(&queue->mutex);
    pthread_mutex_destroy(&queue->mutex);
    pthread_cond_destroy(&queue->cond);
    free(queue);
}

void enqueue(pqueue queue, void *task)
{
    pnode node = malloc(sizeof(Node));
    if (node == NULL)
    {
        exit(1);
    }
    node->task = task;
    node->next = NULL;

    pthread_mutex_lock(&queue->mutex);

    if (queue->head == NULL)
    {
        queue->head = node;
        queue->tail = node;
    }
    else
    {
        queue->tail->next = node;
        queue->tail = node;
    }

    pthread_cond_signal(&queue->cond);
    pthread_mutex_unlock(&queue->mutex);
}

void *dequeue(pqueue queue)
{
    
    pthread_mutex_lock(&queue->mutex);

    while (queue->head == NULL)
    {
        pthread_cond_wait(&queue->cond, &queue->mutex);
    }
    
    pnode node = queue->head;
    void *save = node->task;
    queue->head = node->next;
    if (queue->head == NULL)
    {
        queue->tail = NULL;
    }

    free(node);
    pthread_mutex_unlock(&queue->mutex);
    return save;
}

int isEmpty(pqueue queue)
{
    pthread_mutex_lock(&queue->mutex);
    int isempty = (queue->head == NULL);
    pthread_mutex_unlock(&queue->mutex);
    return isempty;
}
