#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/wait.h>

int check_defunct() {
	int parent_pid = getpid();
	int status;
	char parent_pid_str[20]; // Allocate enough space for the string
	snprintf(parent_pid_str, sizeof(parent_pid_str), "%d", parent_pid);
	
	int monitor_pid = fork();
	if(monitor_pid == 0) {
		char command[100];
		sprintf(command, "ps --ppid %s | grep -c '<defunct>'", parent_pid_str);
		FILE *pipe = popen(command, "r");
        if (pipe == NULL) {
            perror("popen");
            exit(EXIT_FAILURE);
        }

        char result[10];  // Buffer to store the result of grep -c
        fgets(result, sizeof(result), pipe);  // Read the result
        pclose(pipe);  // Close the pipe

        printf("Number of children not reaped yet: %s", result);
	}
	waitpid(monitor_pid, &status, 0);
}

int main(int argc, char* argv[]) {
	int n;
	FILE* fptr;
	fptr = fopen(argv[1], "r");
	fscanf(fptr, "%d", &n);
	int A[n];
	for(int i=0;i<n;i++) {
		fscanf(fptr, "%d", &A[i]);
	}

	// ===========================================================
	// BEGIN YOUR CODE
	// ===========================================================

	int flag = 1;

	int pid = fork();
	if(pid != 0) {
		flag = 0;
		pid = fork();
	}

	if(pid == 0) {
		int sum=0;
		for(int i=0;i<n;i++) {
			if(i%2 == 0 && flag == 1 || i%2 == 1 && flag == 0)
			sum += A[i];
		}
		printf("pid=%d, ppid=%d, sum=%d\n", getpid(), getppid(), sum);
		exit(0);
	}

	if(pid != 0) {
		pid = wait(NULL);
		printf("pid=%d, pid_reaped=%d\n", getpid(), pid);
		pid = wait(NULL);
		printf("pid=%d, pid_reaped=%d\n", getpid(), pid);
	}

	// =============================================================
	// END OF YOUR CODE
	// CODE PRESENT AFTER THIS IS FOR PARENT PROCESS ONLY
	// =============================================================
	
	fflush(stdout);
	check_defunct();
	return 0;
}
