#include<stdio.h>
#include <stdlib.h>


int comp(const void *a,const void *b)
{
    return (*(int *)a - *(int *)b);
}
int main()
{
     
    int n,i,j=0,sum=1,t;
    scanf("%d",&n);
    int cont;
    int a[n + 4],b[n + 4];
    for(i=1;i<=n;i++)
    {
        scanf("%d",&a[i]);
    }
    qsort(a+1,n+1,sizeof(int),comp);
  
    for(i=1;i<=n;i++)
    {
       if(a[i]<=i)
        {
            cont++;
            if(a[i]!=a[i+1] && a[i+1]<=i+1)
            {
                sum++;
            }
        }
        
    }
    if(cont==0)
            sum = 0;
     printf("%d\n",sum);

} 
