#include "types.h"
#include "x86.h"
#include "defs.h"
#include "date.h"
#include "param.h"
#include "memlayout.h"
#include "mmu.h"
#include "proc.h"
/////////////////// New addition ///////////////////////
#include "barrier.h"
/////////////////// End of new addition ///////////////////////

int
sys_fork(void)
{
  return fork();
}

int
sys_exit(void)
{
  exit();
  return 0;  // not reached
}

int
sys_wait(void)
{
  return wait();
}

int
sys_kill(void)
{
  int pid;

  if(argint(0, &pid) < 0)
    return -1;
  return kill(pid);
}

int
sys_getpid(void)
{
  return myproc()->pid;
}

int
sys_sbrk(void)
{
  int addr;
  int n;

  if(argint(0, &n) < 0)
    return -1;
  addr = myproc()->sz;
  if(growproc(n) < 0)
    return -1;
  return addr;
}

int
sys_sleep(void)
{
  int n;
  uint ticks0;

  if(argint(0, &n) < 0)
    return -1;
  acquire(&tickslock);
  ticks0 = ticks;
  while(ticks - ticks0 < n){
    if(myproc()->killed){
      release(&tickslock);
      return -1;
    }
    sleep(&ticks, &tickslock);
  }
  release(&tickslock);
  return 0;
}

// return how many clock tick interrupts have occurred
// since start.
int
sys_uptime(void)
{
  uint xticks;

  acquire(&tickslock);
  xticks = ticks;
  release(&tickslock);
  return xticks;
}

/////////////////// New addition ///////////////////////
int sys_thread_create(void){
  uint* thread;
  void* start_routine;
  void* arg;
  argptr(0, (char**)&thread, sizeof(uint));
  int tmp;
  argint(1, &tmp);
  start_routine = (void*)tmp;
  argint(2, &tmp);
  arg = (void*)tmp;
  return thread_create(thread, start_routine, arg);
}

int sys_thread_exit(void){
  thread_exit();
  return 0;
}

int sys_thread_join(void){
  uint thread;
  argint(0, (int*)&thread);
  return thread_join(thread);
}

int sys_barrier_init(void)
{
  int variable;
  if(argint(0, &variable) < 0){
    cprintf("Kernel: System call returning -1\n");
    return -1;
  }
  return barrier_init(variable);
}

int sys_barrier_check(void)
{
  return barrier_check();
}

int sys_waitpid(void)
{
  int pid;

  if(argint(0, &pid) < 0)
    return -1;
  return waitpid(pid);
}

////////////////// End of new addition /////////////////
/////////// Parts D and E of threads lab/////////
int sys_sleepChan(void) {
  int channel;
  if (argint(0,&channel) < 0) {
    return -1;
  }
  return sleepChan(channel);
}

int sys_getChannel(void) {
  return getChannel();
}

int sys_sigChan(void) {
  int channel;
  if (argint(0,&channel) < 0) {
    return -1;
  }
  return sigChan(channel);
}

int sys_sigOneChan(void) {
  int channel;
  if (argint(0,&channel) < 0) {
    return -1;
  }
  return sigOneChan(channel);
}
/////////// End of Parts D and E of threads lab/////////

