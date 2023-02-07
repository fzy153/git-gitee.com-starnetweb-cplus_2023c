#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <unistd.h>

#define MAX_LISTEN_NUM 5
#define SEND_BUF_SIZE 100
#define RECV_BUF_SIZE 100
#define SERVER_PORT 7801

int main(int argc,char *argv[])
{
    int sock_fd=0;
    char sendbuf[SEND_BUF_SIZE]={0};
    char recvbuf[RECV_BUF_SIZE]={0};

    int sendlen=0;
    int recvlen=0;
    int retlen=0;

    struct sockaddr_in ser_addr;
    memset(&ser_addr,0,sizeof(ser_addr));
    ser_addr.sin_family=AF_INET;
    inet_aton("127.0.0.1",(struct in_addr*)&ser_addr.sin_addr);
    ser_addr.sin_port=htons(SERVER_PORT);

    sock_fd=socket(AF_INET,SOCK_DGRAM,0);
    if(sock_fd<0)
    {
        printf("create socket failed\n");
        exit(1);
    }

    socklen_t len=sizeof(ser_addr);
    while(1)
    {
        gets(sendbuf);
        sendlen=strlen(sendbuf);
        retlen=sendto(sock_fd,sendbuf,sendlen,0,(struct sockaddr*)(&ser_addr),len);
        printf("send data is:%s\n",sendbuf);

        retlen=recvfrom(sock_fd,recvbuf,RECV_BUF_SIZE,0,(struct sockaddr*)(&ser_addr),&len);
        if(retlen<0)
        {
            printf("receive data error\n");
            close(sock_fd);
            exit(1);
        }
        printf("receive data is:%s\n",recvbuf);
    }
    close(sock_fd);
    return 0;
}

