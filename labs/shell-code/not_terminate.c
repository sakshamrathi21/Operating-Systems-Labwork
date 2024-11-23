#include<stdio.h>
#include<signal.h>

void sigInthandler (int sig_num) {
    signal(SIGINT, sigInthandler);
    printf("\nI will run forever \n");
    fflush(stdout);
}

int main (int argc, char *argv[]) {
    signal(SIGINT, sigInthandler);
    while(1) {

    }
    return 0;
}