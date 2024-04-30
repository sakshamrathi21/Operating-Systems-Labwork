#include "types.h"
#include "user.h"

int main(){
    int arr[2];
    arr[0] = -1;
    arr[1] = -1;
    pt_count(arr);
    printf(1, "user pt count = %d\nkernel pt count = %d\n", arr[0], arr[1]);
    // Allocate 4096*1024*2 bytes of memory
    printf(1, "Allocating %d bytes of memory\n", 4096*1024*7);
    char *a = sbrk(4096*1024*7);
    if(a == (char*)-1){
        printf(1, "sbrk failed\n");
        exit();
    }
    pt_count(arr);
    printf(1, "user pt count = %d\nkernel pt count = %d\n", arr[0], arr[1]);
    // Deallocate 4096*1024*2 bytes of memory
    printf(1, "Deallocating %d bytes of memory\n", 4096*1024*4);
    char *b = sbrk(-4096*1024*4);
    if(b == (char*)-1){
        printf(1, "sbrk failed\n");
        exit();
    }
    pt_count(arr);
    printf(1, "user pt count = %d\nkernel pt count = %d\n", arr[0], arr[1]);
    exit();
}