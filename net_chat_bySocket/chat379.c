#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <strings.h>
#include <netdb.h> //for gethostbyname
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <signal.h>
#define MAX_BUF 4096
//#define SERVER_PORT 12138
 
struct user {
	char name[32];
} ;
 
int main(int argc,char *argv[])
{
	struct user use;
    int sockfd;//socket
    char sendBuf[MAX_BUF],recvBuf[MAX_BUF];
    int sendSize,recvSize;
    int SERVER_PORT;
    char PORT[10];
    uint16_t testbuff1;
    uint16_t testbuff2;
    struct hostent * host;
    struct sockaddr_in servAddr;
    char username[32];
    char * p;
    int pid;
 
     if(argc != 4)
     {
         perror("use: ./client [hostname] [SERVER_PORT] [username]");
         exit(-1);
     }
     strcpy(PORT,argv[2]);
     SERVER_PORT=atoi(PORT);
     printf("server port%d\n",SERVER_PORT);
    
     strcpy(use.name,argv[3]);
     printf("username:%s\n",use.name);
     host=gethostbyname(argv[1]);
     if(host==NULL)
     {
         perror("fail to get host by name.");
         exit(-1);
     }
     printf("Success to get host by name ...\n");
 

     if((sockfd=socket(AF_INET,SOCK_STREAM,0))==-1)
     {
         perror("fail to establish a socket");
         exit(1);
     }
     printf("Success to establish a socket...\n");
 
     /*init sockaddr_in*/
     servAddr.sin_family=AF_INET;
     servAddr.sin_port=htons(SERVER_PORT);
     servAddr.sin_addr=*((struct in_addr *)host->h_addr);
     bzero(&(servAddr.sin_zero),8);
 
     /*connect the socket*/
     if(connect(sockfd,(struct sockaddr *)&servAddr,sizeof(struct sockaddr_in))==-1)
     {
         perror("fail to connect the socket");
         exit(1);
     }
    if(recv(sockfd,&testbuff1,sizeof(uint16_t),0) && recv(sockfd,&testbuff2,sizeof(uint16_t),0) ){
        if(ntohs(testbuff1)==207 && ntohs(testbuff2)==167){
            //printf("%d %d\n",ntohs(testbuff1),ntohs(testbuff2));
            printf("handshake success\n");
            printf("Success to connect the socket...\n");
        }
        else{
            printf("wrong number to send");
            exit(-1);
        }
    }
        //printf("Success to connect the socket...\n");
    if(send(sockfd,(char *)&use,sizeof(struct user),0)==-1){
        perror("fail to send datas.");
        exit(-1);
    }
     


    if((pid=fork())<0){
        perror("fork error\n");
    }
    else if(pid==0)/*child*/{
        while(1){
            fgets(sendBuf,MAX_BUF,stdin);
            if(send(sockfd,sendBuf,strlen(sendBuf),0)==-1){
                perror("fail to receive datas.");
            }
            memset(sendBuf,0,sizeof(sendBuf));
        }
    }
    else{
        while(1){
            if((recvSize=recv(sockfd,recvBuf,MAX_BUF,0)==-1)){
                printf("Server maybe shutdown!");
                break;
            }
            if (strncmp(recvBuf,"name",4)==0){
            close(sockfd);

            }
            printf("%s\n",recvBuf);
            memset(recvBuf,0,sizeof(recvBuf));
        }
        kill(pid,SIGKILL);
    }
    close(sockfd);

    return 0;
}
