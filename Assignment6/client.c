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

#define Max 4096
#define PORT 5050

int main(int argc, char * argv[])
{
        struct sockaddr_in serv;
        int sd, bufLen;
        char buf[Max];
        
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
        
        while(1)
        {
        printf ("Client: Enter your message:\n");
        
        while(fgets(buf, sizeof (buf), stdin) != NULL)
        {
                int len = strlen(buf);
                if (buf[len-1] == '\n'){ 
                        buf[len-1] = '\0';
                        break;}
        
        }
        bufLen = strlen(buf);
        
        //write(sd,buf, bufLen);
        send(sd,buf, bufLen,0);
        //printf ("message sent:%s\n",buf);
        
        memset(buf,' ', bufLen);        //added when 'send' used
        read (sd, buf, Max);
        printf ("Message from server: %s\n", buf);
        bufLen = strlen(buf);
        memset(buf,' ', bufLen);
        }
}

