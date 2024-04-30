#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <iostream>

using namespace std;

struct read_write_lock
{
    pthread_mutex_t common_lock;
    pthread_mutex_t rlock;   //Lock to be used by readers.
    pthread_mutex_t wlock;
    int nread;              //Number of readers waiting
    int nwrite;             //Number of writers waiting
    pthread_cond_t c_read;
    pthread_cond_t c_write;
};

void InitalizeReadWriteLock(struct read_write_lock * rw);
void ReaderLock(struct read_write_lock * rw);
void ReaderUnlock(struct read_write_lock * rw);
void WriterLock(struct read_write_lock * rw);
void WriterUnlock(struct read_write_lock * rw);
