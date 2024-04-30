#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>


void* inc(void* arg){
    int i = *(int*)arg;
    printf("I am thread %d\n", i);
    return NULL;
}

int main(){
    int N;
    scanf("%d", &N);
    pthread_t ids[N];
    int args[N];
    for(int i = 0; i  < N; i++){
        args[i] = i;    // why is this required
        pthread_create(&(ids[i]), NULL, inc, &(args[i]));
    }
    for(int i = 0; i  < N; i++){
        pthread_join(ids[i], NULL);
    }
    printf("I am the main thread\n");
}