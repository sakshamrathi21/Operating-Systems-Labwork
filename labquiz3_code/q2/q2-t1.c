#include "types.h"
#include "stat.h"
#include "user.h"

int
main(int argc, char *argv[])
{
    if(argc < 2){
        printf(1, "Usage: %s <0/1>\n", argv[0]);
        exit();
    }

    if(atoi(argv[1]) == 0){
        char* ptr = shm_attach(0);
        printf(1, "page attached to 0x%x\n", ptr);
        *(uint*)ptr = 17;
        printf(1, "value written\n");
        printf(1, "Value: %d\n", *(uint*)ptr);
        shm_detach(ptr);
    }
    else if(atoi(argv[1]) == 1){
        char* ptr = shm_attach(0);
        printf(1, "page attached to 0x%x\n", ptr);
        printf(1, "reading value\n");
        printf(1, "Value: %d\n", *(uint*)ptr);
        shm_detach(ptr);
    }
    else{
        printf(1, "Usage: %s <0/1>\n", argv[0]);
    }
    exit();
}