#include "types.h"
#include "stat.h"
#include "user.h"

int
main(int argc, char *argv[])
{
    int free_pages;
    free_pages = num_free_pages();
    printf(1, "Number of free pages: %d\n", free_pages);
    char* ptr = lazy_shm_attach();
    printf(1, "page attached to 0x%x\n", ptr);
    free_pages = num_free_pages();
    printf(1, "Number of free pages: %d\n", free_pages);
    *(uint*)ptr = 17;
    printf(1, "value written\n");
    printf(1, "Value: %d\n", *(uint*)ptr);
    free_pages = num_free_pages();
    printf(1, "Number of free pages: %d\n", free_pages);
    lazy_shm_detach(ptr);
    printf(1, "page detached\n");
    free_pages = num_free_pages();
    printf(1, "Number of free pages: %d\n", free_pages);
    exit();
}