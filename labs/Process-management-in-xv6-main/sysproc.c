#include "types.h"
#include "x86.h"
#include "defs.h"
#include "date.h"
#include "param.h"
#include "memlayout.h"
#include "mmu.h"
#include "proc.h"

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

int sys_hello(void){
  cprintf("hello\n");
  return 0;
}

int sys_helloYou(void){
  int ptr;
  if(argint(0, &ptr) < 0){
    return -1;
  }
  char* name = (char*)ptr;
  cprintf("%s\n", name);
  return 0;
}

int sys_getNumProc(){
  return getnumprocs();
}

int sys_getMaxPid(){
  return getmaxPID();
}

int sys_getProcInfo(){
  int pid;
  if(argint(0, &pid) < 0){
    return -1;
  }
  int ptr;
  if(argint(1, &ptr) < 0){
    return -1;
  }
  struct processInfo* procinfo = (struct processInfo*)ptr;
  return getprocinfo(pid, procinfo);
}

int sys_welcomeFunction(){
  int ptr;
  if(argint(0, &ptr) < 0){
    return -1;
  }
  char* addr = (char*)ptr;
  myproc()->welc_addr = addr;
  return 0;
}

int sys_welcomeDone(){
  struct proc* curproc = myproc();
  if(curproc->ret_addr){
    curproc->tf->eip = (uint)(curproc->ret_addr);
    return 0;
  }
  return -1;
}

int sys_setprio(){
  int n;
  if(argint(0, &n) < 0){
    return -1;
  }
  myproc()->prio = n;
  return 0;
}

int sys_getprio(){
  return myproc()->prio;
}