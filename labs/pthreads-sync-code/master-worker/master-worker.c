#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <string.h>
#include <errno.h>
#include <signal.h>
#include <sys/wait.h>
#include <pthread.h>

int item_to_produce, curr_buf_size;
int total_items, max_buf_size, num_workers, num_masters;
int processed = 0;
int current_consumed = 0;
pthread_mutex_t m = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t c_prod = PTHREAD_COND_INITIALIZER;
pthread_cond_t c_cons = PTHREAD_COND_INITIALIZER;

int *buffer;

void print_produced(int num, int master) {

  printf("Produced %d by master %d\n", num, master);
}

void print_consumed(int num, int worker) {

  printf("Consumed %d by worker %d\n", num, worker);
  
}

void* reading_buffer(void* data) {
    int worker_thread_id = *((int *) data);
    
    while(1)
    {
        pthread_mutex_lock(&m);
        if (curr_buf_size == 0 && current_consumed < total_items) {
            pthread_cond_wait(&c_cons, &m);
        }
        if(current_consumed >= item_to_produce) {
            pthread_mutex_unlock(&m);
            break;
        }
        pthread_cond_broadcast(&c_prod);
        int consum_num = buffer[processed];
        processed++;
        current_consumed ++;
        processed %= max_buf_size;
        curr_buf_size --;
        print_consumed(consum_num, worker_thread_id);
        pthread_mutex_unlock(&m);
    }
    
    return 0;
}

//produce items and place in buffer
//modify code below to synchronize correctly
void *generate_requests_loop(void *data)
{
    int thread_id = *((int *)data);
    
    while(1){
        pthread_mutex_lock(&m);
        if (curr_buf_size == max_buf_size && item_to_produce < total_items) {
            pthread_cond_wait(&c_prod, &m);
            
        }
        if(item_to_produce >= total_items) {
            pthread_mutex_unlock(&m);
            break;
        }
        pthread_cond_broadcast(&c_cons);
        buffer[curr_buf_size++] = item_to_produce;
        print_produced(item_to_produce, thread_id);
        item_to_produce++;
        pthread_mutex_unlock(&m);
    }
    
    return 0;
}

//write function to be run by worker threads
//ensure that the workers call the function print_consumed when they consume an item

int main(int argc, char *argv[])
{
  int *master_thread_id;
  pthread_t *master_thread;
  item_to_produce = 0;
  curr_buf_size = 0;
  
  
  int i;
  
   if (argc < 5) {
    printf("./master-worker #total_items #max_buf_size #num_workers #masters e.g. ./exe 10000 1000 4 3\n");
    exit(1);
  }
  else {
    num_masters = atoi(argv[4]);
    num_workers = atoi(argv[3]);
    total_items = atoi(argv[1]);
    max_buf_size = atoi(argv[2]);
  }
    
  pthread_t worker_threads[num_workers];
  int worker_thread_ids[num_workers];
   buffer = (int *)malloc (sizeof(int) * max_buf_size);

   //create master producer threads
   master_thread_id = (int *)malloc(sizeof(int) * num_masters);
   master_thread = (pthread_t *)malloc(sizeof(pthread_t) * num_masters);
  for (i = 0; i < num_masters; i++)
    master_thread_id[i] = i;

  for ( i = 0 ; i < num_workers ; i ++ ) worker_thread_ids[i] = i;

  for (i = 0; i < num_masters; i++)
    pthread_create(&master_thread[i], NULL, generate_requests_loop, (void *)&master_thread_id[i]);
  
  //create worker consumer threads
  for (int j = 0 ; j < num_workers ; j ++) {
    pthread_create(&worker_threads[j], NULL, reading_buffer, (void *)&worker_thread_ids[i]);
  }
  
  //wait for all threads to complete
  for (i = 0; i < num_masters; i++)
    {
      pthread_join(master_thread[i], NULL);
      printf("master %d joined\n", i);
    }

    for (i = 0 ; i < num_workers ; i ++) 
    {
        pthread_join(worker_threads[i], NULL);
        printf("worker %d joined\n", i);
    }
  
  /*----Deallocating Buffers---------------------*/
    free(buffer);
    free(master_thread_id);
    free(master_thread);
  
  return 0;
}
