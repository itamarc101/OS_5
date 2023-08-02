
// PART C:
#include "activeobject.h"
#include <stdint.h>

void* run(void* arg)
{
    pao this = (pao) arg;
    void * obj;
    while((obj = dequeue(this->queue)) != NULL) {
        this->func(obj);
        if(this->flag != 1 && this->next != NULL){
            enqueue(this->next->queue, obj);
        }
    }
    if(this->next != NULL)
        enqueue(this->next->queue, obj);
    return NULL;
}

pao createActiveObject(void  (*func)(void*), pao next)
{
    pao obj = (pao)malloc(sizeof(ao));
    if(obj == NULL) return NULL;
    obj->queue = createQueue();
    obj->next = next;
    obj->func = func;
    pthread_create(&obj->thread, NULL, run, obj);
    return obj;
}

pqueue getQueue(pao obj)
{
    return obj->queue;
}

void stop(pao obj)
{
    pthread_join(obj->thread, NULL);
    deleteQueue(obj->queue);
    free(obj);
}

