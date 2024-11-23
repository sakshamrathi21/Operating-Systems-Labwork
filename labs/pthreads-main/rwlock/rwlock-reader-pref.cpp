#include "rwlock.h"

void InitalizeReadWriteLock(struct read_write_lock * rw)
{
  //	Write the code for initializing your read-write lock.
  pthread_mutex_init(&(rw->lock), NULL);
  pthread_mutex_init(&(rw->lock_r), NULL);
  rw->num_readers = 0;
}

void ReaderLock(struct read_write_lock * rw)
{
  //	Write the code for aquiring read-write lock by the reader.
  pthread_mutex_lock(&(rw->lock_r));
  if(rw->num_readers == 0){
    pthread_mutex_lock(&(rw->lock));
  }
  rw->num_readers++;
  pthread_mutex_unlock(&(rw->lock_r));
}

void ReaderUnlock(struct read_write_lock * rw)
{
  //	Write the code for releasing read-write lock by the reader.
  pthread_mutex_lock(&(rw->lock_r));
  if(--(rw->num_readers) == 0){
    pthread_mutex_unlock(&(rw->lock));
  }
  pthread_mutex_unlock(&(rw->lock_r));
}

void WriterLock(struct read_write_lock * rw)
{
  //	Write the code for aquiring read-write lock by the writer.
  pthread_mutex_lock(&(rw->lock));
}

void WriterUnlock(struct read_write_lock * rw)
{
  //	Write the code for releasing read-write lock by the writer.
  pthread_mutex_unlock(&(rw->lock));
}
