#include<stdio.h>
#include<stdlib.h>
#include<string.h>
void maya(int Nu);
char ha[19][10]={"pop","no","zip","zotz","tzec","xul","yoxkin","mol","chen","yax","zac","ceh", "mac","kankin","muan","pax","koyab","cumhu","uayet"};
char tz[20][10]={"imix", "ik", "akbal", "kan", "chicchan", "cimi", "manik", "lamat", "muluk", "ok", "chuen", "eb", "ben", "ix", "mem", "cib", "caban", "eznab","canac", "ahau"};
long ty[100],td[100];
char tm[100][10];


void maya(int Nu)
{
	char hm[10];
 	long num=0;//t年，余下日数
	int i=-1,x=1;
	int hd,hy;
	scanf("%d. %s %d",&hd,&hm,&hy);
   	ty[Nu]=0,td[Nu]=0;
	while(x)
	{
		i++;
		x=strcmp(hm,ha[i]);	
	}
	num=hy*365+i*20+hd+1;
	
	ty[Nu]=num/260;
	num=num%260;

	x=(num)%20;
	if(x==0)
		x=20;
	strcpy(tm[Nu],tz[x-1]);
	td[Nu]=(num)%13;
	if(td[Nu]==0)
		td[Nu]=13;
	return ;
}

int main()
{
	int i,n;
	scanf("%d",&n);
	printf("%d",n);
	for(i=0;i<n;i++)
		maya(i);

	for(i=0;i<n;i++)
		printf("%ld %s %ld\n",td[i],tm[i],ty[i]);
	return 0;
}
