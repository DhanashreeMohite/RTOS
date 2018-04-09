#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include<sys/stat.h>
#include <string.h>
#include "pthread.h"

#define Max 4096
#define PORT 5050

int MsgBuff = 500;

void *RcvMsg(void * sd)
{
        char message[MsgBuff];
        int response;
        int socket_fd = (int) sd;
        while(1)
        {
                memset(message, 0, MsgBuff); // Clear message buffer
                response = recvfrom(socket_fd, message, MsgBuff, 0, NULL, NULL);
                if (response == -1) 
                {
                        printf("\nError in receiving\n");
                        break;
                } 
                else if (response == 0) {
                      printf("\nPeer disconnected\n");
                      break;
                }
                else if(message != NULL)
                {
                        printf("\nClient> %s", message);     
                }
        }
          
}

int main()
{
        struct sockaddr_in serv, cli;
        int sd, sz, nsd,bufLen; 
        char message[MsgBuff];
        pthread_t thread;
        
        memset(&serv, '0', sizeof(serv));
        
        //socket(),creates an UN-named socket inside the kernel and returns an integer known as socket descriptor
        if(((sd = socket(AF_INET,SOCK_STREAM,0))<0))
        {    
              perror("socket failed");
              exit(0);  
        }
        printf ("Socket created\n");
        
        serv.sin_family=AF_INET;        //the domain used
        serv.sin_addr.s_addr= htonl(INADDR_ANY);    //Permit any incoming IP address by declaring INADDR_ANY
        serv.sin_port=htons(PORT);      //Declare port to be used

        //bind(), assigns the details specified above to the socket
        if((bind(sd,(struct sockaddr*)(&serv),sizeof(serv)))<0) //attach
        {
                perror("bind failed");
                exit(0);
        }

        //listen(), specifies maximum number of client connections that server will queue for this listening socket
        if(listen(sd,5)<0)
        {
                perror("listen");
                exit(0);
        }
    
        printf ("Server listening\n");
        
        
        nsd=accept(sd,(struct sockaddr*)NULL,NULL);
        
        // Create new thread to receive messages
        pthread_create(&thread, NULL, RcvMsg, (void *)nsd);
        
        while (fgets(message, MsgBuff, stdin) != NULL) 
        {
                if (strncmp(message, "/q", 2) == 0) 
                {
                printf("Closing connection...\n");
                break;
                }
                send(nsd, message, strlen(message)+1, 0);
                //printf ("sd: %d\n", nsd);
        }
      
        pthread_exit(NULL);       
        close(nsd);
        close(sd);
        exit(0);
        

}
