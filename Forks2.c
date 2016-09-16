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
		printf("I am the child, variable pid=%d, pid=%d, ppid=%d, uid=%d, \n", pid, getpid(), getppid(), getuid());
		int i;
		for (i=0; i<20; i++)
		{	
			printf("i is: %d",i);
		}	
		return 5;	
	}
	else if (pid > 0)
	{
		
		//I am in the parent process
		printf("I am the parent, variable pid=%d, pid=%d, ppid=%d, uid=%d, \n", pid, getpid(), getppid(), getuid());
		wait(NULL);
		printf("child finished parent continues\n");
	}
	else
	{
		perror("Error creating fork\n");
		exit(1);
	}

	return 0;
}
