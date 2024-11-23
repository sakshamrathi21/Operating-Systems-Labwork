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

int main()
{
	const int SIZE = 4096;
	const char *name = "OS";

	const char* free_str = "free";
	const char* OS_str = "OSisFUN";

	const char* fifo_w = "/tmp/wfifo"; // to communicate index written into
	const char* fifo_r = "/tmp/rfifo"; // to communicate index read from
	const char* fifo_c = "/tmp/confirm"; // to syncronise at the end

	mkfifo(fifo_w, 0666);
	mkfifo(fifo_r, 0666);
	mkfifo(fifo_c, 0666);

	int fifo_write = open(fifo_w, O_WRONLY);
	int fifo_read = open(fifo_r, O_RDONLY);
	int confirm = open(fifo_c, O_RDONLY);
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

	/**
	 * Now write to the shared memory region.
 	 *
	 * Note we must increment the value of ptr after each write.
	 */

	// filling up mapped memory with "freeeee"
	for(int i=0; i<SIZE/8; ++i){
		memcpy(ptr+i*8, free_str, strlen(free_str)+1);
	}

	printf("[PRODUCER] initial free written\n");

	// writing "OSisFUN"
	for(int i=0; i<SIZE/8; ++i){
		memcpy(ptr+i*8, OS_str, strlen(OS_str)+1);
		write(fifo_write, &i, sizeof(int));
		printf("[PRODUCER] written at index %d\n", i);
	}

	printf("[PRODUCER] initial OS written\n");

	// writing into indices that are read by consumer
	for(int i=0; i<1000-SIZE/8; ++i){
		int j;
		read(fifo_read, &j, sizeof(int));
		memcpy(ptr+j*8, OS_str, strlen(OS_str)+1);
		write(fifo_write, &j, sizeof(int));
		printf("[PRODUCER] written at index %d\n", j);
	}

	char c;
	read(confirm, &c, 1); // exiting synchronously

	close(fifo_write);
	close(fifo_read);
	close(confirm);

	return 0;
}
