#include "ealloc.h"
#include <fcntl.h>
#include <sys/stat.h>

char* buf1 = NULL;
char* buf2 = NULL;
char* buf3 = NULL;
char* buf4 = NULL;

int bit[64];
int chunk_size[64];

void init_alloc(void) {
    memset(bit,0,((PAGESIZE/256)*4)*sizeof(int));
    memset(chunk_size,0,((PAGESIZE/256)*4)*sizeof(int));
    return;
}

char *alloc(int size_ealloc) {
    if (size_ealloc % 256 != 0 ) return NULL;
    int num_chunks = size_ealloc/256;
    if (num_chunks == 0) return NULL;
    
    for (int m = 0 ; m < 4 ; m ++ ) {
        char* current_buf;
        int offset = 0;
        if (m == 0) {
            if (buf1 == NULL) {
                buf1 = mmap(NULL, PAGESIZE, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
            }
            current_buf = buf1; offset = 0;
        }
        else if (m == 1) {
            if (buf2 == NULL) {
                buf2 = mmap(NULL, PAGESIZE, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
            }
            current_buf = buf2; offset = 16;
        }
        else if (m == 2) {
            if (buf3 == NULL) {
                buf3 = mmap(NULL, PAGESIZE, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
            }
            current_buf = buf3; offset = 32;
        }
        else if (m == 3) {
            if (buf4 == NULL) {
                buf4 = mmap(NULL, PAGESIZE, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
            }
            current_buf = buf4; offset = 48;
        }
        int current = 0;
        // if (num_chunks == 16) {
        //     printf("rr\n");
        // }
        // printf("offset: %d\n", offset);
        for (int i = 0 ; i < 16 ; i ++ ) {
            if (bit[i+offset]) current = 0;
            else current++;
            if (current == num_chunks) {
                int j = offset + i+1-num_chunks;
                for (int l = j ; l < j+num_chunks ; l ++ ) {
                    bit[l] = 1;
                }
                chunk_size[j] = num_chunks;
                // printf("check %d\n", i+1-num_chunks);
                // printf("check%d\n", i);
                return (current_buf+(i+1-num_chunks)*256);
            }
    }
    }
    return NULL;
}

void dealloc(char * to_be_freed) {
    char* currentbuf;
    int offset = 0;
    if ((to_be_freed - buf1) < PAGESIZE && (to_be_freed - buf1) >= 0) {
        // printf("check %d\n", (to_be_freed - buf1)); 
        currentbuf = buf1; 
        offset = 0;
    }
    else if ((to_be_freed - buf2) < PAGESIZE && (to_be_freed - buf2) >= 0) {currentbuf = buf2; offset = 16;}
    else if ((to_be_freed - buf3) < PAGESIZE && (to_be_freed - buf3) >= 0) {currentbuf = buf3; offset = 32;}
    else if ((to_be_freed - buf4) < PAGESIZE && (to_be_freed - buf4) >= 0) {currentbuf = buf4; offset = 48;}
    else {
        // printf("hellodd");
        return;
    }
    
    int start = (to_be_freed - currentbuf)/256;
    if (chunk_size[start+offset] == 0) return;
    
    int chunk_sz = chunk_size[start+offset];
    // printf("hello %d\n", (chunk_sz));
    chunk_size[start+offset] = 0;
   
    // int ss = 0;
    for (int i = start+offset ; i < start + offset + chunk_sz ; i ++ ) {
        // 
        // printf("rrr%d\n", i);
        bit[i] = 0;
    }
    return;
    
}

void cleanup(void) {
    if (buf1 != 0) {
        if (munmap(buf1, PAGESIZE) == -1) return;
    }
    if (buf2 != 0) {
        if (munmap(buf2, PAGESIZE) == -1) return;
    }
    if (buf3 != 0) {
        if (munmap(buf3, PAGESIZE) == -1) return;
    }
    if (buf4 != 0) {
        if (munmap(buf4, PAGESIZE) == -1) return;
    }
    return;
}
