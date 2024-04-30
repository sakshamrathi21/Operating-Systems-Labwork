#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include<string.h>

int main() {
    const char *pipe_path = "/tmp/my_named_pipe";

    mkfifo(pipe_path, 0666);

    int fd = open(pipe_path, O_WRONLY);
    if (fd == -1) {
        perror("open");
        exit(EXIT_FAILURE);
    }

    const char *message = "Hello from Process 1!";
    write(fd, message, strlen(message) + 1);

    close(fd);

    return EXIT_SUCCESS;
}
