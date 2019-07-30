#include<stdio.h>
#include<stdlib.h>
int main()
{
	int m,n,i;
	//int *a;
	int a,b;
	double x,min;

	scanf("%d %d",&n,&m);
	scanf("%d %d",&a,&b);
	min=(double)a/b;

	for(i=1;i<n;i++)
	{
		x=(double)a/b;
		if(min>x)
			min=x;
	
			}
	x=min*m;
	printf("%.8lf\n",x);




	return 0;
}
