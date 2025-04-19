#include "user.h"

struct all{
    int* x;
    struct semaphore *s;
};

void* thread2(void* arg){
    struct all* t = (struct all*) arg;
    printf(1,"Gonna acquire semaphore\n");
    semDown(t->s);
    printf(1,"Acquired semaphore successfully\n");
    int* argPtr = t->x;
    *argPtr = (*argPtr + 1);
    semUp(t->s);
    thread_exit();
    return 0;
}

int main(){
    struct semaphore s;
    semInit(&s,1);
    printf(1, "Hello World\n");
    int x = 10;
    struct all a;
    a.x = &x;
    a.s = &s;
    uint tid1;
    semDown(&s);
    thread_create(&tid1,thread2,(void*)&a);
    sleep(200);
    semUp(&s);
    thread_join(tid1);
    printf(1,"Value of x = %d\n",x);
    exit();
}