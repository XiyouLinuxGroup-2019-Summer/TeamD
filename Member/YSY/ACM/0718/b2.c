#include<stdio.h>

int main()
{
	int t,n,x;
	int i,j,num;
	int n1,n2;
	scanf("%d",&t);
	for(i=0;i<t;i++)
	{
		num=0;
		n1=0;
		n2=0;
		scanf("%d",&n);
		for(j=0;j<n;j++)
		{
			scanf("%d",&x);
			x=x%3;
			if(x==0)
				num++;
			else if(x==1)
				n1++;
			else
				n2++;
		}
		if(n1<n2)
		{
			num+=n1;
			num+=(n2-n1)/3;
		}
		else
		{
			num+=n2;
			num+=(n1-n2)/3;
		}
	printf("%d\n",num);
	}
	return 0;
}
