#include <stdio.h>

int main()
{
    int n,a,b,c,t,i,money=0;
    scanf("%d %d %d %d %d",&n,&a,&b,&c,&t);
    int T[10000];
    for(i=0;i<n;i++)
        scanf("%d",&T[i]);
    if(b>=c)
    {
        money=n*a;   //如果每分钟盈利值小于等于亏损值，就直接读取消息
    }
    else
    {               //如果每天盈利值大于亏损值，就在t分钟之后读取所有消息
        money=n*a;
        for(i=0;i<n;i++)
        {
            money+=(t-T[i])*(c-b);
        }
    }
    printf("%d\n",money);
    return 0;
}

