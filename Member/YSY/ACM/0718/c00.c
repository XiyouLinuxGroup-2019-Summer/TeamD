
#include<stdio.h>
#include <stdlib.h>


int comp(const void *a,const void *b)
{
    return (*(int *)a - *(int *)b);
}
int main()
{
     
    int n,j,i;
    while(scanf("%d",&n) != EOF)
    {
    int a[n];
    for(i=0;i<n;i++)
    {
        scanf("%d",&a[i]);
    }
    qsort(a,n,sizeof(int),comp);
 
   
    for(i=0,j=1;i<n;i++)
    {

       if(a[i]>=j)
        {
            j++;
        }
        
    }
   
     printf("%d\n",j-1);

     return 0;
    }
} 

