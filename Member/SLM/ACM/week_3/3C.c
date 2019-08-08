#include <stdio.h>
#include <math.h>
int a[20], n,count,c[11];
int num;


int check(int k)
{
    int i;
    for(i=1;i<k;i++)
    {
        if(abs(a[i]-a[k]) == abs(i-k) || (a[i] == a[k]))
            return 0;
    } 
        return 1;
}

int main(void)
{
    int k,N;
    n=11;
    while(n--)
    {
        k=1;
        a[1]=0;
        count = 0;
        while(k>0)
        {
            a[k]++;
            while((a[k] <= n)&&(check(k) == 0))
                a[k]++;
            if(a[k] <= n)
            {
                if(k == n)
                {
                    count++;
                }
                else
                {
                    k++;
                    a[k] = 0;
                }
            }
            else
            {
                k--;
            }
        }
        c[n] = count;
    }
    while(1)
    {
        scanf("%d",&N);
        if(N == 0)
            break;
        for(int i=1;i<11;i++)
        {
            if(N==i)
                printf("%d\n",c[N]);
        }
    }
    return 0;
}
