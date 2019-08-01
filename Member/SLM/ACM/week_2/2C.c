#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main()
{
    int n,d,i,j;
    char *a;
    int b[100]={};
    int t=0,max=0;

    a=(char *)malloc(sizeof(char *)*500);
    scanf("%d %d",&n,&d);
    for(i=0;i<d;i++)
    {
        scanf("%s",a);
        for(j=0;j<n;j++)
        {
            if(a[j] == '0')
                b[i]=1;
        }
    }

    for(i=0;i<d;i++)
    {
        if(b[i] == 0)
            t=0;
        if(b[i] == 1)
        {
            t++;
            if(t>max)
                max=t;
        }
    }

    printf("%d\n",max);
    return 0;
}
