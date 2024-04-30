#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/wait.h>
#include<sys/types.h>

int main (int argc, char *argv[]) {
    
    int N = 1;
    for (int i = 0 ; i < N ; i ++ ) {
        int rc = fork();
        if (rc < 0) {
            fprintf(stderr, "fork failed\n");
            exit(1);
        }
        else if (rc == 0) {
            sleep(5);
            printf("hello, I am a child (pid: %d)\n", (int) getpid());
        }
        else {
            wait(NULL);
            printf("hello, I am a parent of %d (pid:%d)\n", rc, (int) getpid());
            // wait(NULL);
            // while(1);
        }
    }
    
    return 0;
}