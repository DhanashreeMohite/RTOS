#include <stdio.h>
#include <unistd.h>
#include <string.h>

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
	int parentId,childId1,childId2,childId3,slength;
	FILE *fp;
	char s[30] = "kill -9 ";
	slength = strlen(s);
	parentId = getpid();
	if ( fork() == 0 )
        {		
		childId1 = getpid();
		sprintf( s + slength , "%d", childId1);
		fp = fopen("/home/dhanashree/Dhanashree/Kill.sh","w");
		fprintf (fp, "%s\n", "#!/bin/sh");
		fprintf (fp, "%s\n", s);
		fclose(fp); 
		printf( "child1 pid %d from pid %d\n",childId1 , getppid() );
		PrintVariable(a, b, c);
		while(1);
        }
	else
	{
		if ( fork() == 0 )
        	{
			sleep(0.2);
			childId2 = getpid();
			sprintf( s + slength , "%d", childId2 );
			fp = fopen("/home/dhanashree/Dhanashree/Kill.sh","a");
			fprintf (fp, "%s\n", s);
			fclose(fp);            		
			printf( "child2 pid %d from pid %d\n", getpid(), getppid() );
			PrintVariable(a, b, c);
			while(1);
        	}
		else
		{
			if ( fork() == 0 )
        		{
								
				sleep(0.4);
				childId3 = getpid();
				sprintf( s + slength , "%d", childId3);
				fp = fopen("/home/dhanashree/Dhanashree/Kill.sh","a");
				fprintf (fp, "%s\n", s);
				fclose(fp);            			
				printf( "child3 pid %d from pid %d\n", getpid(), getppid() );
				PrintVariable(a, b, c);
				while(1);
        		}
			else
			{
				
				sleep(0.8);
				printf( "parent pid %d\n", parentId);
				sprintf( s + slength , "%d", parentId);
				fp = fopen("/home/dhanashree/Dhanashree/Kill.sh","a");
				fprintf (fp, "%s\n", s);
				fclose(fp);
				while(1);
			}
		}
		
	}
	
}
