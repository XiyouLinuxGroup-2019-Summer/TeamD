#include<stdio.h>
#include<stdlib.h>
int coomp ( const void *a , const void *b ) 
{ 
  return *(int *)a - *(int *)b; 
} 

int main()
{
	int n,i,num=0,x=1;
	scanf("%d",&n);
	int a[n+1];
	for(i=1;i<=n;i++)
	{	scanf("%d",&a[i]);	
	}
	qsort(a+1,n+1,sizeof(int),coomp); 

	for(i=1;i<=n;i++)
	{
	//	printf("a[%d]=%d,x=%d",i,a[i],x);
		if(a[i]>=x){
			num++;
			x++;
		}	
	}

	printf("%d\n",num);

	return 0;
}
