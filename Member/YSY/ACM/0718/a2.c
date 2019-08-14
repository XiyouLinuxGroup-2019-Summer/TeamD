#include<stdio.h>

int acc(long long int x);
int acc(long long int x)
{
	int j,r=0;

	for(j=5;j>1;j--)
	{	while(x%j==0)
		{	
			x=x/j;
			x*=j-1;
			r++;			
		}	
		if(j==5)
			j--;
	}
	if (x!=1)
		return (-1);
	else
		return (r);


	
}
int main()
{
	int n,i;
	long long int a;
	scanf("%d",&n);
	for(i=0;i<n;i++)
	{
		scanf("%lld",&a);		
		printf("%d\n",acc(a));
	}
//	printf("%s",a);

	return 0;
}
