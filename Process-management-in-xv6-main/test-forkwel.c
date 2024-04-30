#include "types.h"
#include "stat.h"
#include "user.h"

void welcome(void)
{
  printf(1, "I am child in welcome function\n");
  welcomeDone();    // modify the fork syscall to work even in the absence of welcomeDone - push rbp and return address to the stack, the ret and pop rsp instruction should take care of returning to the right place
}

int main(int argc, char *argv[])
{

  int ret1 = fork();
  if (ret1 == 0)
  {
    printf(1, "I am child with no welcome\n");
  }
  else if (ret1 > 0)
  {
    wait();
    printf(1, "Parent reaped first child\n");

    welcomeFunction(&welcome);

    int ret2 = fork();
    if (ret2 == 0)
    {
      printf(1, "I have returned from welcome function\n");
      exit();
    }
    else if (ret2 > 0)
    {
      wait();
      printf(1, "Parent reaped second child\n");
    }
  }

  exit();
}
