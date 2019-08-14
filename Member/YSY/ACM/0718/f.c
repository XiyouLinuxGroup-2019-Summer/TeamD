#include<stdio.h>
#include<string.h>
void maya(void);
void maya(void)
{	char ha[19][10]={"pop","no","zip","zotz","tzec","xul","yoxkin","mol","chen","yax","zac","ceh", "mac","kankin","muan","pax","koyab","cumhu","uayet"};
	char tz[20][10]={"imix", "ik", "akbal", "kan", "chicchan", "cimi", "manik", "lamat", "muluk", "ok", "chuen", "eb", "ben", "ix", "mem", "cib", "caban", "eznab","canac", "ahau"};
	char hm[10];
	int hd,hy;
	long ty=0,td=0,num=0;//t年，余下日数
	char tm[10];
	int i=-1,x=1;
/*
	for(num=1;num<1000;num++)
	{
		x=(num%260)/20;
		printf("%10s",tz[x]);
		ty++;
		if((ty%5)==0)
			printf("\n");
	
	}	
	
*/	
	scanf("%d. %s %d",&hd,&hm,&hy);

	ty=(hy*365)/260;
	num=(hy*365)%260;

	while(x)
	{
		i++;
		x=strcmp(hm,ha[i]);	
	}
	printf("%d->%s\n",i,ha[i]);
	num+=i*20+hd+1;
	ty+=num/260;
	num=num%260;
	printf("%d\n",num);
	x=num/20;
	if((num%20)==0)
		x=19;
	strcpy(tm,tz[x]);
	td=(num)%13;
	if(td==0)
		td=13;
	printf("%ld %s %ld\n",td,tm,ty);



	return ;
}

int main()
{
	int i,n;
	scanf("%d",&n);
	for(i=0;i<n;i++)
		maya();


	return 0;
}


