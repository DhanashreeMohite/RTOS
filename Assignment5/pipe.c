/*
We have standard linux utilties ls , grep, wc etc
Write a C program that collect the output of one of these and pipe it as input into another – Do not use standard | operator
Eg:
To find the number of .jpg files in a Linux folder we use the following
% ls -1 *.jpg | wc –l
Your Job is to:
Run ls -1 through a C- program and get the output and simillarly run wc utility. “pipe” the output of ls -1 to wc utility through your C program itself Or search for a particular file name in output of ls -1 using grep
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>

int main()
{
        int     fd[2];
        pid_t pid;
        
                
        if(pipe(fd)<0)
        {
                //failure in creating a pipe
                perror("pipe error");
                exit (1);
        }
        
        //Creating a child process
        pid = fork();
        if (pid < 0) 
        {
         //failure in creating a child
                perror ("fork error");
                exit(1);
        }
        
        if(pid == 0)
        {
                close(1);       //close stdout
                close(fd[0]);   //close read end
                dup(fd[1]);     //duplicate stdout
                execl("/bin/ls", "ls","-l", NULL);
                printf("ls error\n");
                exit(1);
        }
        else
        {       wait (NULL);    //wait till child terminate
                close(0);       //close stdin
                dup(fd[0]);     //duplicate stdin
                close(fd[1]);   //close write end
                execl("/usr/bin/wc", "wc","-l", NULL);
                printf("wc error\n");
                exit(1);     
          }
        
        return(0);
}

