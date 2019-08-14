#include<stdio.h>
int main()
{
	int x,n,i=0;
	scanf("%d",&n);
	i=0;
	x=1;
	while((n+1)>x)
	{
		x*=2;
		i++;
	}
	printf("%d",i);


	return 0;
}
