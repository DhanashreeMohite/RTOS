#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include<sys/stat.h>
#include <string.h>
#include <errno.h>

#define Max 4096
#define PORT 5050

int main()
{
        struct sockaddr_in serv, cli;
        int sd, sz, nsd,bufLen; 
        char buf[Max];
        int opt = 1;
        
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
        
        while(1)
        {
        //printf ("Message accepted\n");
        memset(buf,' ', bufLen);        //added when 'send' used
        read(nsd,buf,Max);
        printf ("Message from Client: %s\n", buf);
        
        bufLen = strlen(buf);
        memset(buf,' ', bufLen);
        
        printf ("server: Enter your message:\n");
        while(fgets(buf, sizeof (buf), stdin) != NULL)
        {
                int len = strlen(buf);
                if (buf[len-1] == '\n'){ 
                        buf[len-1] = '\0';
                        break;}
        
        }
        bufLen = strlen(buf);
        
        //write(nsd,buf, bufLen);
        send(nsd,buf,bufLen,0);
        }
        close(nsd);

}


