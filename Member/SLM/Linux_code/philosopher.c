#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include <pthread.h>
#include <errno.h>
#include <unistd.h>
#include <math.h>

pthread_mutex_t chopstick[6] ;

void get(int *left,int *right,char phi) 
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
    get(&left,&right,phi);
    for(;;)
    {
        usleep(10);
        pthread_mutex_lock(&chopstick[left]);
        printf("Philosopher %c fetches chopstick %d\n", phi,left);
        if (pthread_mutex_trylock(&chopstick[right]) == EBUSY)
        {
            pthread_mutex_unlock(&chopstick[left]);
            continue;
        }
        printf("Philosopher %c fetches chopstick %d\n", phi,right);
        printf("Philosopher %c is eating.\n",phi);
        usleep(10);
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
        pthread_mutex_init(&chopstick[i],NULL);
        pthread_create(&A,NULL, eat_think, (void *)"A");
        pthread_create(&B,NULL, eat_think, (void *)"B");
        pthread_create(&C,NULL, eat_think, (void *)"C");
        pthread_create(&D,NULL, eat_think, (void *)"D");
        pthread_create(&E,NULL, eat_think, (void *)"E");
        
        pthread_join(A,NULL);
        pthread_join(B,NULL);
        pthread_join(C,NULL);
        pthread_join(D,NULL);
        pthread_join(E,NULL);
        
        return 0;
}
