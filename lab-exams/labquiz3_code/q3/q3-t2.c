#include "types.h"
#include "stat.h"
#include "user.h"

int
main(int argc, char *argv[])
{
    int f = fork();

    if(f == 0){
        sleep(50);
        int free_pages;
        free_pages = num_free_pages();
        printf(1, "[CHILD] Number of free pages: %d\n", free_pages);
        char* ptr = lazy_shm_attach();
        printf(1, "[CHILD] page attached to 0x%x\n", ptr);
        sleep(50);
        printf(1, "[CHILD] Value: %d\n", *(uint*)ptr);
        free_pages = num_free_pages();
        printf(1, "[CHILD] Number of free pages: %d\n", free_pages);
        exit();
    }
    else{
        int free_pages;
        free_pages = num_free_pages();
        printf(1, "[PARENT] Number of free pages: %d\n", free_pages);
        char* ptr = lazy_shm_attach();
        printf(1, "[PARENT] page attached to 0x%x\n", ptr);
        *(uint*)ptr = 17;
        printf(1, "[PARENT] value written\n");
        free_pages = num_free_pages();
        printf(1, "[PARENT] Number of free pages: %d\n", free_pages);
        wait();
        exit();
    }
}