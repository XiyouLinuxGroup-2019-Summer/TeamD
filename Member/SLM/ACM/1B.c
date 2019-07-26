#include <stdio.h>

int main()
{
    int t,n,a,b,num;
    int q[110];
    int i;
    scanf("%d",&t);
    while(t--)
    {
        num=0;
        a=0;
        b=0;
        scanf("%d",&n);
        for(i=0;i<n;i++)
        {
            scanf("%d",&q[i]);
        }


        for(i=0;i<n;i++)
        {
            if(q[i]%3 == 0)
            {
                num++;
                continue;
            }
            else if(q[i]%3 == 1)
            {
                a++;
                continue;
            }
            else if(q[i]%3 == 2)
            {
                b++;
                continue;
            }
        }
        if(a>=b)
        {
            num+=b;
            num+=(a-b)/3;
        }
        if(b>a)
        {
            num+=a;
            num+=(b-a)/3;
        }
    printf("%d\n",num);
    }
    return 0;
}
