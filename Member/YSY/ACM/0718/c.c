#include<stdio.h>
int main()
{
	int n,i,sum,a;
	scanf("%d",&n);
	
	sum=0;
	while(scanf("%d",&a)!='\n')
	{
		sum+=a;	
	}
	
	for(i=1;i<n;i++)
	{
		sum-=(n+1-i);
		if(sum<=0)
			break;
			
	}

	printf("%d",i);

	return 0;
}
