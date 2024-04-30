#include<stdio.h>
#include<pthread.h>

#define NUM_THREADS 10
#define COUNT_INCREMENT 1000
pthread_mutex_t lock;

int counter = 0;

void* increment_counter(void* arg) {
    
    for (int i = 0 ; i < COUNT_INCREMENT ;  i ++ ) {
        pthread_mutex_lock(&lock);
        counter ++;
        pthread_mutex_unlock(&lock);
    } 
    return NULL;
}

int main (int argc, char* argv[]) {
    pthread_t threads[NUM_THREADS];
    pthread_mutex_init(&lock, NULL);
    for (int i = 0 ; i < NUM_THREADS ; i ++ ) {
        
        pthread_create(&threads[i], NULL, increment_counter, NULL);
        // printf("Final Counter Value: %d\n", counter);
        // pthread_exit(NULL);
    }
    for (int i = 0 ; i < NUM_THREADS ; i ++ ) {
        pthread_join(threads[i], NULL);
    }
    pthread_mutex_destroy(&lock);
    // pthread_exit(NULL);
    printf("Final Counter Value: %d\n", counter);
    return 0;
}