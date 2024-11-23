#include "alloc.h"
#include <fcntl.h>
#include <sys/stat.h>

char* buf;

int bit[PAGESIZE/8];
int chunk_size[PAGESIZE/8];

int init_alloc() {
    // int fd = open(__FILE__, O_RDONLY);
    buf = mmap(NULL, PAGESIZE, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    memset(bit,0,(PAGESIZE/8)*sizeof(int));
    memset(chunk_size,0,(PAGESIZE/8)*sizeof(int));
    if (buf == MAP_FAILED) {
        perror("Error mapping file");
        // close(fd);
        return 1;
    }
    return 0;
}

int cleanup() {
    if (munmap(buf, PAGESIZE) == -1) return -1;
    return 0;
}

char *alloc(int size) {
    if (size % 8 != 0) {
        return NULL;
    }
    int num_chunks = size/8;
    // printf("check %d\n", num_chunks);
    if (num_chunks == 0) return NULL;
    int current = 0;
    for (int i = 0 ; i < PAGESIZE/8 ; i ++ ) {
        if (bit[i]) current = 0;
        else current++;
        if (current == num_chunks) {
            int j = i+1-num_chunks;
            for (int l = j ; l < j+num_chunks ; l ++ ) {
                bit[l] = 1;
            }
            chunk_size[j] = num_chunks;
            // printf("check %d\n", i+1-num_chunks);
            return (buf+(i+1-num_chunks)*8);
        }
    }
    return NULL;
}

void dealloc(char * to_be_freed) {
    if (to_be_freed - buf < 0 || (to_be_freed - buf) >= PAGESIZE || (to_be_freed-buf)%8 != 0) return;
    int start = (to_be_freed-buf)/8;
    if (chunk_size[start] == 0) return;
    int chunk_sz = chunk_size[start];
    chunk_size[start] = 0;
    for (int i = start ; i < start + chunk_sz ; i ++ ) {
        bit[i] = 0;
    }
    return;
}