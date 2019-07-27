#include <stdio.h>

int main()
{
    int n,d=1,i;

    scanf("%d",&n); 
    for(i=0;i<=30;i++)
    {
        if(i==0)
            d=1;
        else
            d*=2;

        if(d==n)
        {
            printf("%d\n",i+1);
            break;
        }
        else if(d>n)
        {
            printf("%d\n",i);
            break;
        }
    }
    return 0;
}


