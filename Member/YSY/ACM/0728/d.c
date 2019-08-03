#include<stdio.h>
int main()
{
	int n,a,b,c,t,i,sum=0;
 	scanf("%d %d %d %d %d",&n,&a,&b,&c,&t);
	int ti[1001];
	for(i=0;i<n;i++)
		scanf("%d",&ti[i]);

	if(b>=c)
		sum=a*n;
	else{
 		for(i=0;i<n;i++)
			sum+=t-ti[i];
		sum*=(c-b);
		sum+=a*n;
	}
	printf("%d\n",sum);
	return 0;
}
