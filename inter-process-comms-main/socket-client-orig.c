#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>

#define SOCK_PATH "unix_socket_example"

void error(char *msg)
{
    perror(msg);
    exit(0);
}

int main(int argc, char *argv[])
{
    int sockfd, portno, n;

    struct sockaddr_un serv_addr;
    char buffer[256];

    /* create socket, get sockfd handle */
    sockfd = socket(AF_UNIX, SOCK_DGRAM, 0);
    if (sockfd < 0) 
        error("ERROR opening socket");

    /* fill in server address */
    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sun_family = AF_UNIX;
    strcpy(serv_addr.sun_path, SOCK_PATH); 

    bzero(buffer,256);

	// open test.txt and count number of bytes
	FILE *fp;
	fp = fopen("test.txt", "r");
	if (fp == NULL) {
		printf("Error opening file\n");
		exit(1);
	}
	fseek(fp, 0, SEEK_END);
	int file_size = ftell(fp);
	fclose(fp);

	sendto(sockfd, &file_size, sizeof(file_size), 0, (struct sockaddr *)&serv_addr, sizeof(serv_addr));

	fp = fopen("test.txt", "r");
	while (fgets(buffer, 256, fp) != NULL) {
		sendto(sockfd, buffer, strlen(buffer), 0, (struct sockaddr *)&serv_addr, sizeof(serv_addr));
	}
	fclose(fp);

    close(sockfd);
    return 0;
}