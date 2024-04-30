#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <sys/stat.h>

int main() {
    int fd = open(__FILE__, O_RDONLY);  // __FILE__ is a predefined macro containing the current file's name
    if (fd == -1) {
        perror("Error opening file");
        return 1;
    }

    char *buf = mmap(NULL, 4096000, PROT_READ, MAP_PRIVATE, fd, 0);
    if (buf == MAP_FAILED) {
        perror("Error mapping file");
        close(fd);
        return 1;
    }
    // printf("hello\n");

    // char check = *(buf + 0);

    // Accessing the mapped memory here...

    // if (munmap(buf, 4096) == -1) {
    //     perror("Error unmapping file");
    // }

    close(fd);
    sleep(200);
    return 0;
    
    // 407971472    944
    // 407975456    944
    // 407976496    992
    // 407975472    944
}
