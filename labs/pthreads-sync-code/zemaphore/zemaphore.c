#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <string.h>
#include <errno.h>
#include <signal.h>
#include <wait.h>
#include "zemaphore.h"

void zem_init(zem_t *s, int value) {
    pthread_mutex_init(&(s->main_lock), NULL);
    s->c = PTHREAD_COND_INITIALIZER;
    s->zem_counter = value;
}

void zem_down(zem_t *s) {
    pthread_mutex_lock(&(s->main_lock));
    s->zem_counter--;
    if (s->zem_counter < 0) pthread_cond_wait(&(s->c), &(s->main_lock));
    pthread_mutex_unlock(&(s->main_lock));
}

void zem_up(zem_t *s) {
    pthread_mutex_lock(&(s->main_lock));
    s->zem_counter ++;
    pthread_cond_signal(&(s->c));
    pthread_mutex_unlock(&(s->main_lock));
}
