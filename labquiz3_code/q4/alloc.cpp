#include <stdio.h>
#include <string.h>
#include <sys/mman.h>
#include <unistd.h>
#include <stdlib.h>
#include <assert.h>
#include <map>

#include "alloc.h"

using namespace std;

char* buf = NULL;
int bit[4];
// int last_sz[4];

int init_alloc(){
    for (int i = 0 ; i < 4 ; i ++ ) {
        bit[i] = 0;
        // last_sz[i] = 0;
    }
    if ((buf = (char*)mmap(NULL, 4096, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0)) == MAP_FAILED) return -1;
    return 0;
}

int cleanup(){
    if (munmap(buf, 4096) == -1) return -1;
    return 0;
}

char *alloc(int size){
    if (size > 4096 || size%1024 != 0) return (char*)0;
    if (size == 1024) {
        for (int i = 0 ; i < 4 ; i ++ ) {
            if (!bit[i]) {
                bit[i] = 1;
                // last_sz[i] = 1;
                return (buf+i*1024);}
        }
    }
    else if (size == 2048) {
        if (!bit[0] && !bit[1]) {
            bit[0] = 2;
            bit[1] = 2;
            return (buf);
        }
        if (!bit[2] && !bit[3]) {
            bit[2] = 2;
            bit[3] = 2;
            return (buf+2*1024);
        }
    }
    else if (size == 4096) {
        if (!bit[0] && !bit[1] && !bit[2] && !bit[3]) {
            bit[0] = 4;
            bit[1] = 4;
            bit[2] = 4;
            bit[3] = 4;
            return buf;
        }
    }
    return (char*)0;
}

void dealloc(char* f_mem){
    if (f_mem > (buf+4096) || f_mem < buf) return;
    int index = ((f_mem-buf)/1024);
    if (bit[index] == 4) {
        bit[0] = 0;
        bit[1] = 0;
        bit[2] = 0;
        bit[3] = 0;
        return;
    }
    if (bit[index] == 1) {
        bit[index] = 0;
        return;
    } 
    if (bit[index] == 2) {
        if (index%2 == 0) {
            bit[index] = 0;
            bit[index+1] = 0;
        }
        else {
            bit[index] = 0;
            bit[index-1] = 0;
        }
        return;
    }
    return;
}
