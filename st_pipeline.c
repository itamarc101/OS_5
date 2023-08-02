#include <math.h>
#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include "activeobject.h"

// PART A:
int isPrime(unsigned int number)
{
    if (number == 2)
        return 1;
    if (number < 2 || number % 2 == 0)
        return 0;
    for (unsigned int i = 3; i * i <= number; i += 2)
    {
        if (number % i == 0)
        {
            return 0;
        }
    }
    return 1;
}

// PART D:

void first(void* arg)
{
    void **arr = (void **)arg;
    int N = *(int *)arr[0]; 
    int seed = *(int *)arr[1]; 
    pao ao1 = (pao)arr[2];
    srand(seed);
    for (int i = 0; i < N; i++) {
        int number = rand() % 900000 + 100000;
        enqueue(ao1->next->queue,(void *) &number);
        sleep(1);
    }
    enqueue(ao1->queue,NULL);

}

void second( void * arg)
{
    int *number = (int*)arg;
    printf("%d\n", *number);
    if(isPrime(*number))
        printf("true\n");
    else
        printf("false\n");
    *number += 11;
}

void third(void* arg)
{
    int * number = (int*)arg;
    printf("%d\n", *number);
    if(isPrime(*number))
        printf("true\n");
    else
        printf("false\n");
    *number -= 13;

}

void fourth(void * arg)
{
    int * number = (int*)arg;
    printf("%d\n", *number);
    if(isPrime(*number))
        printf("true\n");
    else
        printf("false\n");
    *number += 2;
    printf("%d\n", *number);

}


int main(int argc, char *argv[])
{
    if (argc < 2 || argc >3)
    {
        perror("eror with elements\n");
        return 1;
    }
    int N = atoi(argv[1]);
    int seed = argc > 2 ? atoi(argv[2]) : time(NULL);
    void **arr = (void **)malloc(sizeof(void *) * 3);
    if (arr == NULL)
    {
        fprintf(stderr, "Memory allocation failed\n");
        return 1;
    }
    arr[0] =(void*)& N;
    arr[1] = (void*)& seed;
    pao ao4 = createActiveObject(fourth, NULL);
    pao ao3 = createActiveObject(third, ao4);
    pao ao2 = createActiveObject(second, ao3);
    pao ao1 = createActiveObject(first, ao2);
    ao1->flag = 1;
    arr[2] = ao1;
    enqueue(ao1->queue, (void *)arr);
    sleep(1); 
    pthread_join(ao1->thread, NULL);
    pthread_join(ao2->thread, NULL);
    pthread_join(ao3->thread, NULL);
    pthread_join(ao4->thread, NULL);
    stop(ao1);
    stop(ao2);
    stop(ao3);
    stop(ao4);
    free(arr);
    return 0;
}
