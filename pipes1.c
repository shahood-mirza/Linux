#include <unistd.h>
#include <stdio.h>
#include <string.h>

int main(void)
{
	int fd[2];
	char * message = "Hello There";
	char buffer[50];

	if  (pipe(fd)==-1)
	{
		//error pipe did not succeed
		perror("Pipe call failed");
		return 1;
	}
	int length = write(fd[1], message, strlen(message) + 1);
	printf("%d bytes were written\n", length);

	int readlength = read(fd[0], buffer, 50);
	printf("%d bytes were read\n", readlength);

	printf("%Message was: %s\n", buffer);

	return 0;
}