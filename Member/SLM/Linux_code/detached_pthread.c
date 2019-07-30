#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <pthread.h>
#include <dirent.h>
#include <signal.h>
#include <time.h>
#include <sys/types.h>
#include <semaphore.h>

void *thread1(void *arg)
{
    while(1)
    {
        usleep(100 *1000);
        printf("thread1 is running..\n");
    }
    printf("Leave threa1!\n");
}

int main(int argc,char *arg[])
{
    pthread_t thid;

    pthread_create(&thid,NULL,(void *)thread1,NULL);
    pthread_detach(thid);  //使线程处于分离状态
    sleep(1);
    printf("Leave main thread\n");

    return 0;
}

/*
 * ./a.out
 * 运行结果：
thread1 is running..
thread1 is running..
thread1 is running..
thread1 is running..
thread1 is running..
thread1 is running..
thread1 is running..
thread1 is running..
thread1 is running..
Leave main thread
*/
