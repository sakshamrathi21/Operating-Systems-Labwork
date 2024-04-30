#include "rwlock.h"

void InitalizeReadWriteLock(struct read_write_lock * rw)
{
  //	Write the code for initializing your read-write lock.
  pthread_mutex_init(&(rw->common_lock), NULL);
  pthread_mutex_init(&(rw->rlock), NULL);
  pthread_mutex_init(&(rw->wlock), NULL);
  rw->nread = 0;
  rw->nwrite = 0;
  rw->c_read = PTHREAD_COND_INITIALIZER;
  rw->c_write = PTHREAD_COND_INITIALIZER;
}

void ReaderLock(struct read_write_lock * rw)
{
  //	Write the code for aquiring read-write lock by the reader.
  pthread_mutex_lock(&(rw->rlock));
  if (rw->nread == 0) pthread_mutex_lock(&(rw->common_lock));
  rw->nread ++;
  pthread_mutex_unlock(&(rw->rlock));
}

void ReaderUnlock(struct read_write_lock * rw)
{
  //	Write the code for releasing read-write lock by the reader.
  pthread_mutex_lock(&(rw->rlock));
  rw->nread --;
  if (rw->nread == 0) pthread_mutex_unlock(&(rw->common_lock));
  pthread_mutex_unlock(&(rw->rlock));
}

void WriterLock(struct read_write_lock * rw)
{
  //	Write the code for aquiring read-write lock by the writer.
  pthread_mutex_lock(&(rw->common_lock));
  // pthread_mutex_unlock(&(rw->common_lock));
}

void WriterUnlock(struct read_write_lock * rw)
{
  //	Write the code for releasing read-write lock by the writer.
  // pthread_mutex_lock(&(rw->common_lock));
  pthread_mutex_unlock(&(rw->common_lock));
}
