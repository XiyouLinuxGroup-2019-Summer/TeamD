#include<stdio.h>
#include<math.h>
int k;
long n;
int ef(long v);
int ef(long v)
{
	long sum=0,i,d;
	for(i=0;;i++)
	{
		d=v/(pow(k,i));
		if(!d){
			if (sum>=n)
				return 0;//总数过多or刚好
			else 
				return 1;//数量不够
		}
			sum+=d;
	}
}

int main()
{
	long x,v,a,b;
	scanf("%ld",&n);
	scanf("%d",&k);
	a=1;
	b=n;
	while(a!=b)
	{
		v=(a+b)/2;
		if(v==a)
			break;
		x=ef(v);
	
		if(x)
			a=v;
		else
			b=v;
		
	}
	if(ef(a))
		v=b;
	else
		v=a;
	printf("%ld\n",v);
    return 0;
}
