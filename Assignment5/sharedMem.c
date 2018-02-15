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
char mtext[200];
};

int main()
{
        //shared memory operations
        key_t key;
        struct SharedMemory *m;
        
        int shmid;
        if ((key = ftok(".", 'B')) == -1)       //generating key
        {
                printf("ftok");
                exit(1);
        }
   //IPC_CREAT | 0666 for a server (i.e., creating and granting read and write access to the server)
        if ((shmid = shmget(key, (200*sizeof(char)), (IPC_CREAT | 0666))) < 0)  //shared mem ID
        {
                perror("shmget");
                exit(1);
        }
        
        m= shmat(shmid, NULL, 0);       //attaches shared mem

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
                wait(NULL);     //waiting till child completes writing
                for(int i = 0; i<10; i++)
                {
                        printf("reading= %s \n",m->mtext);

                }
                exit(0);
                
        }
        
        shmdt(NULL);    //dettaches shared mem
        return 0;
        
}
