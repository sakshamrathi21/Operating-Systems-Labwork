#include <stdio.h>
#include <pthread.h>

int counter = 0;

pthread_mutex_t lock;

void* inc(void* arg){
    pthread_mutex_lock(&lock);
    for(int i = 0; i < 1000; i++){
        counter++;
    }
    pthread_mutex_unlock(&lock);
    return NULL;
}

int main(){
    pthread_t ids[1000];
    pthread_mutex_init(&lock, NULL);
    for(int i = 0; i  < 1000; i++){
        pthread_create(&(ids[i]), NULL, inc, NULL);
    }
    for(int i = 0; i  < 1000; i++){
        pthread_join(ids[i], NULL);
    }
    pthread_mutex_destroy(&lock);
    printf("counter: %d\n", counter);

}