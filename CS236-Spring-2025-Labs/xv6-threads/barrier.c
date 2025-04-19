/*----------xv6 sync lab----------*/
#include "types.h"
#include "x86.h"
#include "spinlock.h"
#include "defs.h"
#include "barrier.h"

//define any variables needed here
struct spinlock lock;
int glbl; // used as wakeup channel
int cur_num;
int tot_num;

int
barrier_init(int n)
{
  cur_num = 0;
  tot_num = n;
  initlock(&lock, "barrier");
  return 0;
}

int
barrier_check(void)
{
  acquire(&lock);
  cur_num++;
  if(cur_num < tot_num){
    sleep(&glbl, &lock);
  }
  else{
    wakeup(&glbl);
  }
  cur_num--;
  release(&lock);
  return 0;
}

/*----------xv6 sync lock end----------*/
