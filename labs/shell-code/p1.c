#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/wait.h>
#include<sys/types.h>
#include<signal.h>
#define _POSIX_SOURCE

int main (int argc, char *argv[]) {
    int rc = fork();
    if (rc < 0) {
        fprintf(stderr, "fork failed\n");
        exit(1);
    }
    else if (rc == 0) {
        // sleep(10000000);
        printf("hello, I am a child (pid: %d)\n", (int) getpid());
        char* myargs[3];
        myargs[0] = "sleep";
        myargs[1] = "1000";
        myargs[2] = NULL;
        execvp("sleep", myargs);
        
    }
    else {
        // wait(NULL);
        printf("hello, I am a parent of %d (pid:%d)\n", rc, (int) getpid());
        sleep(1);
        int check = kill(rc, SIGTERM);
        if (check == 0) {
            printf("Child Process Terminated\n");
            exit(1);
        }
        // execvp("ps", "");
        printf("hello, I am a parent of %d (pid:%d)\n", rc, (int) getpid());
        printf("%d", check);
        // wait(NULL);
        // while(1);
    }
    return 0;
}