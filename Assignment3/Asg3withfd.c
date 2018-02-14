/*
Question:
create Three child processes.

What the main program will do is
1. print the address of three local variable int a b c .
2. Then it will print address of three global variable   int g_first , g_second, g_third
3. In third third step it will fork() three child process and parent enter into a infinite loop and wait for ever (Do not terminate)
4. Each of these process will execute a function PrintVariable(). Parent does not call this function instead it will go into infinite loop
5. The function PrintVariable will print the address of the three local int j,k,l ;  which are declared inside the function PrintVariable()
6. After this the same function will also print the address of the global variable  int g_first , g_second, g_third
7. After this all the three child process will get into a infinite loop just like parent and wait for ever

Observe the value of the address printed by each of the child process for each of the variable and document it in the form of a table.
*/

//with file descriptor

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
	printf( "address of x : %p\n", (void*) &x);
	printf( "address of y : %p\n", (void*) &y);
	printf( "address of z : %p\n", (void*) &z);
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
	if ( fork() == 0 )	//child process1
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
		if ( fork() == 0 )	//child process2
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
			if ( fork() == 0 )	//child process3
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
				
				//parent process
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
				PrintVariable(a, b, c);
				while(1);
			}
		}
		
	}
	
}

/*
Observation:
After execution we can see, the global variable as well as the local variable addresses are same for the processes.

The reason behind this is below:
When fork executed, the child process is given a PID,  its memory  map is set up, and it is given shared access to its parent’s files. Then its registers are set up and it is ready to run.
Hence we can say that, after a fork, we have two nearly identical processes with contents of their pages are the same.
But they will stay the same until one or the other changes a memory value in a page.
Instead of making copies of the pages in the fork(), the operating system lets both processes share all the pages, but marks every page as read-only.
If one process tries to alter a page, a memory error occurs. The operating system receives an interrupt, sees that the page is shared, makes a copy of the page by physically copying the contents to another page frame and mapping the 2nd page frame to the current process, and gets the process to retry the memory access.  


*/
