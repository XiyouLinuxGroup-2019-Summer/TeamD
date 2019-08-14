#include<stdio.h>
#include<stdlib.h>
#include<string.h>
typedef struct node 
{
	int s;
	int e;
}tele;

int coomp(const void *a,const void *b)
{
    tele *aa=(struct node *)a;
    tele *bb=(struct node *)b;
    return(((aa->e)>(bb->e))?1:-1);
}


int main()
{
	int n,i,now,num;
	tele tv[200];
	memset(tv,0,200);
	while(1)
	{
		scanf("%d",&n);
		if(!n)
			break;
		for(i=0;i<n;i++)
		{
			scanf("%d %d",&tv[i].s,&tv[i].e);
		}

		qsort(tv,n,sizeof(tele),coomp);
		
		num=1;
		now=tv[0].e;
		for(i=1;i<n;i++)
            	{
                	if(tv[i].s>=now)
                	{
                    	num++;
                   	now=tv[i].e;
                	}	
            	}	
		
		printf("%d\n",num);
	}

	return 0;
}


