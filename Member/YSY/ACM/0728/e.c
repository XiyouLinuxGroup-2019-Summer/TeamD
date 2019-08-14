#include<stdio.h>
#include<stdlib.h>
int coomp ( const void *a , const void *b ) 
{ 
  return *(int *)a - *(int *)b; 
} 

int main()
{
	int n,m,i,sum=0;
	scanf("%d %d",&n,&m);
	int a[n];
	for(i=0;i<n;i++)
	{	scanf("%d",&a[i]);	
	}
	qsort(a,n,sizeof(int),coomp);
	
	for(i=0;i<m;i++){
		if(a[i]>0)
			break;
		sum+=a[i];
	}
	printf("%d\n",-sum);



	return 0;
}
