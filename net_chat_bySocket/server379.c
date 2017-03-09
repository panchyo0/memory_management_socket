#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <strings.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <sys/un.h>
#include <sys/ioctl.h>
#include <sys/wait.h>
#include <sys/select.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/stat.h> 
#include <unistd.h>
#include <signal.h>

#include <setjmp.h>
  //#define SERVER_PORT 12138
#define BACKLOG 20
#define MAX_CON_NO 10
#define MAX_DATA_SIZE 4096


int MAX(int a,int b)
{
   if(a>b) return a;
      return b;
}
struct user{
  char name[32];
  };
FILE *fp;
void signal_handler(int arg){
   
   fprintf(fp,"\n");
   fprintf(fp,"terminating......\n");
   fclose(fp);

   exit(0);
}

  int main(int argc,char *argv[])
  {
    int SERVER_PORT;
    char PORT[10];
    char logname[60];
    struct user use;
    struct sockaddr_in serverSockaddr,clientSockaddr;
    char sendBuf[MAX_DATA_SIZE],recvBuf[MAX_DATA_SIZE],temp[MAX_DATA_SIZE];
    int sendSize,recvSize;
    int sockfd,clientfd;
    fd_set servfd,recvfd;//
    int fd_A[BACKLOG+1];//
    char fd_C[BACKLOG+1][32];
    int conn_amount;//
    int max_servfd,max_recvfd;
    int on=1;
    socklen_t sinSize=0;
    char username[32];
   
    int i,j;
    struct timeval timeout;
      /////////DAEMON
    pid_t pid = 0;
    pid_t sid = 0;
    
    



    pid = fork();
          
    if (pid < 0)
    {
        printf("fork failed!\n");
        exit(1);
    }
     
    if (pid > 0)
    {
    	// in the parent
       printf("pid of parent process %d \n", pid);
    
       exit(0); 
    }

    
       umask(0);

	// open a log file
     
    sprintf(logname,"server379%d.log",pid);
	 fp = fopen (logname, "w+");
    
    // create new process group -- don't want to look like an orphan
    sid = setsid();
    if(sid < 0)
    {
    	printf("cannot create new process group");
        exit(1);
    }
    
    /* Change the current working directory */ 
    if ((chdir("/")) < 0) {
      printf("Could not change working directory to /\n");
      exit(1);
    }		

	// close standard fds
    close(STDIN_FILENO);
    close(STDOUT_FILENO);
    close(STDERR_FILENO);

    signal(SIGTERM,signal_handler);
///////////////////////////////////////////////////////////
     
      if(argc != 2)
      {

         exit(1);
      }
      strcpy(PORT,argv[1]);
      SERVER_PORT=atoi(PORT);
      
    
  
      /*establish a socket*/
      if((sockfd = socket(AF_INET,SOCK_STREAM,0))==-1)
      {
         perror("fail to establish a socket");
          exit(1);
      }
      printf("Success to establish a socket...\n");
  
      /*init sockaddr_in*/
      serverSockaddr.sin_family=AF_INET;
      serverSockaddr.sin_port=htons(SERVER_PORT);
      serverSockaddr.sin_addr.s_addr=htonl(INADDR_ANY);
      bzero(&(serverSockaddr.sin_zero),8);
  
      /*
       * SOL_SOCKET.SO_REUSEADDR 
       * */
      setsockopt(sockfd,SOL_SOCKET,SO_REUSEADDR,&on,sizeof(on));
  
     /*bind socket*/
     if(bind(sockfd,(struct sockaddr *)&serverSockaddr,sizeof(struct sockaddr))==-1)
      {
          perror("fail to bind");
          exit(1);
      }
      printf("Success to bind the socket...\n");
  
      /*listen on the socket*/
     if(listen(sockfd,BACKLOG)==-1)
      {
          perror("fail to listen");
          exit(1);
      }
  
      timeout.tv_sec=1;//
      timeout.tv_usec=0;
      sinSize=sizeof(clientSockaddr);//
  
      FD_ZERO(&servfd);//
      FD_ZERO(&recvfd);//
      FD_SET(sockfd,&servfd);
      conn_amount=0;
      max_servfd=sockfd;//
      max_recvfd=0;//
	  memset(fd_A,0,sizeof(fd_A));
      memset(fd_C,0,sizeof(fd_C));
      
      while(1)
      {
      
         FD_ZERO(&servfd);//
         FD_ZERO(&recvfd);//
         FD_SET(sockfd,&servfd);
         
         switch(select(max_servfd+1,&servfd,NULL,NULL,&timeout))//
        {
             case -1:
                 perror("select error");
                 break;
            //case 0:
                
              //   break;
             default:
             
                 if(FD_ISSET(sockfd,&servfd))//
                {   
				
                    /*accept a client's request*/
                     if((clientfd=accept(sockfd,(struct sockaddr  *)&clientSockaddr, &sinSize))==-1)
                     {
                         perror("fail to accept");
                         exit(1);
                     }
                     // send two hex number to client
                    uint16_t testbuff1=htons(0xCF);
                    uint16_t testbuff2=htons(0xA7);
                    send(clientfd,&testbuff1,sizeof(uint16_t),0);
                    send(clientfd,&testbuff2,sizeof(uint16_t),0);

                    fprintf(fp,"Success to accpet a connection request...\n");
                    fprintf(fp,"0x01 >>>>>> %s:%d join in!\n",inet_ntoa(clientSockaddr.sin_addr),ntohs(clientSockaddr.sin_port));
                   
/////////////////////////
                    if((recvSize=recv(clientfd,(char *)&use,sizeof(struct user),0))==-1 || recvSize==0)
                     {
                         perror("fail to receive datas");
                     }
                     fprintf(fp,"user name from the client:%s\n",use.name);
					 
                     memset(recvBuf,0,sizeof(recvBuf));
//////////////////////////////////////////////////////////////////////////////////

					 //////////////
		    for (j=0;j<MAX_CON_NO;j++){
  			if ((strcmp(fd_C[j],use.name))==0){
  			 strcat(sendBuf,"name already exist\n"); 
  			 strcat(sendBuf,"about to close\n");
 		         send(clientfd,sendBuf,strlen(sendBuf),0);
			 memset(sendBuf,0,sizeof(sendBuf));
			 fprintf(fp,"name already exist!\n");
   			//FD_CLR(clientfd,&recvfd);
  			  }
			}
					 /////////////////
                        memset(recvBuf,0,sizeof(recvBuf));
//////////////////////////////////////////////////////////////////////////////////
/*Send the number of current clients*/
		   sprintf(sendBuf,"%d clients online\n",conn_amount);
		   if((sendSize=send(clientfd,sendBuf,strlen(sendBuf),0))!=strlen(sendBuf))
                         {
                               perror("fail");
                                exit(1);
                         }
                   else
                         {
                              fprintf(fp,"Success\n");
                          }
		  memset(sendBuf,0,sizeof(sendBuf));
/*Send the names of the current */
	         for (j=0;j<MAX_CON_NO;j++){
  			if ((strcmp(fd_C[j],"\0"))!=0){
   			strcat(sendBuf,fd_C[j]); 
   			strcat(sendBuf,"\n");
   
  			}
		}
		if((sendSize=send(clientfd,sendBuf,strlen(sendBuf),0))!=strlen(sendBuf))
                           {                                  perror("fail");
                                         exit(1);
                                     }
                                     else
                                     {
                                         fprintf(fp,"Success\n");
                                     }
				memset(sendBuf,0,sizeof(sendBuf));
///////////////////////////////////////////////////////////////////////////////////
                   
					sprintf(recvBuf,"0x01 %s join in!",use.name);
                    for(j=0;j<MAX_CON_NO;j++)
                             {
                                 if(fd_A[j]!=0&&i!=j)
                                 {
                                     fprintf(fp,"Send Data to %d,",fd_A[j]);
                                     if((sendSize=send(fd_A[j],recvBuf,strlen(recvBuf),0))!=strlen(recvBuf))
                                     {
                                         perror("fail");
                                         exit(1);
                                     }
                                     else
                                     {
                                         fprintf(fp,"Success\n");
                                     }
                                 }
                             }
                     
                     memset(recvBuf,0,MAX_DATA_SIZE);




/////////////////////////

                     fd_A[conn_amount]=clientfd;
                     strcpy(fd_C[conn_amount],use.name);
                     conn_amount++;
                     max_recvfd=MAX(max_recvfd,clientfd);

                 }
                 break;
         }
         //FD_COPY(recvfd,servfd);
         for(i=0;i<MAX_CON_NO;i++)//
         {
             if(fd_A[i]!=0)
             {
                 FD_SET(fd_A[i],&recvfd);//
             }
         }
 
         switch(select(max_recvfd+1,&recvfd,NULL,NULL,&timeout))
        {
             case -1:
                 //select error
                 break;
             case 0:
                 //timeout
                 break;
             default:
                 for(i=0;i<MAX_CON_NO;i++)
                 {
                     if(FD_ISSET(fd_A[i],&recvfd))
                     {
                         /*receive datas from client*/
                         if((recvSize=recv(fd_A[i],recvBuf,MAX_DATA_SIZE,0))==-1 || recvSize==0)
                         {
                             //perror("fail to receive datas");
                       
                             fprintf(fp,"fd %d close\n",fd_A[i]);
							  fprintf(fp,"0x02 %s terminate\n",fd_C[i]);
							
			      conn_amount--;
                             //FD_CLR(fd_A[i],&recvfd);
                             memset(sendBuf,0,sizeof(sendBuf));
                             sprintf(sendBuf,"0x02 %s leaves the chat\n",fd_C[i]);
							
                             /////////tell every one someone leaves
                             for(j=0;j<MAX_CON_NO;j++)
                             {
                                 if(fd_A[j]!=0&&i!=j)
                                 {
                                     fprintf(fp,"Send Data to %d,",fd_A[j]);
                                      if((sendSize=send(fd_A[j],sendBuf,strlen(sendBuf),0))!=strlen(sendBuf))
                                     {
                                         perror("fail");
                                         exit(1);
                                     }
                                     else
                                     {
                                         fprintf(fp,"Success\n");
                                     }
                                     
                                 }
                                 
                             }
                             memset(sendBuf,0,sizeof(sendBuf));
                              /////////////////////////
                             fd_A[i]=0;
                             strcpy(fd_C[i],"\0");
			    
                         }
                       
                         else
                         {
                             sprintf(temp,"%s says:",fd_C[i]);
							 strcat(temp,recvBuf);
							 strcpy(recvBuf,temp);
							 fprintf(fp,"%s\n",recvBuf);
                             /*send datas to client*/
                            for(j=0;j<MAX_CON_NO;j++)
                             {
                                 if(fd_A[j]!=0&&i!=j)
                                 {
                                     fprintf(fp,"Send Data to %d,",fd_A[j]);
                                     if((sendSize=send(fd_A[j],temp,strlen(temp),0))!=strlen(temp))
                                     {
                                         perror("fail");
                                         exit(1);
                                     }
                                     else
                                     {
                                         fprintf(fp,"Success\n");
                                     }
                                 }
                             }
                           
                             memset(recvBuf,0,MAX_DATA_SIZE);
							 memset(temp,0,MAX_DATA_SIZE);
                         }
                     }
                 }
                 break;
         }
		 
     }
  
     return 0;
 }
