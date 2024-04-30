#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <sys/stat.h>
#include <fcntl.h>

#define SOCK_PATH "unix_socket_example"

void error(char *msg)
{
    perror(msg);
    exit(0);
}

int main(int argc, char *argv[])
{
    int sockfd, portno, n;
    if (argc < 2) {
        printf("Please give the file path.\n");
    }
    struct sockaddr_un serv_addr;
    char buffer[256];
    char end[256];
    /* create socket, get sockfd handle */
    sockfd = socket(AF_UNIX, SOCK_DGRAM, 0);
    if (sockfd < 0) 
        error("ERROR opening socket");

    /* fill in server address */
    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sun_family = AF_UNIX;
    strcpy(serv_addr.sun_path, SOCK_PATH); 


     int filefd = open(argv[1], O_RDONLY);
    if (filefd < 0)
        error("ERROR opening file");
    struct stat fileStat;
    if (fstat(filefd, &fileStat) == -1) {
        perror("Error getting file status");
        close(filefd);
        return 3;
    }

    off_t fileSize = fileStat.st_size;
    int fileS = (int)fileSize;
    ssize_t bytesRead;
    // if (bytesRead < 0)
    //     error("ERROR reading from file");

    /* connect to the server */
    if (connect(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0)
        error("ERROR connecting to server");

    /* send file data to the server */
    printf("Sending data...\n");
    n = write(sockfd, &fileS, sizeof(fileS));
    // printf("check file size %d\n", fileSize);
    while ((bytesRead = read(filefd, buffer, sizeof(buffer))) > 0) {
        // printf(buffer);
        n = write(sockfd, buffer, bytesRead);
        // printf("%d\n", n);
        if (n < 0)
            error("ERROR writing to socket");
        memset(buffer, '\0', 256*sizeof(char));
    }

    if (n < 0)
        error("ERROR writing to socket");
    // end = "The file read is complete";
    // n = write(sockfd, end, )
    close(filefd);
    close(sockfd);
    return 0;
}
