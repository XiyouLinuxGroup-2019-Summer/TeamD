#include<stdio.h>
#include<string.h>
int main()
{
	int i,j,m,x=0;
	char b[200],d[200];
	char c;
	m=0;
	strcpy(d,"\0");
	for(i=0;i<=301;i++)
	{
		//scanf("%[^\n]",b);
		gets(b);
		if(!(strcmp(b,"START")))	
		{	m=1;continue;
		}
		else if(!(strcmp(b,"END")))	
		{
			for(j=0;j<x;j++)
			{
			c=d[j];
			if(c<'A'||c>'Z')
				continue;
			if(c<'F')
				d[j]=c+21;
			else 
				d[j]=c-5;	
			}
			printf("%s\n",d);
			x=0;
			m=0;
			strcpy(d,"\0");
		}
		if(m)
		{x+=strlen(b);strcat(d,b);
		}
		if(!(strcmp(b,"ENDOFINPUT")))	
			break;	
		
	}
	return 0;
}

