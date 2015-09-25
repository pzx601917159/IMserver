#include <stdio.h>
#include <sys/socket.h>
#include <sys/poll.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>
#include <string.h>
#include <stdarg.h>
#include <stdlib.h>

#define MAXLINE 5

void useage(char* arg)
{
    fprintf(stderr,"%s [ip] [port] \n",arg);
}

int main(int argc, char **argv)
{
    if(argc<3)
    {
        useage(argv[0]);
        return 0;
    }
    int sockfd;
    char recvline[MAXLINE + 1];
    struct sockaddr_in servaddr;
 
 
    if ( (sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        printf("socket error\n");
        return 0;
    }

    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port   = htons(uint16_t(atoi(argv[2])));
    if (inet_pton(AF_INET, argv[1], &servaddr.sin_addr) <= 0)
    {
        printf("inet_pton error for %s\n", argv[1]);
        return 0;
    }
 
    if (connect(sockfd, (sockaddr *) &servaddr, sizeof(servaddr)) < 0)
    {
        printf("connect error\n");
        return 0;
    }
 
    char input[100]="pengzhixiang hahaha!\n";
    //while (fgets(input, 100, stdin) != EOF)
    //{
        write(sockfd, input, strlen(input));
 
        int n = 0;
        int count = 0;
        while (1)
        {
            n = read(sockfd, recvline + count, MAXLINE);
            if (n == MAXLINE)
            {
                count += n;
                continue;
            }
            else 
                break;
        }
         printf("%s\n", recvline);
    //}
    return 0;
}
