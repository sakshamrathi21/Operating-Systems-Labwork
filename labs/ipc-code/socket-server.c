#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <sys/un.h>
#include <sys/stat.h>

#define SOCK_PATH "unix_socket_example"

void error(char *msg)
{
    perror(msg);
    exit(1);
}

int main(int argc, char *argv[])
{
     int sockfd;
     char buffer[256];
     struct sockaddr_un serv_addr, cli_addr;
     int n;

     /* create socket */

     sockfd = socket(AF_UNIX, SOCK_DGRAM, 0);
     if (sockfd < 0) 
        error("ERROR opening socket");

     /* fill in socket addres */
     bzero((char *) &serv_addr, sizeof(serv_addr));
     serv_addr.sun_family = AF_UNIX;
     strcpy(serv_addr.sun_path, SOCK_PATH);

     /* bind socket to this address */
     if (bind(sockfd, (struct sockaddr *) &serv_addr,
              sizeof(serv_addr)) < 0) 
              error("ERROR on binding");
     
     /* read message from client */

     bzero(buffer,256);
     int len = sizeof(cli_addr);

     printf("Server ready\n");
     int p;
     read(sockfd, &p, sizeof(p));
    //  n = recvfrom(sockfd, &p, 255, 0, (struct sockaddr *)&cli_addr, &len);
     int total_size = p;
     int counter = 0;
    //  printf("che%d\n", total_size);
     while (n = recvfrom(sockfd, buffer, 256, 0, (struct sockaddr *)&cli_addr, &len)) {
        // Process or save the received data as needed
        // In this example, we print the received data
        // printf("cdciehcief%d\n", n);
        counter += n;
        printf("%s\n",buffer);
        memset(buffer, '\0', 256*sizeof(char));
        
        if (counter >= total_size) break;
        
    }
    // printf("hello\n");
    //  n = recvfrom(sockfd, buffer, 255, 0, (struct sockaddr *)&cli_addr, &len);
    //  printf("hello\n");
    //  printf("%s\n",buffer);
     
     if (n < 0) error("ERROR reading from socket");
     

     unlink(SOCK_PATH);
     return 0; 
}
