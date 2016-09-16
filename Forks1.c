#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

int main(void)
{
	pid_t pid;
	
	printf("One\n");
	pid = fork();
	if (pid == 0)
	{
		//I am in the child process
		printf("I am the child, pid=%d \n", pid);
		if (execl("/bin/ls", "", NULL)==-1);
			perror("Exec failed\n");
		printf("This will never be printed because process was replaced by the exec() call\n");
	}
	else if (pid > 0)
	{
		//I am in the parent process
		printf("I am the parent, pid=%d \n", pid);
		system("ls");
		printf("This will print after ls\n");
	}
	else
	{
		perror("Error creating fork\n");
		exit(1);
	}

	return 0;
}
