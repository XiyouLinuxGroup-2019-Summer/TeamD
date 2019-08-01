#include <stdio.h>
#include <stdlib.h>

int cmp(const void *a,const void *b)
{
    return(*(int *)a-*(int *)b);
}

int main()
{
    int n,m,i,money[1000],profit=0;
    scanf("%d %d",&n,&m);
    for(i=0;i<n;i++)
    {
        scanf("%d",&money[i]);
    }

    qsort(money,n,sizeof(money[0]),cmp);

    for(i=0;i<m;i++)
    {
        if(money[i]<0)
        profit+=money[i];
    }
    printf("%d\n",-profit);
    return 0;
}
