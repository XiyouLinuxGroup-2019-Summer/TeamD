#include<stdio.h>
#include<string.h>
int m;
int main()
{
	int i,j,n,x;
	char a[100][200];
	char b[200];
	char c;
	m=0;
	n=0;
	for(i=0;i<=301;i++)
	{
		//scanf("%[^\n]",b);
		gets(b);
		if(!(strcat(b,"START")))	
		{	m=1;continue;
		}
		else if(!(strcmp(b,"END")))	
			m=0;
		if(m)
		{n++;strcpy(a[n],b);
		}
		if(!(strcmp(b,"ENDOFINPUT")))	
			break;	
		
	}

	for(i=0;i<=n;i++)
	{
		x=strlen(a[i]);
	//	printf("所以，a[%d]\n%s会变成：\n",i,a[i]);
		for(j=0;j<x;j++)
		{
			c=a[i][j];
			if(c<65||c>90)
				continue;
			
			if(c<70)
				a[i][j]=c+21;
			else 
				a[i][j]=c-5;	
		
		}
		printf("%s\n",a[i]);
	}
	return 0;
}
