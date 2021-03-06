/*
    Auther       : Heinz Samuelsson
    Date         : 2018-02-02
    File         : http_client.c
    Reference    : http://www.theinsanetechie.in/2014/02/a-simple-http-client-and-server-in-c.html
    Description  : 
                     gcc http_client.c -o client 
                     ./client '192.168.0.6/abc.txt' 4444 
                     ./client '192.168.0.6/abc.html' 4444 
                     ./client '192.168.0.6/abc.pdf' 4444

		     ./client '10.239.181.182/abc.html' 4444
*/ 

#include"stdio.h"  
#include"stdlib.h"  
#include"sys/types.h"  
#include"sys/socket.h"  
#include"string.h"  
#include"netinet/in.h"  
#include"netdb.h"
#include <unistd.h>     // close()
#include <arpa/inet.h>
  
#define BUF_SIZE 1024 
#define MAX_PORT_NUMBER 65536
#define MIN_PORT_NUMBER 0
  
int get_request(char * url, char * port);
int isValidIP(char * ip);
int parseHeader(char * header);
char * splitKeyValue(char * line, int index);
void openFile();

FILE * fileptr;
char keys[][25] = {"Date: ", "Hostname: ", "Location: ", "Content-Type: "};
char status[4] = {0, 0, 0, 0};
char contentFileType[100];
char path[1000];


int main(int argc, char**argv) {  

        struct sockaddr_in addr, cl_addr;  
        int sockfd, ret; 
        struct hostent * server;
        char * url, * temp;
        int portNumber;
        char * fileName;
        char status_ok[] = "OK";
        char buffer[BUF_SIZE]; 
        char http_not_found[] = "HTTP/1.0 404 Not Found";
        char http_ok[] = "HTTP/1.0 200 OK";
        char location[] = "Location: ";
        char contentType[] = "Content-Type: ";
        int sPos, ePos;
   
        if (argc < 3) {
                printf("usage: [URL] [port number]\n");
                exit(1);  
        }
   
        url = argv[1];
        portNumber = atoi(argv[2]);
	printf("%s() [%d] - url:%s, portNumber=%d\n", __func__, __LINE__, url, portNumber);

   
        //checking the protocol specified
        if ((temp = strstr(url, "http://")) != NULL) {
		printf("%s() - Protocol HTTP\n", __func__);
                url = url + 7;
        } else if ((temp = strstr(url, "https://")) != NULL) {
		printf("%s() - Protocol HTTPS\n", __func__);
                url = url + 8;
        }

	else {
	        printf("%s() - No protocol selected\n", __func__);
	}
   
        //checking the port number
        if (portNumber > MAX_PORT_NUMBER || portNumber < 0) {
                printf("Invalid Port Number!");
                exit(1);
        }
    
        sockfd = get_request(url, argv[2]); 
	printf("%s() [%d] - sockfd: %d\n", __func__, __LINE__, sockfd);
   
        memset(&buffer, 0, sizeof(buffer));
        ret = recv(sockfd, buffer, BUF_SIZE, 0);  

	printf("%s() [%d] - ret: %d\n", __func__, __LINE__, ret);

        if (ret < 0) {  
                printf("[A] Error receiving HTTP status!\n");    
        } 

	else {
                printf("%s() [%d] - %s\n", __func__, __LINE__, buffer);

                if ((temp = strstr(buffer, http_ok)) != NULL) {
		        printf("%s() [%d] - HTTP OK\n", __func__, __LINE__);
                        send(sockfd, status_ok, strlen(status_ok), 0);

                } else {
			printf("%s() [%d] - Close socket\n", __func__, __LINE__);
                        close(sockfd);
                        return 0;
                }
        }
   
        memset(&buffer, 0, sizeof(buffer)); 
        ret = recv(sockfd, buffer, BUF_SIZE, 0);  

        if (ret < 0) {  
                 printf("[B] Error receiving HTTP header!\n");    

        } else {

                printf("%s() [%d] - %s\n", __func__, __LINE__, buffer);

                if (parseHeader(buffer) == 0) {
                        send(sockfd, status_ok, strlen(status_ok), 0);
                } else {
                        printf("Error in HTTP header!\n");
                        close(sockfd);
                        return 0;
                }
        } 
   
        fileptr = fopen(path, "w");
        printf("%s() [%d] - file: [%s], path: %s\n", __func__, __LINE__, fileName, path);

        if (fileptr == NULL) {
                printf("Error opening file!\n");
                close(sockfd);
                return 0;
        }
   
        memset(&buffer, 0, sizeof(buffer));

        while (recv(sockfd, buffer, BUF_SIZE, 0) > 0) { //receives the file

             if ((strstr(contentFileType, "text/html")) != NULL) {
	             printf("%s() [%d] - If text/html\n", __func__, __LINE__);
                     fprintf(fileptr, "%s", buffer);
             } else {
	             printf("%s() - No text/html\n", __func__);
                     fwrite(&buffer, sizeof(buffer), 1, fileptr);
             }

             memset(&buffer, 0, sizeof(buffer));
        }
   
        fclose(fileptr);
        close(sockfd);
   
        openFile();
   
        return 0;
}


int get_request(char * url, char * port) {
   
        int sockfd, bindfd;
        char * ptr, * host;
        char getrequest[1024];
        struct sockaddr_in addr;
       
        if (isValidIP(url)) { //when an IP address is given
		printf("%s() - IP address is given\n", __func__);
                sprintf(getrequest, "GET / HTTP/1.0\nHOST: %s\n\n", url);

        } else { //when a host name is given
		printf("%s() - Host name is given\n", __func__);

                if ((ptr = strstr(url, "/")) == NULL) {
                        //when hostname does not contain a slash
			printf("%s() [%d] No slash\n", __func__, __LINE__);
                        sprintf(getrequest, "GET / HTTP/1.0\nHOST: %s\n\n", url);
                } else {
                        //when hostname contains a slash, it is a path to file
			printf("%s() [%d] Contains slash, path=%s, ptr=%s\n", __func__, __LINE__, path, ptr);
                        strcpy(path, ptr);
                        host = strtok(url, "/");
                        sprintf(getrequest, "GET %s HTTP/1.0\nHOST: %s\n\n", path, url);
                }
        } 
       
        // creates a socket to the host
        sockfd = socket(AF_INET, SOCK_STREAM, 0);
	printf("%s() [%d] - sockfd: %d\n", __func__, __LINE__, sockfd);
    
        if (sockfd < 0) {  
                printf("Error creating socket!\n");  
                exit(1);  
        }  

        printf("%s() - Socket created...\n", __func__);
       
        memset(&addr, 0, sizeof(addr));  
        addr.sin_family = AF_INET;  
        addr.sin_addr.s_addr = inet_addr(url);
        addr.sin_port = htons(atoi(port));
       
        //if (connect(sockfd, (struct sockaddr *) &addr, sizeof(addr)) < 0 ) {
        if (connect(sockfd, (struct sockaddr *)&addr, sizeof(struct sockaddr)) < 0 ) {
		perror("Connection Error ");
                printf("%s() - Connection Error!\n", __func__);
                exit(1);
        }
    
        printf("%s() - Connection successful...\n", __func__);
        ptr = strtok(path, "/");
        strcpy(path, ptr);
        printf("%s() [%d] - path=%s, ptr=%s, getrequest: %s\n\n", __func__, __LINE__, path, ptr, getrequest); 

        //fileptr = fopen(path, "w");
        //strcpy(fileName, path);
        //sprintf(fileName, "%s", path);
       
               //int optval = 1;
               //setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof optval);
       
        // writes the HTTP GET Request to the sockfd
        write(sockfd, getrequest, strlen(getrequest));
       
        return sockfd;
}


int isValidIP(char * ip) {
    struct sockaddr_in addr;

    int valid = inet_pton(AF_INET, ip, &(addr.sin_addr));
    printf("%s() [%d] - valid=%d\n", __func__, __LINE__, valid);
    return valid != 0;
}


int parseHeader(char * header) {

         //"Date: %sHostname: %s:%d\nLocation: %s\nContent-Type: %s\n\n"
         char * line, * key, * value;
         char temp[100];
         int i = 0;

	 // split string into tokens
         line = strtok(header, "\n");

	 printf("%s() [%d] - header: %s line: %s\n", __func__, __LINE__, header, line);
    
         while (line != NULL) {
                 printf("%s() [%d] - %s\n", __func__, __LINE__, line);
                 strcpy(temp, line);
                 value = splitKeyValue(line, i);  
    
                 if (i == 3) {   
                         strcpy(contentFileType, value);
                 }

                 //printf("value=%s\n", value);
                 line = strtok(NULL, "\n");
                 i++; 
         }
    
         for (i = 0; i < 4; i++) {
             if (status[i] == 0) return 1;
                     printf("%s() [%d] - status[%d]=%d\n", __func__, __LINE__, i, status[i]);
             }
    
         return 0;
}

char * splitKeyValue(char * line, int index) {

        char * temp;

        if ((temp = strstr(line, keys[index])) != NULL) {
                temp = temp + strlen(keys[index]);
                status[index] = 1;
        }

        printf("%s() [%d] - temp=%d\n", __func__, __LINE__, *temp);

        return temp;
}

void openFile() {
        char * temp;
        char command[100];
        char fileName[1000];
    
        strcpy(fileName, path);
        printf("%s() [%d] - fileName: %s, path: %s\n", __func__, __LINE__, fileName, path);
        printf("%s() [%d] - Content Type: %s\n", __func__, __LINE__, contentFileType);
    
        if ((temp = strstr(contentFileType, "text/html")) != NULL) {
    
                if ((temp = strstr(fileName, ".txt")) != NULL) {
		        printf("%s() [%d] - Text file\n", __func__, __LINE__);
                        sprintf(command, "gedit %s", fileName);
                } else {
		        printf("%s() [%d] - HTTP file\n", __func__, __LINE__);
                        sprintf(command, "firefox %s", fileName);
                }
    
                system(command);

        } else if ((temp = strstr(contentFileType, "application/pdf")) != NULL) {
		printf("%s() [%d] - PDF file\n", __func__, __LINE__);
                sprintf(command, "acroread %s", fileName);
                system(command);
    
        } else {
                printf("The filetype %s is not supported. Failed to open %s!\n", contentFileType, fileName);
        }
}
