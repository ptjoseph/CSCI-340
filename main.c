#include <stdio.h>
#include <assert.h>
#include <pthread.h>
#include <semaphore.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>

#define NUM_ITERATIONS 100000
#define MAX_BUFFER_SIZE 10

sem_t mutex;
sem_t full;
sem_t empty;

int buffer[MAX_BUFFER_SIZE];
int fillIndex = 0;
int useIndex = 0;
int count = 0;


void put(int value){
    buffer[fillIndex] = value;
    fillIndex = (fillIndex + 1);
}

void *producer(void *arg){
    
    sem_wait(&empty);
    sem_wait(&mutex);
        
    put(count);
        
    sem_post(&mutex);
    sem_post(&full);

    count++;
    
    return 0;
}

int get(){
    int tmp = buffer[useIndex];
    useIndex = (useIndex + 1);
    return tmp;
}

void *consumer(void *arg) {
    sem_wait(&full);
    sem_wait(&mutex);
    
    int temporary = get();
    
    sem_post(&mutex);
    sem_post(&empty);
    
    printf("Removing %d from the buffer\n", temporary);
    
    return 0;
}



int main(int argc, char *argv[]){
    pthread_t p1,p2;
    int rc;

    //initialize semaphore that will guarantee mutual exclusive access to the shared counter.

    sem_init(&mutex, 0,1);
    sem_init(&full, 0, 0);
    sem_init(&empty, 0, MAX_BUFFER_SIZE);


    printf("main:begin\n");
    rc = pthread_create(&p1, NULL, producer, "A");
    assert(rc==0);
    
    rc = pthread_create(&p2, NULL, consumer, "B");
    assert(rc==0);
    
    rc = pthread_join(p1,NULL);
    assert(rc==0);
    
    rc = pthread_join(p2,NULL);
    assert(rc==0);
    
    printf("main: end\n");
    printf("Total Items Produced: %d\n",fillIndex);
    printf("Total Items Consumed: %d\n", useIndex);
    
    return 0;

}
