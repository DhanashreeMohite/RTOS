#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include<netinet/in.h>
#include <errno.h>
#include <stdlib.h>
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
                else if (message != NULL)
                {
                        printf("\nServer> %s", message);     
                }
        }
}

int main(int argc, char * argv[])
{
        struct sockaddr_in serv;
        int sd, bufLen;
        char message[MsgBuff];
        pthread_t thread;
        
        //memset(buf, '0', sizeof(buf));
        
        //socket(),creates an UN-named socket inside the kernel and returns an integer known as socket descriptor
        if(((sd = socket(AF_INET,SOCK_STREAM,0))<0))
        {    
              perror("socket failed");
              exit(0);  
        }

        serv.sin_family=AF_INET;        //the domain used
        serv.sin_addr.s_addr= inet_addr("127.0.0.1");   //use the loop back ip address,as same machine usd for client and server
        serv.sin_port=htons(PORT);      //Declare port to be used
        
        //connect the socket referred to by the file descriptor sd to the address and port
        if(connect(sd, (struct sockaddr*)(&serv), sizeof(serv)) < 0)
        {
              perror("connect failed");
              exit(0);  
        }
        
        printf ("Connection Established\n");
        //printf ("sd: %d\n", sd);
        // Create new thread to receive messages
        pthread_create(&thread, NULL, RcvMsg, (void *)sd);
        
       
        while (fgets(message, MsgBuff, stdin) != NULL) {
        if (strncmp(message, "/q", 2) == 0) {
        printf("Closing connection...\n");
        break;
        }
        send(sd, message, strlen(message)+1, 0);
        }
        
        close(sd);
        exit(0);      
}
