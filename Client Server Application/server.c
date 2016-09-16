/* A simple server in the internet domain using TCP
   The port number is passed as an argument */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/stat.h>
#include <time.h>

#define DOCUMENT_ROOT "/home/martin/server"

#define BUFFER_SIZE  1024

void handleClient(int);
void getFileName(char *, char *);
void sendResponse(int isError, char * fileName, int clientSocket);

// Get the size of a file in bytes
long flength(char *); 

void error(const char *msg)
{
    perror(msg);
    exit(1);
}

int main(int argc, char *argv[])
{
	//Takes a port to listen to as an argument
     int sockfd, newsockfd, portno;
     socklen_t clilen;
     pid_t pid;
     struct sockaddr_in serv_addr, cli_addr;
     if (argc < 2) {
         fprintf(stderr,"ERROR, no port provided\n");
         fprintf(stderr,"USAGE: %s <portNo>\n",argv[0]);
         exit(1);
     }
     sockfd = socket(AF_INET, SOCK_STREAM, 0); //create a socket
     if (sockfd < 0) 
        error("ERROR opening socket");
     bzero((char *) &serv_addr, sizeof(serv_addr)); //Clear the contents of serv_addr
     portno = atoi(argv[1]);	// read the port from the command line argument
     serv_addr.sin_family = AF_INET;
     serv_addr.sin_addr.s_addr = INADDR_ANY;
     serv_addr.sin_port = htons(portno);
     if (bind(sockfd, (struct sockaddr *) &serv_addr, 
              sizeof(serv_addr)) < 0) 
              error("ERROR on binding"); //bind the socket
     listen(sockfd,5);	//listen for incoming connections (5 in a queue)
     clilen = sizeof(cli_addr);
     while (1)
     {
		 newsockfd = accept(sockfd, 
		             (struct sockaddr *) &cli_addr, 
		             &clilen); //A connection comes in
		 if (newsockfd < 0) 
		      error("ERROR on accept");
		 if ((pid=fork())<0)	//Fork to handle it (concurrent server)
		 {
		 	perror("Fork failed");
		 	close(newsockfd);
		 	continue;
		 }
		 else if (pid>0) //parent
		 {
		 		close(newsockfd);
		 		continue;
		 }
		 else //child
		 {	
			handleClient(newsockfd); //handle the connection
		 }
		 
     }
    
     close(sockfd);
     return 0; 
}

void handleClient(int clientSocket)
{
     	char buffer[BUFFER_SIZE];
     	int n;
	int count = 0;
	char  fileName[100];
	 bzero(buffer,BUFFER_SIZE);
	 while (1)
	 {
		
		n = read(clientSocket,buffer,BUFFER_SIZE); //read from the socket

		printf("%s",buffer);
	 	int i=0;
		if (count==0)
		{
			getFileName(buffer, fileName);
		}	
		if (strcmp(&buffer[n-2],"\n\n"))
		{
			//no more input
			break;
		}
		count++;
     	}
//	printf("fileName: %s ",fileName);
	if (fileExists(fileName))
	{
		sendResponse(1, fileName, clientSocket);
	}
	else 
	{
		sprintf(fileName, "%s/error.html", DOCUMENT_ROOT);
		sendResponse(0, fileName, clientSocket);
	}
	 close(clientSocket); //close the socket.
}
void sendResponse(int isError, char * fileName, int clientSocket)
{
	char header[500];
	if (isError == 0)
	{
		sprintf(header, "HTTP/1.1 404 Not Found\n");
	}
	else
	{
		sprintf(header, "HTTP/1.1 200 OK\n");
	}
	time_t t = time(NULL);
	char date[60];
	sprintf(date, "Date: %s", ctime(&t)) ;
	strcat(header, date);
	strcat(header, "Server: Martin's server\n");
	char length[40];
	sprintf(length, "Content-Length: %d\n", flength(fileName));
	strcat(header, length);
	strcat(header, "Content-Type: text/html\n\n");

	printf("Response: %s", header);
	write(clientSocket, header, strlen(header));

	FILE * file = fopen(fileName, "r");
	char line[80];
	int numChars = 0;
	while(fgets(line, 80, file)!=NULL)
	{
		write(clientSocket, line, strlen(line));
		printf("%s", line);
	}
}

void  getFileName(char * requestLine, char * fileName)
{
	char * result = strtok(requestLine, " ");
	result = strtok(NULL," ");
	sprintf(fileName, "%s%s",DOCUMENT_ROOT, result);
}

int fileExists(char * fname)
{
	struct stat file_status;

	if(stat(fname, &file_status) != 0)
	{
		perror("could not stat");
		return 0;
	}
	else
		return 1;
}

long flength(char *fname)
{
	struct stat file_status;

	if(stat(fname, &file_status) != 0)
	{
		perror("could not stat");
		return -1L;
	}
//		printf("%ld\n", file_status.st_size);
	return file_status.st_size;
}
