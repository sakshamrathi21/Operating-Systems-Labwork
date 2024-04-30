#include "types.h"
#include "stat.h"
#include "user.h"

// note that no detach is being used
// still program should run correctly multiple times

int
main(int argc, char *argv[])
{
    int f = fork();

    if(f == 0){
        char* ptr = shm_attach(8);
        sleep(50);
        printf(1, "Value: %d\n", *(uint*)ptr);
        exit();
    }
    else{
        char* ptr = shm_attach(8);
        *(uint*)ptr = 17;
        wait();
    }
    exit();
}