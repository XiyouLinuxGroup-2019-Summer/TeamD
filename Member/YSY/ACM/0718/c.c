#include<stdio.h>
int main()
{
	int n,i,sum,a[100];
	scanf("%d",&n);
	
	sum=0;
	for(i=1;i<=n;i++)
	{	scanf("%d",&a[i]);
		sum+=a[i];	
	}
	
	for(i=1;i<=n;i++)
	{
		sum-=(n+1-i);
		if(sum<=0)
			break;
			
	}

	printf("%d",i);

	return 0;
}
