/*
PRoblem-2
Software engineers are designing an application that runs on a Linux machine. The Core of the app is mathematical engine which processes complex mathematical equations based on Linear Alegera.
Multiple users would like to run this this application through a client.
Design a client and server model to implement this app
*/

//sending result
#define MAXSIZE 128

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>

struct msgBuf {
long mesg_type;
char data[MAXSIZE];
};

int main(void)
{
        struct msgBuf buf;
        int msqid;
        key_t key;
        int a,b,c;

        if ((key = ftok(".", 'A')) == -1) //generating key--> depends on path and id
        {
                perror("ftok");
                exit(0);
        }
        
        if ((msqid = msgget(key, 0644 | IPC_CREAT)) == -1)
        {
                perror("msgget");
                exit(0);
        }
        
        printf("server msgid is %d\n", msqid);
        
        while(1)
        {
        int len = strlen(buf.data);
        
        //receiving inputs
        if (msgrcv(msqid, &buf, MAXSIZE, 1, 0) == -1)
                perror("msgrcv");
                
        printf("%s Data received\n", buf.data); 

        sscanf(buf.data, "%d %d", &a, &b);
        
        printf("Received : a = %d, b= %d\n", a, b);

        //operation
        c = a+b;
        
        char output[10];
        sprintf(output,"%d",c);
        strcpy(buf.data,output);
        
        // sending reult
        len = strlen(buf.data);
        buf. mesg_type = 1;
        
        if (msgsnd(msqid, &buf, (len+1), 0) == -1) /* +1 for '\0' */
                perror("msgsnd");
        
        printf("result = %s sent\n", output);
        }
         
        return 0;
        
}
