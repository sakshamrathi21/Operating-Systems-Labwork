#include<stdio.h>
#include<pthread.h>
#include<unistd.h>

#define NUM_THREADS 100
int current_thread = 1;
pthread_cond_t c = PTHREAD_COND_INITIALIZER;
pthread_mutex_t m = PTHREAD_MUTEX_INITIALIZER;

void* increment_counter(void* thread_number) {
    long t_id = (long)thread_number;
    usleep(700000);
    pthread_mutex_lock(&m);
    while (current_thread < t_id)
    {
        pthread_cond_wait(&c, &m);
    }
    printf("Hello, I am thread %ld\n", t_id);
    current_thread ++;
    pthread_cond_broadcast(&c);  
    pthread_mutex_unlock(&m);
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