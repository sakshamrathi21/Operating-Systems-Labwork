#include<stdio.h>
#include<pthread.h>

#define NUM_THREADS 10

void* increment_counter(void* thread_number) {
    long t_id = (long)thread_number;
    printf("Hello, I am thread %ld\n", t_id);
    return NULL;
}

int main (int argc, char* argv[]) {
    pthread_t threads[NUM_THREADS];
    for (long i = 0 ; i < NUM_THREADS ; i ++ ) {
        long t = i + 1;
        pthread_create(&threads[i], NULL, increment_counter, (void*)(t));
    }
    for (int i = 0 ; i < NUM_THREADS ; i ++ ) {
        pthread_join(threads[i], NULL);
    }
    printf("Hello, I am the main thread\n");
    return 0;
}