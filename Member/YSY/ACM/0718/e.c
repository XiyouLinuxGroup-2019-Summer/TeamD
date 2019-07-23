#include <stdio.h>
#include <stdlib.h>

int main()
{
    	int i,x,n,m;
    	scanf("%d",&x);
	for(i=0;i<x;i++)
	{
    	scanf("%d %d",&n,&m);
    	if(n%(m+1)==0)
        	printf("second\n");
    	else
        	printf("first\n");
	
	}
    return 0;
}

