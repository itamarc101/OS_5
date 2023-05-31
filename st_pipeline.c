#include <math.h>
#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

// PART A:

int isPrime(unsigned int number)
{
    if (number < 2)
    {
        printf("false\n");
        return 0;
    }

    if (number == 2)
    {
        printf("true\n");
        return 1;
    }

    if (number % 2 == 0)
    {
        printf("false\n");
        return 0;
    }

    unsigned int sqrtNum = sqrt(number);
    for (unsigned int i = 3; i <= sqrtNum; i += 2)
    {
        if (number % i == 0)
        {
            printf("false\n");
            return 0;
        }
    }
    printf("true\n");
    return 1;
}

// PART B:

typedef struct Node
{
    struct Node *next;
    void *data;
} Node;

typedef struct Queue
{
    Node *head;
    Node *tail;
    pthread_mutex_t mutex;
    pthread_cond_t cond;
} Queue;

Queue *st_queue_create()
{
    Queue *queue = malloc(sizeof(Queue));
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

void st_queue_destroy(Queue *queue)
{
    pthread_mutex_lock(&queue->mutex);

    Node *curr = queue->head;
    while (curr != NULL)
    {
        Node *next = curr->next;
        free(curr);
        curr = next;
    }

    pthread_mutex_unlock(&queue->mutex);
    pthread_mutex_destroy(&queue->mutex);
    pthread_cond_destroy(&queue->cond);
    free(queue);
}

void st_queue_push(Queue *queue, void *item)
{
    Node *node = malloc(sizeof(Node));
    if (node == NULL)
    {
        perror("malloc");
        exit(1);
    }
    node->data = item;
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

void *st_queue_pop(Queue *queue)
{
    pthread_mutex_lock(&queue->mutex);

    while (queue->head == NULL)
    {
        pthread_cond_wait(&queue->cond, &queue->mutex);
    }

    Node *node = queue->head;
    void *item = node->data;
    queue->head = node->next;

    if (queue->head == NULL)
    {
        queue->tail = NULL;
    }

    free(node);
    pthread_mutex_unlock(&queue->mutex);
    return item;
}

int st_queue_is_empty(Queue *queue)
{
    pthread_mutex_lock(&queue->mutex);
    int isEmpty = (queue->head == NULL);
    pthread_mutex_unlock(&queue->mutex);
    return isEmpty;
}


// PART C:

typedef struct AO
{
    Queue* queue;
    pthread_t thread;
    void (*func)(void*);
    int active;
}AO;

void* run(void* arg)
{
    AO * ao = (AO*) arg;
    void * task;
    while((task = st_queue_pop(ao->queue)) != NULL) ao->func(task);
    return NULL;
}

AO* createActiveObject(int (*func)(void*))
{
    AO* ao = (AO*)malloc(sizeof(AO));
    if(ao == NULL) return NULL;

    ao->queue = st_queue_create();
    // NEED TO CHECK THISSSS 
    // MAYBE NEED TO DO IN TYPEDEF STRUCT AO - "struct AO * next;""
    // MAYBE NEED TO ADD THISSS ao->next = next;
    // bimkom func ulay zarih next
    // vvvvvv
    
    //ao->func = func;
    ao->active = 1;
    pthread_create(&ao->thread, NULL, run, ao);
    return ao;
}

Queue * getQueue(AO* ao)
{
    return ao->queue;
}

void stop(AO* ao)
{
    ao->active = 0;
    pthread_join(ao->thread, NULL);
    st_queue_destroy(ao->queue);
    free(ao);
}


// PART D:

void func1(void* arg)
{
    int count = *(int*)arg;
    srand(count);

    for (int i = 0; i < count; i++) {
        int number = rand() % 900000 + 100000;
        st_queue_push(getQueue((AO*)arg), (void*)&number);
        sleep(1);
    }
}

void func2(void * arg)
{
    int number = *(int*)arg;
    printf("Received number: %d\n", number);

    // Check if the number is prim

    printf("%s\n", isPrime(number) ? "true " : "false");
    // Add 11 to the number and pass it to the next AO
    number += 11;
    st_queue_push(getQueue((AO*)arg), (void*)&number);
}


