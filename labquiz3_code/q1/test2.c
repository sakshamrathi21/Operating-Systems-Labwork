#include "types.h"
#include "user.h"

int main(){
    int arr[2];
    arr[0] = -1;
    arr[1] = -1;
    pt_count(arr);
    printf(1, "user pt count = %d\nkernel pt count = %d\n", arr[0], arr[1]);


    printf(1, "Allocating %d bytes of memory\n", 4096*256);
    char *a = sbrk(4096*256);
    if(a == (char*)-1){
        printf(1, "sbrk failed\n");
        exit();
    }
    pt_count(arr);
    printf(1, "user pt count = %d\nkernel pt count = %d\n", arr[0], arr[1]);


    printf(1, "Allocating %d bytes of memory\n", 4096*256);
    char *b = sbrk(4096*256);
    if(b == (char*)-1){
        printf(1, "sbrk failed\n");
        exit();
    }
    pt_count(arr);
    printf(1, "user pt count = %d\nkernel pt count = %d\n", arr[0], arr[1]);


    printf(1, "Allocating %d bytes of memory\n", 4096*(4096+512));
    char *c = sbrk(4096*(4096+512));
    if(c == (char*)-1){
        printf(1, "sbrk failed\n");
        exit();
    }
    pt_count(arr);
    printf(1, "user pt count = %d\nkernel pt count = %d\n", arr[0], arr[1]);
    
    printf(1, "Deallocating %d bytes of memory\n", 4096);
    char *d = sbrk(-4096);
    if(d == (char*)-1){
        printf(1, "sbrk failed\n");
        exit();
    }
    pt_count(arr);
    printf(1, "user pt count = %d\nkernel pt count = %d\n", arr[0], arr[1]);

    printf(1, "Deallocating %d bytes of memory\n", 4096*512+8);
    char *e = sbrk(-4096*512-8);
    if(e == (char*)-1){
        printf(1, "sbrk failed\n");
        exit();
    }
    pt_count(arr);
    printf(1, "user pt count = %d\nkernel pt count = %d\n", arr[0], arr[1]);


    printf(1, "Deallocating %d bytes of memory\n", 4096*1540);
    char *f = sbrk(-4096*1540);
    if(f == (char*)-1){
        printf(1, "sbrk failed\n");
        exit();
    }
    pt_count(arr);
    printf(1, "user pt count = %d\nkernel pt count = %d\n", arr[0], arr[1]);
    exit();
}