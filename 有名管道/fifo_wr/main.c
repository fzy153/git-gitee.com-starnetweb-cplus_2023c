#include<stdio.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<string.h>
#include<errno.h>
#include<unistd.h>

#define BUF_SIZE 64
char buf[BUF_SIZE]={0};
int len=0;
int fdr=-1;
int fdw=-1;
static int get_char()
{
    fd_set rfds;
    struct timeval tv;
    int ch=0;
    FD_ZERO(&rfds);
    FD_SET(0,&rfds);
    tv.tv_sec=0;
    tv.tv_usec=1;
    if(select(1,&rfds,NULL,NULL,&tv)>0)
    {
        ch=getchar();
    }
    return ch;
}


int main()
{
    int ch=0;
    //创建有名管道文件1
    int ret = -1;
    ret = mkfifo("/tmp/fifoa",0666);
    if(-1 == ret && errno != EEXIST)
    {
        perror("mkfifoa error\r\n");
        return -1;
    }
    printf("create fifoa OK!\r\n");
    //创建有名管道文件2
    ret = mkfifo("/tmp/fifob",0666);
    if(-1 == ret && errno != EEXIST)
    {
        perror("mkfifob error\r\n");
        return -1;
    }
    printf("create fifob OK!\r\n");
    //打开文件fifoa
    fdw = open("/tmp/fifoa",O_WRONLY,0666);
    if(-1 == fdw)
    {
        perror("open errer\r\n");
        return -1;
    }
    printf("open fifoa OK!\r\n");
    //打开文件fifob
    fdr = open("/tmp/fifob",O_RDONLY|O_NONBLOCK,0666);
    if(-1 == fdr)
    {
        perror("open error\r\n");
        return -1;
    }
    printf("open fifob OK!\r\n");
    char buf[BUF_SIZE] = {0};
    while(1)
    {
        memset(buf,0,sizeof(buf));
        if((len=read(fdr,buf,sizeof(buf)))>0)
        {
            printf("read %s from rdfifo\n",buf);
            break;
        }
        //先写入
        ch=get_char();
        if(ch=='q')
        {
            memset(buf,0,sizeof(buf));
            strcpy(buf,"Send quit msg ok!");
            if((len=write(fdw,buf,sizeof(buf)))==-1)
            {
                printf("The FIFO has not been read yet.Please try later\n");
            }
            else
            {
                printf("write %s to the wrFIFO\n",buf);
            }
        }
        else if(ch=='c')
        {
            break;
        }

    }

    //关闭文件
    close(fdw);
    close(fdr);
    return 0;
}
