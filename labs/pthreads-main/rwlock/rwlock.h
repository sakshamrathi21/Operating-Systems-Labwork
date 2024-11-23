#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <iostream>
#include <stdbool.h>

using namespace std;

struct read_write_lock
{
    bool is_reader_pref;
    int num_readers;
    int num_writers;
    pthread_mutex_t lock;
    pthread_mutex_t lock_r;
    pthread_mutex_t lock_w;
    pthread_cond_t cond_r;
    pthread_cond_t cond_w;
};

void InitalizeReadWriteLock(struct read_write_lock * rw);
void ReaderLock(struct read_write_lock * rw);
void ReaderUnlock(struct read_write_lock * rw);
void WriterLock(struct read_write_lock * rw);
void WriterUnlock(struct read_write_lock * rw);
