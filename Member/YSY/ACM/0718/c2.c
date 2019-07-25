#include<stdio.h>
#include<stdlib.h>
int cmp ( const void *a , const void *b ) 
{ 
  return *(int *)a - *(int *)b; 
} 

int main()
{
	int n,i,num=0,xi;
	scanf("%d",&n);
	int a[n+1];
	for(i=1;i<=n;i++)
	{	scanf("%d",&a[i]);	
	}
	qsort(a+1,n+1,sizeof(int),cmp); 
	xi=0;

	for(i=1;i<=n;i++)
	{
		if(a[i]<=i&&xi!=a[i])
		{
			num++;
			xi=a[i];
		}	
	}

	printf("%d\n",num);

	return 0;
}
