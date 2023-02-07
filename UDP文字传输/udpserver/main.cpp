#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <unistd.h>

#define SEND_BUF_SIZE 100
#define RECV_BUF_SIZE 100
#define LISTEN_PORT 7801


int main(int argc,char *argv[])
{
    int listen_sock=0;
    struct sockaddr_in hostaddr;
    struct sockaddr_in clientaddr;
    char sendbuf[SEND_BUF_SIZE]={0};
    char recvbuf[RECV_BUF_SIZE]={0};

    int sendlen=0;
    int recvlen=0;
    int retlen=0;
    int leftlen=0;
    char *ptr=NULL;

    memset((void *)&hostaddr,0,sizeof(hostaddr));
    memset((void *)&clientaddr,0,sizeof(clientaddr));
    hostaddr.sin_family=AF_INET;
    hostaddr.sin_port=htons(LISTEN_PORT);
    hostaddr.sin_addr.s_addr=htonl(INADDR_ANY);

    listen_sock=socket(AF_INET,SOCK_DGRAM,0);
    if(listen_sock<0)
    {
        printf("create socket failed\n");
        exit(1);
    }


    if(bind(listen_sock,(struct sockaddr*)&hostaddr,sizeof(hostaddr))<0)
    {
        printf("bind socket failed\n");
        close(listen_sock);
        exit(1);
    }
    socklen_t len=sizeof(clientaddr);
    while(1)
    {
        retlen=recvfrom(listen_sock,recvbuf,RECV_BUF_SIZE,0,(struct sockaddr*)&clientaddr,&len);
        if(retlen<0)
        {
            printf("receive data error\n");
            close(listen_sock);
            exit(1);
        }
        printf("receive data is:%s\n",recvbuf);

        gets(sendbuf);
        sendlen=strlen(sendbuf);
        retlen=sendto(listen_sock,sendbuf,sendlen,0,(struct sockaddr*)&clientaddr,len);
        if(retlen<0)
        {
            printf("send data error\n");
            close(listen_sock);
            exit(1);
        }
        printf("send data is:%s\n",sendbuf);
    }

    close(listen_sock);
    return 0;
}
