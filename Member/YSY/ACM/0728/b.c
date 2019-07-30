#include<stdio.h>

int main()
{
	int n,i;
	scanf("%d",&n);

	
	for(i=0;;i++)	
	{	
		n=n/2;
		if(!n)
			break;
	}
	printf("%d",i+2);
	
	return 0;
}
