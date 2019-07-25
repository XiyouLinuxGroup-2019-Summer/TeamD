#include<stdio.h>
#include<stdlib.h>
int coomp ( const void *a , const void *b ) 
{ 
  return *(int *)a - *(int *)b; 
} 

int main()
{
	int n,i,num=0;
	scanf("%d",&n);
	int a[n+1];
	for(i=1;i<=n;i++)
	{	scanf("%d",&a[i]);	
	}
	qsort(a+1,n+1,sizeof(int),coomp); 
	a[0]=0;
	for(i=1;i<=n;i++)
	{
		if((a[i]<=i)&&(a[i]!=a[i-1]))
		{
			num++;
		}	
	}

	printf("%d\n",num);

	return 0;
}
