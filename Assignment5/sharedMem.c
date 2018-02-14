/*
Re-write the sample program (Which was discussed in class) which uses Shared memory with parent and child created out of fork()
*/

//with wait(NULL)

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>  
#include <sys/shm.h>    
#include <unistd.h>     //fork
#include <sys/ipc.h>    //shmget,ftok
#include <errno.h>
#include <sys/wait.h>
#include <semaphore.h>
#include <sys/sem.h>

struct SharedMemory {
//long mtype;
char mtext[200];
};

int main()
{
        key_t key;
        struct SharedMemory *m;
        
        int shmid;
        if ((key = ftok("reference.txt", 'B')) == -1)
        {
                printf("ftok");
                exit(1);
        }
   //IPC_CREAT | 0666 for a server (i.e., creating and granting read and write access to the server)
        if ((shmid = shmget(key, (200*sizeof(char)), (IPC_CREAT | 0666))) < 0)
        {
                perror("shmget");
                exit(1);
        }
        
        m= shmat(shmid, NULL, 0);

        if ( m == (struct SharedMemory *)-1)
        {       
                perror("shmat");
                exit(1);
        }
        
        pid_t pid;
        pid = fork();
        
        if (pid < 0) 
        {
         //failure in creating a child
                perror ("fork error");
                exit(1);
        }
        
        if(pid == 0)
        {
                for(int i = 0; i<10; i++)
                {
                        sprintf(m->mtext,"instance: %d ", i);
                        printf("writing: %s \n", m->mtext);
                }
                exit(0);
                      
        }
        else
        {
                wait(NULL);
                for(int i = 0; i<10; i++)
                {
                        printf("reading= %s \n",m->mtext);

                }
                exit(0);
                
        }
        
        shmdt(NULL);
        return 0;
        
}
