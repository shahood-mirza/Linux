#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

void appendValue(int value);
int readValue();

int main()
{
	printf("beginning sequence...\n");
	pid_t pid1 ;
	int x=0;

	//Create a file and initialize it with a 0
	FILE * open = fopen("part2.txt","w");
	fprintf(open,"0");
	fclose(open);
	int number;

	pid1 = fork();
	for(x;x<1000;x++)
	{	
		//prevent the race
		FILE *lock = fopen("lock","w");
		int flock=fileno(lock);
		lockf(flock, F_LOCK, 0L);
		number = readValue();//read the number
		number=number+1;//increment
		appendValue(number);//appending to file
		lockf(flock, F_ULOCK, 0L);
		fclose(lock);
	}
	if (pid1 >0)
	{
		wait(NULL); //wait for child to finish
		printf("...Complete\n");
		unlink("lock");
	}
	return 0;	
}

void appendValue(int value)
{
	//constructor to append to the file
	FILE *fp = fopen("part2.txt","a");	
	if(fp == NULL)
	{
		printf("File can't be accessed");
		exit(1);
	}
	fprintf(fp,"%d\n",value);
	fclose(fp);
}

int readValue()
{
	int number;
	//another constructor to read the file
	FILE *fpr = fopen("part2.txt","r");
	while (!feof(fpr))
	{
		fscanf(fpr,"%d",&number);
	}
	fclose(fpr);
	return number;
}
