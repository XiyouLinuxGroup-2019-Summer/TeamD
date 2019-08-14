#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include <pthread.h>
#include <errno.h>
#include <unistd.h>
#include <math.h>

pthread_mutex_t chopstick[6] ;

void get(int *left,int *right,char phi) 
//不同的科学家，对应的左右手筷子不一样
{
    switch (phi)
    {
    case 'A':
        *left = 5;
        *right = 1;
        break;
    case 'B':
        *left = 1;
        *right = 2;
        break;
    case 'C':
        *left = 2;
        *right = 3;
        break;
    case 'D':
        *left = 3;
        *right = 4;
        break;
    case 'E':
        *left = 4;
        *right = 5;
        break;
    }
}

void *eat_think(void *arg)
{
    char phi = *(char *)arg;
    int left,right;
    get(&left,&right,phi);//获取筷子的编号【？
    for(;;)
    {
        usleep(10);//思考十秒
        pthread_mutex_lock(&chopstick[left]);//锁住左边筷子
        printf("Philosopher %c fetches chopstick %d\n", phi,left);
	//如果右边筷子被锁？释放左边，防止死锁
        if (pthread_mutex_trylock(&chopstick[right]) == EBUSY)
        {
            pthread_mutex_unlock(&chopstick[left]);
            continue;//从这儿进入下一次循环（抢左筷子，判断右筷子...）
        }
	//抢到了：
        printf("Philosopher %c fetches chopstick %d\n", phi,right);
        printf("Philosopher %c is eating.\n",phi);
        usleep(10);//进餐
	//之后释放两个筷子
        pthread_mutex_unlock(&chopstick[left]);
        printf("Philosopher %c release chopstick %d\n", phi,left);
        pthread_mutex_unlock(&chopstick[right]);
        printf("Philosopher %c release chopstick %d\n", phi,right);
    }
}

int main()
{
    pthread_t A,B,C,D,E; //5个哲学家
    int i;
    for (i = 0; i < 5; i++)
	//初始化互斥锁～
        pthread_mutex_init(&chopstick[i],NULL);
	//创建不同的哲学家为不同的线程
        pthread_create(&A,NULL, eat_think, (void *)"A");
        pthread_create(&B,NULL, eat_think, (void *)"B");
        pthread_create(&C,NULL, eat_think, (void *)"C");
        pthread_create(&D,NULL, eat_think, (void *)"D");
        pthread_create(&E,NULL, eat_think, (void *)"E");
        //等待它们的调用终止
        pthread_join(A,NULL);
        pthread_join(B,NULL);
        pthread_join(C,NULL);
        pthread_join(D,NULL);
        pthread_join(E,NULL);
        
        return 0;
}

