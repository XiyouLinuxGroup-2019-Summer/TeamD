#include<stdio.h>
#include<string.h>
int main()
{
	int i,j,m,x=0;
	char b[200],d[200];
	char c;
	m=0;
	
	for(i=0;i<=301;i++)
	{
		//scanf("%[^\n]",b);
		gets(b);
		if(!(strcmp(b,"START")))	
		{	m=1;continue;
		}
		else if(!(strcmp(b,"END")))	
		{	m=0;
			for(j=0;j<x;j++)
			{
			c=d[j];
			if(c<65||c>90)
				continue;
			if(c<70)
				d[j]=c+21;
			else 
				d[j]=c-5;	
			}
		printf("%s\n",d[i]);
		x=0;
		}
		if(m)
		{x+=strlen(b);strcat(d[n],b);
		}
		if(!(strcmp(b,"ENDOFINPUT")))	
			break;	
		
	}
	return 0;
}
