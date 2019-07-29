#include <stdio.h>
#include <stdlib.h>

int acmp(const void *a,const void *b)
{
    return (*(int *)a - *(int *)b);
}

int main()
{
    long long int i,n,tmp=1,final=0;
    int a[200000];
    scanf("%lld",&n);
    for(i=0;i<n;i++)
    {
        scanf("%d",&a[i]);
    }
    qsort(a,n,sizeof(int),acmp);

    for(i=0;i<n;i++)
    {
        if(a[i]>=tmp)
        {
            tmp++;
            final++;
        }
    }
    printf("%lld\n",final);

    return 0;
}
