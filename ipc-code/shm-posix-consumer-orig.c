/**
 * Simple program demonstrating shared memory in POSIX systems.
 *
 * This is the consumer process
 *
 * Figure 3.18
 *
 * @author Gagne, Galvin, Silberschatz
 * Operating System Concepts - Ninth Edition
 * Copyright John Wiley & Sons - 2013
 */

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <unistd.h>
#include <string.h>

int main()
{
	const char *name = "OS";
	const int SIZE = 4096;
	const char *message0= "freeeee";
	int NUM_ITERATIONS = 1024;

	int shm_fd;
	void *ptr;
	int i;

	/* open the shared memory segment */
	shm_fd = shm_open(name, O_RDWR, 0666);
	if (shm_fd == -1) {
		printf("shared memory failed\n");
		exit(-1);
	}

	/* now map the shared memory segment in the address space of the process */
	ptr = mmap(0,SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);
	if (ptr == MAP_FAILED) {
		printf("Map failed\n");
		exit(-1);
	}

	const char *pipe_path = "/tmp/my_named_pipe";

    int fd = open(pipe_path, O_RDONLY);
    if (fd == -1) {
        perror("open");
        exit(EXIT_FAILURE);
    }

    char buffer[256];
    
    // if (bytesRead == -1) {
    //     perror("read");
    //     exit(EXIT_FAILURE);
    // }

	/* now read from the shared memory region */
	// printf("%s", (char *)ptr);

	/* remove the shared memory segment */
	if (shm_unlink(name) == -1) {
		printf("Error removing %s\n",name);
		exit(-1);
	}
	int counter = 0;
	while(1) {
		counter++;
		ssize_t bytesRead = read(fd, buffer, sizeof(buffer));
		// printf("check\n");
		// printf(buffer);
		int offset = atoi(buffer);
		// printf("cchchc%d\n", offset);
		if (offset < 0 || offset * strlen(message0) >= SIZE) {
			printf("Invalid offset\n");
		}

		void* current_ptr = ptr + offset * (strlen(message0)+1);
		
		printf("The message read at the current ptr is %s\n", (char *)current_ptr);
		// printf("hello\n");
		sprintf(current_ptr,"%s",message0);
		// printf("hello\n");
		usleep(10000);
		// printf("%d\n", counter);
		if (counter >= NUM_ITERATIONS-3) break;
	}
	close(fd);
	return 0;
}
