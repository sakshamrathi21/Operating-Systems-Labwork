#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include<string.h>

int main() {
    const char *pipe_path = "/tmp/my_named_pipe";

    int fd = open(pipe_path, O_RDONLY);
    if (fd == -1) {
        perror("open");
        exit(EXIT_FAILURE);
    }

    char buffer[256];
    ssize_t bytesRead = read(fd, buffer, sizeof(buffer));
    if (bytesRead == -1) {
        perror("read");
        exit(EXIT_FAILURE);
    }

    printf("Received message: %s\n", buffer);

    close(fd);

    return EXIT_SUCCESS;
}
