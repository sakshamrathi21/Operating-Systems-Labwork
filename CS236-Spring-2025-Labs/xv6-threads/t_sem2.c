#include "user.h"

int CHILDSEM=0;
int PARENTSEM=1;

struct semaphore s1, s2;

void* thread1(void* arg){
    semDown(&s1);
    printf(1, "I am thread 1\n");
    semUp(&s2);
    thread_exit();
    return 0;
}

void* thread2(void* arg){
    semDown(&s1);
    printf(1, "I am thread 2\n");
    semUp(&s2);
    thread_exit();
    return 0;
}

void* thread3(void* arg){
    sleep(2);

    //inspite of the sleep, parent should print first
    printf(1, "I am thread3 and I should print first\n");

    semUp(&s1);
    semDown(&s2);

    printf(1, "Only one other thread has printed by now\n");

    semUp(&s1);
    semDown(&s2);
    printf(1, "Both the other threads have printed by now\n");

    thread_exit();
    return 0;
}

int main()
{
    semInit(&s1,0);
    semInit(&s2,0);
  
    uint tid1, tid2, tid3;
    thread_create(&tid1, thread1, 0);
    thread_create(&tid2, thread2, 0);
    thread_create(&tid3, thread3, 0);

    thread_join(tid1);
    thread_join(tid2);
    thread_join(tid3);

    exit();
}

