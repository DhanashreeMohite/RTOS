#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <stdlib.h>

int g_first , g_second, g_third;
void PrintVariable(int a, int b, int c)
{
	int x,y,z;
	printf( "address of a : %p\n", (void*) &a);
	printf( "address of b : %p\n", (void*) &b);
	printf( "address of c : %p\n", (void*) &c);
	printf( "address of a : %p\n", (void*) &x);
	printf( "address of b : %p\n", (void*) &y);
	printf( "address of c : %p\n", (void*) &z);
	printf( "address of g_first : %p\n", (void*) &g_first);
	printf( "address of g_second : %p\n", (void*) &g_second);
	printf( "address of g_third : %p\n", (void*) &g_third);
}

void main()
{
	
	int a,b,c;
	int parentId,childId1,childId2,childId3,slength,fd,headerlen,len;
	char s[30] = "kill -9 ";
	char header[30] = "#!/bin/sh \n";
	if((fd = open("/home/dhanashree/Dhanashree/Kill.sh", O_WRONLY | O_APPEND | O_CREAT))<0)
	{ 
		perror("Problem in opening the file");
		exit(1);
	}
	headerlen = strlen(header);
	write(fd, header, headerlen);
	close(fd);
	slength = strlen(s);
	parentId = getpid();
	if ( fork() == 0 )
        {		
		childId1 = getpid();
		sprintf( s + slength , "%d\n", childId1);
		len = strlen(s);
		if((fd = open("/home/dhanashree/Dhanashree/Kill.sh", O_WRONLY | O_APPEND))<0)
		{ 
			perror("Problem in opening the file");
			exit(1);
		}
		write(fd, s, len);
		close(fd); 
		printf( "child1 pid %d from pid %d\n",childId1 , getppid() );
		PrintVariable(a, b, c);
		while(1);
        }
	else
	{
		if ( fork() == 0 )
        	{
			childId2 = getpid();
			sprintf( s + slength , "%d\n", childId2 );
			len = strlen(s);
			if((fd = open("/home/dhanashree/Dhanashree/Kill.sh", O_WRONLY | O_APPEND))<0)
			{ 
				perror("Problem in opening the file");
				exit(1);
			}
			write(fd, s, len);
			close(fd);             		
			printf( "child2 pid %d from pid %d\n", getpid(), getppid() );
			PrintVariable(a, b, c);
			while(1);
        	}
		else
		{
			if ( fork() == 0 )
        		{
								
				childId3 = getpid();
				sprintf( s + slength , "%d\n", childId3);
				len = strlen(s);
				if((fd = open("/home/dhanashree/Dhanashree/Kill.sh", O_WRONLY | O_APPEND))<0)
				{ 
					perror("Problem in opening the file");
					exit(1);
				}
				write(fd, s, len);
				close(fd);           			
				printf( "child3 pid %d from pid %d\n", getpid(), getppid() );
				PrintVariable(a, b, c);
				while(1);
        		}
			else
			{
				
				printf( "parent pid %d\n", parentId);
				sprintf( s + slength , "%d\n", parentId);
				len = strlen(s);
				if((fd = open("/home/dhanashree/Dhanashree/Kill.sh", O_WRONLY | O_APPEND))<0)
				{ 
					perror("Problem in opening the file");
					exit(1);
				}
				write(fd, s, len);
				close(fd);
				while(1);
			}
		}
		
	}
	
}
