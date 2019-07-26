#include<stdio.h>
#include<stdlib.h>
int coomp ( const void *a , const void *b ) 
{ 
  return *(int *)a - *(int *)b; 
} 

int main()
{ 
	long i,n,num=0,x=1;
	scanf("%ld",&n);
	int a[n+1];
	for(i=1;i<=n;i++)
	{	scanf("%ld",&a[i]);	
	}
	qsort(a+1,n+1,sizeof(int),coomp); 
printf("%d",a[1]);
	for(i=1;i<=n;i++)
	{
		if(a[i]>=x){
			num++;
			x++;
		}	
	}

	printf("%ld\n",num);

	return 0;
}
