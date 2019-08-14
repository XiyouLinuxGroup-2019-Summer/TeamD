#include<stdio.h>
int main()
{
	int k;
	int a,b;

	scanf("%d",&k);
	
	if(k>36){
		printf("-1");
	//	return 0;
	}
	
	else if(k==0){
		printf("1");
	}
	else
	{
		a=k/2;
		b=k%2;
		while(a--)
		{
		printf("8");
		}
		
		if(b)
			printf("0");
	}
	return 0;

}
