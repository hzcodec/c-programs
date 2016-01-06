/*
    Auther      : Heinz Samuelsson
    Date        : Wed Jan  6 23:00:06 CET 2016
    File        : client1.c
    Reference   : -
    Description : client1 is communicating with server1.
                  How to use:
                    Start server - ./server1
                    Start client - ./client1 <ip address>
                  It's also possible to use telnet.
                    telnet <ip-address> 3490
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define PORT    3490
#define MAXSIZE 1024

int main(int argc, char *argv[])
{
    struct sockaddr_in server_info;
    struct hostent *he;
    int    socket_fd,num;
    char   buffer[1024];
    char   buff[1024];

    if (argc != 2)
    {
        fprintf(stderr, "Usage: client hostname\n");
        exit(1);
    }

    if ((he = gethostbyname(argv[1])) == NULL)
    {
        fprintf(stderr, "Cannot get host name\n");
        exit(1);
    }

    if ((socket_fd = socket(AF_INET, SOCK_STREAM, 0))== -1)
    {
        fprintf(stderr, "Socket Failure!!\n");
        exit(1);
    }

    memset(&server_info, 0, sizeof(server_info));
    server_info.sin_family = AF_INET;
    server_info.sin_port   = htons(PORT);
    server_info.sin_addr   = *((struct in_addr *)he->h_addr);

    if (connect(socket_fd, (struct sockaddr *)&server_info, sizeof(struct sockaddr))<0)
      {
        perror("connect");
        exit(1);
    }

    while(1)
    {
        printf("Client: Enter Data for Server: ");
        fgets(buffer,MAXSIZE-1,stdin);

        if ((send(socket_fd,buffer, strlen(buffer), 0))== -1)
        {
                fprintf(stderr, "Failure Sending Message\n");
                close(socket_fd);
                exit(1);
        }

        else
        {
                printf("Client:Message being sent: %s\n", buffer);
                num = recv(socket_fd, buffer, sizeof(buffer), 0);
                if (num <= 0)
                {
                    printf("Either Connection Closed or Error\n");
                    //Break from the while loop
                    break;
                }

                buff[num] = '\0';
                printf("Client:Message Received From Server -  %s\n", buffer);
         }
    }

    close(socket_fd);
}
