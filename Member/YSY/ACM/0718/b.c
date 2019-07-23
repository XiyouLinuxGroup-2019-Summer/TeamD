#include<stdio.h>

int main()
{
	int t,n,a[10],b[10];
	int i,j,num;
	int n1,n2;
	scanf("%d",&t);
	for(i=0;i<t;i++)
	{
		num=0;
		n1=0;
		n2=0;
		a[10]=0;
		scanf("%d",&n);
		for(j=0;j<n;j++)
		{
			scanf("%d",&a[j]);
			if(a[j]%3==0)
			{
				a[j]=0;
				num++;
			}
			else
				a[j]=a[j]%3;
		}
		
		//paixu
		for(j=0;j<n;j++)
		{
			if(a[j]==2)
				n2++;
			else if(a[j]==1)
				n1++;
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
		b[i]=num;

	}
	for(i=0;i<t;i++)
		printf("%d\n",b[i]);
	return 0;
}
