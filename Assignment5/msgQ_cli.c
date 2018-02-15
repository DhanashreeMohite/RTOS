/*
PRoblem-2
Software engineers are designing an application that runs on a Linux machine. The Core of the app is mathematical engine which processes complex mathematical equations based on Linear Alegera.
Multiple users would like to run this this application through a client.
Design a client and server model to implement this app
*/

//sending input
//receiving result
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
        
        /*permission to:  owner      group      other     
                /¯¯¯\      /¯¯¯\      /¯¯¯\
        octal:            6          6          6
        binary:         1 1 0      1 1 0      1 1 0
        what to permit: r w x      r w x      r w x

        binary         - 1: enabled, 0: disabled                

        what to permit - r: read, w: write, x: execute

        permission to  - owner: the user that create the file/folder
                 group: the users from group that owner is member
                 other: all other users
        */
        //either returns the message queue identifier for a newly created message queue or returns the identifiers for a queue which exists with the same key value.
        
        if ((msqid = msgget(key, 0644 | IPC_CREAT)) == -1)
        {
                perror("msgget");
                exit(0);
        }
        
        printf("client msgid is %d\n", msqid);
        while(1)
        {
        printf("Give 2 nos. for addition operation separated by space: \n");
        printf("a b = ");
        
        while(fgets(buf.data, sizeof (buf.data), stdin) != NULL)
        {
                int len = strlen(buf.data);
                if (buf.data[len-1] == '\n'){ 
                        buf.data[len-1] = '\0';
                        break;}
        
        }
        
        // sending inputs
        int len = strlen(buf.data);
        buf. mesg_type = 1;
        if (msgsnd(msqid, &buf, (len+1), 0) == -1) /* +1 for '\0' */
                perror("msgsnd");
                
        printf("%s Data Send \n", buf.data);
       
        //Receiving result
        if (msgrcv(msqid, &buf, MAXSIZE, 1, 0) == -1)
                perror("msgrcv");
                
        sscanf(buf.data, "%d", &c);
        
        printf("final result after operation : result = %d\n", c);
        }
       
        if (msgctl(msqid, IPC_RMID, NULL) == -1)        //IPC_RMID = immediately remove msg q
        {
                perror("msgctl");
                exit(0);
        }
        return 0;       

}

/*
references:
http://qa.geeksforgeeks.org/4449/qa.geeksforgeeks.org/4449/meaning-of-0666-in-permission

https://users.cs.cf.ac.uk/Dave.Marshall/C/node25.html
*/
