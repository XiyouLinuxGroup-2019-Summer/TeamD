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
		printf("NO.%d,d=%d,v=%d\n",i,d,v);
		if(!d){
			printf("//////////////////////\n");
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
	int x,v,a,b;
	scanf("%d",&n);
	scanf("%d",&k);
	a=1;
	b=n;
	printf("n=%d,k=%d\n",n,k);
	if(k==1){
		printf("%d\n",n);
		return 0;
	}
	while(a!=b)
	{
		printf("%d~%d\n",a,b);
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
//	else
//		v=a;

	printf("%d",v);
    return 0;
}
