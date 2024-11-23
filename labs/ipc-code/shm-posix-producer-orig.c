/**
 * Simple program demonstrating shared memory in POSIX systems.
 *
 * This is the producer process that writes to the shared memory region.
 *
 * Figure 3.17
 *
 * @author Silberschatz, Galvin, and Gagne
 * Operating System Concepts  - Ninth Edition
 * Copyright John Wiley & Sons - 2013
 */

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include<string.h>

int main()
{
	const int SIZE = 4096;
	const char *name = "OS";
	const char *message0= "freeeee";
	const char *message1= "OSisFUN";
	int NUM_ITERATIONS = 1024;

	int shm_fd;
	void *ptr;

	/* create the shared memory segment */
	shm_fd = shm_open(name, O_CREAT | O_RDWR, 0666);

	/* configure the size of the shared memory segment */
	ftruncate(shm_fd,SIZE);

	/* now map the shared memory segment in the address space of the process */
	ptr = mmap(0,SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);
	if (ptr == MAP_FAILED) {
		printf("Map failed\n");
		return -1;
	}

	const char *pipe_path = "/tmp/my_named_pipe";

    mkfifo(pipe_path, 0666);

    int fd = open(pipe_path, O_WRONLY);
    if (fd == -1) {
        perror("open");
        exit(EXIT_FAILURE);
    }

	/**
	 * Now write to the shared memory region.
 	 *
	 * Note we must increment the value of ptr after each write.
	 */
	for (int i = 0 ; i < 512 ; i ++ ) {
		sprintf(ptr,"%s",message0);
		ptr += strlen(message0);
	}
	ptr -= 512*strlen(message0);
	
	
	for (int i = 0 ; i < NUM_ITERATIONS ; i ++ ) {
		if (i == 512) {
			ptr -= 512*(strlen(message0)+1);
			i -=512;
			NUM_ITERATIONS -= 512;
		}
		// printf("ch\n");
		sprintf(ptr,"%s",message1);
		char str[20];
		sprintf(str, "%d", i);
		usleep(10000);
		write(fd, str, strlen(str)+1);
		ptr += (strlen(message1)+1);
	}
	sleep(20);
	close(fd);

	return 0;
}
