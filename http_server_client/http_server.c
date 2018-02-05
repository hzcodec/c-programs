/*
    Auther       : Heinz Samuelsson
    Date         : 2018-02-02
    File         : http_server.c
    Reference    : http://www.theinsanetechie.in/2014/02/a-simple-http-client-and-server-in-c.html
    Description  : 
                     gcc http_server.c -o server 
                     ./server 'www.dhanoop.com' '/home/dhanoopbhaskar/coding/http/dir/' 4444
                     ./server 'www.dhanoop.com' '/home/heinz.samuelsson/dhanoopbhaskar/coding/http/dir/' 4444
                     ./server 'www.dhanoop.com' '/home/heinz.samuelsson/shitdir/http/dir/' 4444

                   NB: The requested files abc.txt, abc.html and abc.pdf should be present in 
		   the path '/home/dhanoopbhaskar/coding/http/dir/' 
		   (or whatever is given while running server).

		   To check if a port is used or not in Linux.
		       > netstat -ltun

		   Or use nmap (this will kill the server)
		       > nmap 10.239.181.182

		   Or
		       > netstat -a | grep 4444
*/ 
 
#include"stdio.h"
#include"stdlib.h"
#include"sys/types.h"
#include"sys/socket.h"
#include"string.h"
#include"netinet/in.h"
#include"time.h"
#include"dirent.h"
#include"netdb.h"
#include <unistd.h>     // close()
#include <arpa/inet.h>

#define BUF_SIZE 1024
#define CLADDR_LEN 100

int createSocket(char * host, int port);
int listenForRequest(int sockfd);
char * getFileType(char * file);


int main(int argc, char **argv) { 

        DIR * dirptr;
        FILE * fileptr;
        time_t timenow;
        struct tm * timeinfo;
        time (&timenow);
        timeinfo = localtime(&timenow);
       
        char * header, * request, * path, * newpath, * host;
        char * dir, * temp;
        int port, sockfd, connfd;
        char get[3], http[9];
        char filepath[BUF_SIZE];
        char http_not_found[] = "HTTP/1.0 404 Not Found\n";
        char http_ok[] = "HTTP/1.0 200 OK\n";
        char buffer[BUF_SIZE];
        char * contentType;
       
        if (argc != 4) {
            printf("usage: [host] [directory] [portnumber]\n");
            exit(1);
        }
       
        header = (char*)malloc(BUF_SIZE*sizeof(char));
        request = (char*)malloc(BUF_SIZE*sizeof(char));
        path = (char*)malloc(BUF_SIZE*sizeof(char));
        newpath = (char*)malloc(BUF_SIZE*sizeof(char));
       
        host = argv[1];
        dir = argv[2];
        port = atoi(argv[3]);
	printf("%s() [%d] - host: %s, dir: %s, port=%d\n", __func__, __LINE__, host, dir, port);
       
        if ((dirptr = opendir(dir)) == NULL) {
             printf("Directory Not Found!\n");
             exit(1);
        }
        
        sockfd = createSocket(host, port);
        
        for (;;) {
    
            printf("%s() - Waiting for a connection...\n", __func__);

            connfd = listenForRequest(sockfd);
    
            //gets the request from the connection
            recv(connfd, request, 100, 0);
            printf("%s() [%d] - Processing request...\n", __func__, __LINE__);
    
            //parses request
            sscanf(request, "%s %s %s", get, path, http);
            newpath = path + 1; //ignores the first slash
            sprintf(filepath,"%s/%s", dir, newpath);

            contentType = getFileType(newpath);
            sprintf(header, "Date: %sHostname: %s:%d\nLocation: %s\nContent-Type: %s\n\n", asctime(timeinfo), host, port, newpath, contentType);

	    printf("%s() [%d] - filepath=%s, dir=%s, newpath=%s\n", __func__, __LINE__, filepath, dir, newpath);
    
            if ((fileptr = fopen(filepath, "r")) == NULL ) {
                    printf("%s() [%d] - File not found!\n", __func__, __LINE__);
                    send(connfd, http_not_found, strlen(http_not_found), 0); //sends HTTP 404

            } else {
                    printf("%s() [%d] - Sending the file...\n", __func__, __LINE__);
                    send(connfd, http_ok, strlen(http_ok), 0); //sends HTTP 200 OK  
                    recv(connfd, buffer, BUF_SIZE, 0);
    
             if ((temp = strstr(buffer, "OK")) == NULL) {
                     printf("Operation aborted by the user!\n");
                     break;
             }

             send(connfd, header, strlen(header), 0); //sends the header
             recv(connfd, buffer, BUF_SIZE, 0);
    
             if ((temp = strstr(buffer, "OK")) == NULL) {
                     printf("Operation aborted by the user!\n");
                     break;
             }
    
             memset(&buffer, 0, sizeof(buffer));

             while (!feof(fileptr)) { //sends the file
                     fread(&buffer, sizeof(buffer), 1, fileptr);
                     send(connfd, buffer, sizeof(buffer), 0);
                     memset(&buffer, 0, sizeof(buffer));
             }
    
            printf("File sent...\n");
            }
    
            printf("Processing completed...\n");
            close(connfd);
        }
       
        close(sockfd);
        free(header);
        free(request);
        free(path);
        free(newpath);
       
        return 0;
}


int createSocket(char * host, int port) {
        int sockfd;
        struct sockaddr_in addr;
        struct hostent * host_ent;
        char * hostAddr;
       
        memset(&addr, 0, sizeof(addr));
        addr.sin_family = AF_INET;
        addr.sin_addr.s_addr = INADDR_ANY;
        addr.sin_port = htons((short)port);
       
        sockfd = socket(AF_INET, SOCK_STREAM, 0);

        if (sockfd < 0) {  
                printf("Error creating socket!\n");  
                exit(1);  
        }  

        printf("Socket created...\n");
       
        if (bind(sockfd, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
                printf("Error binding socket to port!\n");  
                exit(1);
        }

        printf("Binding done...\n");
       
        return sockfd;
}

int listenForRequest(int sockfd) {
        int conn;
        char hostip[32];
        struct sockaddr_in addr;
        struct hostent * host;
        struct in_addr inAddr;
        int len;
       
        addr.sin_family = AF_INET;
       
        listen(sockfd, 5); //maximum 5 connections
        len = sizeof(addr); 
	printf("%s() [%d] - len=%d\n", __func__, __LINE__, len);
        printf("--------------------------------------------------------\n");
    
        if ((conn = accept(sockfd, (struct sockaddr *)&addr, &len)) < 0) {
                printf("%s() - Error accepting connection!\n", __func__);
                exit(1);
        }
    
        printf("  %s() - Connection accepted...\n", __func__);
         
	// convert IPv4 and IPv6 addresses from binary to text form
        inet_ntop(AF_INET, &(addr.sin_addr), hostip, 32);
	printf("  %s() - hostip=%s\n", __func__, hostip);

        // convert IPv4 and IPv6 addresses from text to binary form
        inet_pton(AF_INET, hostip, &inAddr);
	printf("  %s() - inAddr=%x\n", __func__, inAddr.s_addr);

        // HzS host = gethostbyaddr(&inAddr, sizeof(inAddr), AF_INET);
	// HzS printf("%s() - host:%s\n", __func__, host->h_name);
       
        //printf("---Connection received from: %s [IP= %s]---\n", host->h_name, hostip);
        printf("  %s() - -Connection received:\n", __func__);
    
        return conn;
}

char * getFileType(char * file) {
        char * temp;

	printf("%s() [%d] - file=%s\n", __func__, __LINE__, file);
    
        if ((temp = strstr(file, ".html")) != NULL) {
                return "text/html";
    
        } else if ((temp = strstr(file, ".pdf")) != NULL) {
                return "application/pdf";
    
        } else if ((temp = strstr(file, ".txt")) != NULL) {
                return "text/html";
        }
}
