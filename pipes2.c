#include <unistd.h>
#include <stdio.h>
#include <string.h>

#define READ_LENGTH 255

int main(void)
{
	int fd[2];
	int secondfd[2];
	pid_t pid;

	char buffer[READ_LENGTH];

	if  (pipe(fd)==-1)
	{
		//error pipe did not succeed
		perror("Pipe call failed");
		return 1;
	}
	if  (pipe(secondfd)==-1)
	{
		//error pipe did not succeed
		perror("Second pipe call failed");
		return 2;
	}

	pid = fork();
	if (pid < 0)
	{
		perror("Fork call failed");
		return 3;
	}
	else if (pid == 0)
	{
		//in the child. We will write here
		close(fd[0]);	//close the reading end as we don't need it
		char * message = "Hello from the child";

		int length = write(fd[1], message, strlen(message) + 1);
		printf("In child, %d bytes were written\n", length);

		close(secondfd[1]);	//closes the writing end as we don't need it

		int readlength = read(fd[0], buffer, READ_LENGTH);
		printf("In child, %d bytes were read\n", readlength);
		printf("%Message was: %s\n", buffer);
	}
	else
	{
		//in the parent. We will write here
		close(fd[1]);	//close the write end as we don't need it

		int readlength = read(fd[0], buffer, READ_LENGTH);
		printf("%d bytes were read\n", readlength);
		printf("%Message was: %s\n", buffer);

		close(secondfd[0]);

		char * message = "Hello from the child";
		int length = write(fd[1], message, strlen(message) + 1);
		printf("In Parnet, %d bytes were written\n", length);
	}

	return 0;
}