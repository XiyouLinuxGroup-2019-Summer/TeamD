#include<stdio.h>
#include<math.h>
int k,n;
int ef(int v);
int ef(int v)
{
	int sum=0,i,d;
	for(i=0;;i++)
		{
			d=v/(pow(k,i));
			if(!d)
				break;
			sum+=d;
		}
	if (sum>n)
		return 0;
	else 
		return 1;
}

int main()
{
	int x,v,a,b;
	scanf("%d",&n);
	scanf("%d",&k);
	a=1;
	b=n;
	printf("n=%d,k=%d\n",n,k);
	while(a!=b)
	{
		v=(a+b)/2;
		x=ef(v);
		if(x)
			a=v;
		else
			b=v;
	}
	
	printf("%d",v);
    return 0;
}
