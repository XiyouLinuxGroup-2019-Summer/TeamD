#include <stdio.h>

int main()
{
    long long int a;
    int n;
    int i;

    scanf("%d",&n);
    while(n--)
    {
        i=0;
        scanf("%lld",&a);
        if(a == 1)
        {
            printf("0\n");
            continue;
        }
        while(a > 1)
        {
            if(a%2 == 0)
            {
                a=a/2;
                i++;
                continue;
            }
            else if(a%3 == 0)
            {
                a=a/3*2;
                i++;
                continue;
            }
            else if(a%5 == 0)
            {
                a=a/5*4;
                i++;
                continue;
            }
            else
            {
                printf("-1\n");
                break;
            }

        }
        if(a == 1)
        {
            printf("%d\n",i);
        }
        
    }

    return 0;
}
    

