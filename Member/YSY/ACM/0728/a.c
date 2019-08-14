#include<stdio.h>
int main()
{
	int m,n,i;
	double a,b;
	double x,min;

	scanf("%d %d",&n,&m);
	scanf("%lf %lf",&a,&b);

	min=a/b;

	for(i=1;i<n;i++)
	{
		scanf("%lf %lf",&a,&b);
		x=a/b;
		if(min>x)
			min=x;
	
	}
	x=min*m;
	printf("%.8lf\n",x);




	return 0;
}
