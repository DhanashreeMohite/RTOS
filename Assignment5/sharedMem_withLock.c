/*
1. Re-write the sample program (Which was discussed in class) which uses Shared memory with parent and child created out of fork()
2. Implement Semaphore for the preventing accidental over write while reading in progress
*/

//with lock

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
#include <pthread.h> 

struct SharedMemory {
//long mtype;
char mtext[200];
};


pthread_mutex_t mut;

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
               
                if (pthread_mutex_lock(&mut) != 0)                                  
                        perror("mutex_lock"); 
                        
                for(int i = 0; i<10; i++)
                {
                        sprintf(m->mtext,"instance: %d ", i);
                        printf("writing: %s \n", m->mtext);
                }
                
                if (pthread_mutex_unlock(&mut) != 0)                                  
                        perror("mutex_lock"); 
                
                exit(0);
                      
        }
        else
        { 
                if (pthread_mutex_lock(&mut) != 0)                                  
                        perror("mutex_lock"); 
                for(int i = 0; i<10; i++)
                {
                        printf("reading= %s \n",m->mtext);

                }
                if (pthread_mutex_unlock(&mut) != 0)                                  
                        perror("mutex_lock");  
                
                exit(0);
                
         }
         
        shmdt(NULL);
        return 0;
        
}
