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
#include <stddef.h>
#include <pulse/simple.h>
#include <pulse/error.h>

#define PORT 5050
#define BUFSIZE 1024

struct sockaddr_in cli;
int slen=sizeof(cli);
int sd;

/* A simple routine calling UNIX write() in a loop */
static ssize_t loop_write(int fd, const void*data, size_t size) {
    ssize_t ret = 0;
    while (size > 0) {
        ssize_t r;
        printf ("Sending data\n");
        if ((r = sendto(fd, data, size, 0, (struct sockaddr *)&cli, slen)) < 0)
            return r;
        if (r == 0)
            break;
        ret += r;
        data = (const uint8_t*) data + r;
        size -= (size_t) r;
    }
    return ret;
}

void *RcvMsg()
{
        int16_t message[BUFSIZE];
        int response;
        //int socket_fd = (int) sd;
            /* The Sample format to use */
    static const pa_sample_spec ss = {
        .format = PA_SAMPLE_S16LE,
        .rate = 44100,
        .channels = 2
    };
    pa_simple *s = NULL;
    int ret = 1;
    int error;
     /* Create the recording stream */
        if (!(s = pa_simple_new(NULL,"Julius", PA_STREAM_PLAYBACK, NULL, "playback", &ss, NULL, NULL, &error))) 
        {
                fprintf(stderr, __FILE__": pa_simple_new() failed: %s\n", pa_strerror(error));
                if (s)
                        pa_simple_free(s);    
                //return ret;
        }
        
        while(1)
        {
                uint8_t buf[BUFSIZE];
                ssize_t r;

                /* Read some data ... */
                printf ("Reading data\n");
                if ((r = recvfrom(sd, buf, sizeof(buf), 0, (struct sockaddr *) &cli, &slen)) <= 0) 
                {
                        if (r == 0) /* EOF */
                        break;
                        fprintf(stderr, __FILE__": read() failed: %s\n", strerror(errno));
                        if (s)
                                pa_simple_free(s);    
                        //return ret;
                }
                /* ... and play it */
                printf ("Playing data\n");
                if (pa_simple_write(s, buf, (size_t) r, &error) < 0) 
                {
                        fprintf(stderr, __FILE__": pa_simple_write() failed: %s\n", pa_strerror(error));
                        if (s)
                                pa_simple_free(s);    
                        //return ret;
                }
        }
        /* Make sure that every single sample was played */
        if (pa_simple_drain(s, &error) < 0) 
        {
                fprintf(stderr, __FILE__": pa_simple_drain() failed: %s\n", pa_strerror(error));
                if (s)
                        pa_simple_free(s);    
                //return ret;
        }
        ret = 0;          
}


int main(int argc, char * argv[])
{
        pthread_t thread;
        int r;
        
        memset(&cli, '0', sizeof(cli));
        
        //socket(),creates an UN-named socket inside the kernel and returns an integer known as socket descriptor
        if(((sd = socket(AF_INET,SOCK_DGRAM, 0))<0))
        {    
              perror("socket failed");
              exit(0);  
        }
        printf ("Socket created\n");
        
        cli.sin_family=AF_INET;        //the domain used
        cli.sin_addr.s_addr= htonl(INADDR_ANY);    //Permit any incoming IP address by declaring INADDR_ANY
        cli.sin_port=htons(PORT);      //Declare port to be used

        //bind(), assigns the details specified above to the socket
        if((bind(sd,(struct sockaddr*)(&cli),sizeof(cli)))<0) //attach
        {
                perror("bind failed");
                exit(0);
        }
        
        printf ("Connection Established\n");
        uint8_t buf[BUFSIZE];
        recvfrom(sd, buf, sizeof(buf), 0, (struct sockaddr *) &cli, &slen);
        // Create new thread to receive messages
        pthread_create(&thread, NULL, RcvMsg, NULL);
        
         static const pa_sample_spec ss = {
        .format = PA_SAMPLE_S16LE,
        .rate = 44100,
        .channels = 2
        };
        pa_simple *s = NULL;
        int ret = 1;
        int error;
        /* Create the recording stream */
            if (!(s = pa_simple_new(NULL, argv[0], PA_STREAM_RECORD, NULL, "record", &ss, NULL, NULL, &error))) {
        fprintf(stderr, __FILE__": pa_simple_new() failed: %s\n", pa_strerror(error));
        if (s)
            pa_simple_free(s);
        return ret;
    }
        
        while(1) 
        {
                uint8_t buf[BUFSIZE];
                /* Record some data ... */
                if (pa_simple_read(s, buf, sizeof(buf), &error) < 0) {
                    fprintf(stderr, __FILE__": pa_simple_read() failed: %s\n", pa_strerror(error));
                     if (s)
                        pa_simple_free(s);
                        
                     return ret;
                }
                /* And write it to STDOUT */
                if (loop_write(sd, buf, sizeof(buf)) != sizeof(buf)) {
                    fprintf(stderr, __FILE__": write() failed: %s\n", strerror(errno));
                    if (s)
                        pa_simple_free(s);
                    return ret;
                }
        }
        
        
        ret = 0;
        pthread_exit(NULL);       
        close(sd);
        exit(0);
        

}

/*
command for compiling: gcc ServerUDP.c -o ser -lpthread $(pkg-config --cflags --libs libpu
lse-simple)
*/
