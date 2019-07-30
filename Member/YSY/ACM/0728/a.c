#include<stdio.h>
#include<stdlib.h>
int main()
{
	int m,n,i;
	//int *a;
	int a,b;
	float x,min;

	scanf("%d %d",&n,&m);
	scanf("%d %d",&a,&b);
	min=(float)a/b;
//	printf("%d,%d,%f\n",a,b,min);

	for(i=1;i<n;i++)
	{
//		scanf("%d %d",&a,&b);
		x=(float)a/b;
		if(min>x)
			min=x;
	
			}
	x=min*m;
	printf("%.8f\n",x);




	return 0;
}
