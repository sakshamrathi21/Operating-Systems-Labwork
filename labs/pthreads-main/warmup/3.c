#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>

pthread_cond_t cond;
pthread_mutex_t lock;

int current = 0;

void* inc(void* arg){
    sleep(rand() % 3);
    int i = *(int*)arg;
    pthread_mutex_lock(&lock);
    while(current != i){
        pthread_cond_wait(&cond, &lock);
    }
    printf("I am thread %d\n", i);
    current++;
    pthread_cond_broadcast(&cond);
    pthread_mutex_unlock(&lock);
    return NULL;
}

int main(){
    pthread_cond_init(&cond, NULL);
    pthread_mutex_init(&lock, NULL);
    int N;
    scanf("%d", &N);
    pthread_t ids[N];
    int args[N];
    for(int i = 0; i < N; i++){
        args[i] = i;    // why is this required
        pthread_create(&(ids[i]), NULL, inc, &(args[i]));
    }
    for(int i = 0; i  < N; i++){
        pthread_join(ids[i], NULL);
    }
    printf("I am the main thread\n");
}