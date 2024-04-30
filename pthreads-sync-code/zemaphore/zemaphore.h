#include <pthread.h>

typedef struct zemaphore {
    pthread_mutex_t main_lock;
    pthread_cond_t c;
    int zem_counter;
} zem_t;

void zem_init(zem_t *, int);
void zem_up(zem_t *);
void zem_down(zem_t *);
