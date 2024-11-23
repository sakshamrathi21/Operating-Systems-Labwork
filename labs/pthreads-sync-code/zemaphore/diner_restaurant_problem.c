#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <string.h>
#include <errno.h>
#include <signal.h>
#include <wait.h>
#include <pthread.h>
#include "zemaphore.h"

zem_t entering_diners, exiting_diners, enter_done, exit_done, mutex_enter, mutex_exit;
int count_enter, count_exit;

#define MAX_DINERS 10

void variables_init () {
    zem_init(&entering_diners, 0);
    zem_init(&exiting_diners, 0);
    zem_init(&enter_done, 0);
    zem_init(&exit_done, 0);
    zem_init(&mutex_enter, 0);
    zem_init(&mutex_exit, 0);
    count_enter = 0;
    count_exit = 0;
}

void* restaurant_thread(void* arg) {
    openFrontDoor();
    for (int i = 0 ; i < MAX_DINERS ; i ++ ) {
        zem_up(&entering_diners);
    }
    zem_down(&enter_done);
    closeFrontDoor();
    serveFood();
    openBackDoor();
    for (int i = 0 ; i < MAX_DINERS ; i ++ ) {
        zem_up(&exiting_diners);
    }
    zem_down(&exit_done);
    closeBackDoor();
}

void* diner_thread(void* arg) {
    zem_down(&entering_diners);
    enterRestaurant();
    zem_down(&mutex_enter);
    count_enter++;
    if (count_enter == MAX_DINERS) {
        zem_up(&enter_done);
        count_enter = 0;
    }
    zem_up(&mutex_enter);

    eat();

    zem_down(&exiting_diners);
    exitRestaurant();
    zem_down(&mutex_exit);
    count_exit++;
    if (count_exit == MAX_DINERS) {
        zem_up(&exit_done);
        count_exit = 0;
    }
    zem_up(&mutex_exit);
}