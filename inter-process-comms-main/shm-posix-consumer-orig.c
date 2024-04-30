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
	const int SIZE = 40;

	int shm_fd;
	void *ptr;

	const char* fifo_w = "/tmp/wfifo"; // communicate indices written into
	const char* fifo_r = "/tmp/rfifo"; // communicate indices read from
	const char* fifo_c = "/tmp/confirm"; // synchronised exit
	mkfifo(fifo_w, 0666);
	mkfifo(fifo_r, 0666);
	mkfifo(fifo_c, 0666);

	int fifo_write = open(fifo_w, O_RDONLY);	
	int fifo_read = open(fifo_r, O_WRONLY);
	int confirm = open(fifo_c, O_WRONLY);

	/* open the shared memory segment */
	shm_fd = shm_open(name, O_RDWR, 0666);
	if (shm_fd == -1) {
		printf("shared memory failed\n");
		exit(-1);
	}

	/* now map the shared memory segment in the address space of the process */
	ptr = mmap(0,SIZE, PROT_READ, MAP_SHARED, shm_fd, 0);
	if (ptr == MAP_FAILED) {
		printf("Map failed\n");
		exit(-1);
	}

	char* str = (char*)malloc(8); // to store contents read from shared memory

	// read from indices that are written into
	for(int i=0; i<1000; ++i){
		int j;
		read(fifo_write, &j, sizeof(int));
		memcpy(str, ptr+8*j, 8);
		write(fifo_read, &j, sizeof(int));
		printf("[CONSUMER] read %s from index %d\n", str, j);
		sleep(1);
	}

	write(confirm, "c", 1);

	close(fifo_write);
	close(fifo_read);
	close(confirm);

	return 0;
}
