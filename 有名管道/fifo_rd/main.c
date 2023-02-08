#include<stdio.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<string.h>
#include<errno.h>
#include<unistd.h>

#define BUF_SIZE 64
#define TIME_NEXT 50

enum TASK_PHONE_STATE
{
    TASK_PHONE_STATE_NONE=0,
    TASK_PHONE_STATE_RING,
    TASK_PHONE_STATE_TALK,
    TASK_PHONE_STATE_HANGUP,
};

int phone_state=TASK_PHONE_STATE_NONE;

void set_state(int state)
{
    phone_state=state;
}

int get_state()
{
    return phone_state;
}
int main()
{
    int time=0;
    int len=0;
    //打开文件fifoa
    int fdr = open("/tmp/fifoa",O_RDONLY|O_NONBLOCK,0666);
    if(-1 == fdr)
    {
        perror("open error\r\n");
        return -1;
    }
    printf("open fifoa OK!\r\n");
    //打开文件fifob
    int fdw = open("/tmp/fifob",O_WRONLY|O_NONBLOCK,0666);
    if(-1 == fdw)
    {
        perror("open error");
        return -1;
    }
    printf("open fifob OK!\r\n");

    char buf[BUF_SIZE] = {0};
    while(1)
    {
        //先读
        memset(buf,0,sizeof(buf));
        if((len=read(fdr,buf,sizeof(buf)))>0)
        {
            printf("read %s from wrfifo\n",buf);
            set_state(TASK_PHONE_STATE_HANGUP);
        }

        time++;
        if(time>=TIME_NEXT)
        {
            time=0;
            if(get_state()==TASK_PHONE_STATE_RING)
            {
                set_state(TASK_PHONE_STATE_TALK);
            }
            else if(get_state()==TASK_PHONE_STATE_TALK)
            {
                set_state(TASK_PHONE_STATE_HANGUP);
            }
            else
            {
                set_state(TASK_PHONE_STATE_RING);
            }
            printf("Current state is %d\n",get_state());
        }

        if(get_state()==TASK_PHONE_STATE_HANGUP)
        {
            strcpy(buf,"Send quit msg!");
            if((len=write(fdw,buf,sizeof(buf)))==-1)
            {
                printf("The rdFIFO has not been read yet.Please try later\n");
            }
            else
            {
                printf("write %s to the rdFIFO\n",buf);
            }
            break;
        }
        usleep(100*1000);

    }

    close(fdr);
    close(fdw);
}
