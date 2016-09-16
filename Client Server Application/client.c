#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h> 

void error(const char *msg)
{
    perror(msg);
    exit(0);
}

int main(int argc, char *argv[])
{
    int sockfd, portno, n;
    struct sockaddr_in serv_addr;
    struct hostent *server;

    char buffer[256];
    if (argc < 3) {
       fprintf(stderr,"USAGE %s <hostname> <port>\n", argv[0]);
       exit(0);
    }
    portno = atoi(argv[2]);
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) 
        error("ERROR opening socket");
    server = gethostbyname(argv[1]);
    if (server == NULL) {
        fprintf(stderr,"ERROR, no such host\n");
        exit(0);
    }
    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy((char *)server->h_addr, 
         (char *)&serv_addr.sin_addr.s_addr,
         server->h_length);
    serv_addr.sin_port = htons(portno);
    if (connect(sockfd,(struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0) 
        error("ERROR connecting");
    printf("Please enter the request: ");
    bzero(buffer,256);

	int count_blank_lines = 0;  
	while (1)	//send request to server
	{
		fgets(buffer,255,stdin);
	    n = write(sockfd,buffer,strlen(buffer));
		if (n < 0) 
	    	error("ERROR writing to socket");
		if (!strcmp(buffer,"\n"))
			count_blank_lines++;
		if (count_blank_lines==2)
			break;
	}
	printf("Done writing\n");
	bzero(buffer,256);
	while ((n = read(sockfd,buffer,255))>0) //read response from server
	{
		printf("%s",buffer);
		bzero(buffer,256);
	}
		close(sockfd);

    return 0;
}