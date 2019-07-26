#include<stdio.h>
#include<stdlib.h>
#include<string.h>
void ship(void);
void ship(void)
{
	char L[]="left",R[]="right";
	int l,n,i,fnow=0,lnow,x=0;
	int *car;
	int *from;
	char ch[10];
	scanf("%d %d",&l,&n);
	l=l*100;
	car=(int*)malloc(n*sizeof(int));
	from=(int*)malloc(n*sizeof(int));
	for(i=0;i<n;i++)
	{
		scanf("%d %s",&car[i],ch);
		if(!strcmp(ch,L))		
			from[i]=0;
		else
			from[i]=1;
	}	
	lnow=l;
	for(i=0;i<n;)
	{
		if(fnow==from[i]&&lnow>=car[i])
		{	lnow-=car[i];
			i++;
			continue;
		}
		x++;
		fnow=x%2;
		lnow=l;
	}
	if(lnow!=l)
		x++;
	printf("%d\n",x);

	free(car);
	free(from);
}

int main()
{
	int i,n;
	scanf("%d",&n);
	for(i=0;i<n;i++)
		ship();	
 	return 0;
}
