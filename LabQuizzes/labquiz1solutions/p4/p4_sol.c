#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>

void child_process(int n) {
  usleep(5000);
  for (int idx = 1; idx <= n; ++idx) {
    printf("Child [%d]: %d\n", getpid(), idx);
    sleep(1);
  }
  exit(0);
}

int main(int argc, char* argv[]) {
  if (argc < 4) {
    printf(
        "Usage: ./sched <time-slice> <c1-execution-time> "
        "<c2-execution-time>\n");
    exit(1);
  }

  int num_children = 2;
  int time_slice = atoi(argv[1]);
  int children_exec_time[2];
  for (int idx = 2; idx < num_children + 2; ++idx) {
    children_exec_time[idx - 2] = atoi(argv[idx]);
  }

  printf("timeslice: %d, c1 execution time: %d, c2 execution time: %d\n",
         time_slice, children_exec_time[0], children_exec_time[1]);

  // ******************************
  //    YOUR SOLUTION STARTS HERE
  // ******************************
  int children[2];
  for (int idx = 0; idx < num_children; ++idx) {
    int pid;
    if ((pid = fork()) == 0) {
      child_process(children_exec_time[idx]);
    } else {
      children[idx] = pid;
      kill(children[idx], SIGSTOP);
    }
  }

  int curr = 0;

  while (1) {
    kill(children[curr], SIGCONT);
    kill(children[1 - curr], SIGSTOP);
    curr = 1 - curr;

    int p0_exited = (waitpid(children[0], NULL, WNOHANG) == children[0]);
    int p1_exited = (waitpid(children[1], NULL, WNOHANG) == children[1]);
    if (p0_exited) {
      kill(children[1], SIGCONT);
      break;
    }
    if (p1_exited) {
      kill(children[0], SIGCONT);
      break;
    }
    sleep(time_slice);
  }
  wait(NULL);

  // ******************************
  //    YOUR SOLUTION ENDS HERE
  // ******************************

  printf("Parent [%d] terminates!!\n", getpid());
}
