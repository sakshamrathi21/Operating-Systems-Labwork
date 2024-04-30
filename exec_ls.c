#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/wait.h>
#include<sys/types.h>

int main (int argc, char *argv[]) {
    printf("hello\n");
    execl("/bin/cd", "cd", NULL);
    // printf(ret);
    printf("worldr\n");
    // int rc = fork();
    // if (rc < 0) {
    //     fprintf(stderr, "fork failed\n");
    //     exit(1);
    // }
    // else if (rc == 0) {
    //     printf("hello, I am a child (pid: %d)\n", (int) getpid());
    //     execl("/bin/ls", "ls"); 
    // }
    // else {
    //     wait(NULL);
    //     printf("hello, I am a parent of %d (pid:%d)\n", rc, (int) getpid());
    //     // execl("/bin/ls", "ls"); 
    //     // wait(NULL);
    //     // while(1);
    // }
    return 0;
}