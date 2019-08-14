#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main()
{
	int n,d,i,j;
	int x=0,opp,max=0;//输出x,opp来了吗,辅助计数m
	scanf("%d %d",&n,&d);
	while(d--)
	{
		while(n--){
			scanf("%d",&opp);
			if(opp==0){
				x+=1;
				break;
			}
		}
		if(opp){
			if(max<x)
				max=x;
			x=0;
		}
	}	
	if(max<x)
		max=x;

	printf("%d",max);

	return 0;
}

