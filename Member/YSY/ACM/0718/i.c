#include<stdio.h>
#include<stdlib.h>
#include<string.h>
void ship(void);
void ship(void)
{
	char L[]="left",R[]="right";
	int l,n,i,fnow=0,lnow,x=0;//船长、车数、变量、现在船在？、航行次数
	int *car;
	int *from;
	char ch[10];
//	printf("请输入所要创建的一维动态数组的长度：");
	scanf("%d %d",&l,&n);
	l=l*100;
	car=(int*)malloc(n*sizeof(int));
	from=(int*)malloc(n*sizeof(int));
	for(i=0;i<n;i++)
	{
		scanf("%d %s",&car[i],ch);
		if(!strcmp(ch,L))//车从左边来，是0
			from[i]=0;
		else
			from[i]=1;
	}	
	lnow=l;
	for(i=0;i<n;)
	{
		if(fnow==from[i]&&lnow>=car[i])
		{	lnow-=car[i];
			printf("从%d装上车%d，此时l=%d\n",fnow,i,lnow);
			i++;
			continue;
		}
		//过河
		x++;
		fnow=x%2;
		lnow=l;
		printf("到%d,x=%d\n",fnow,x);
	}
	if(lnow!=l)
		x++;
	printf("%d\n",x);

	free(car);//释放第一维指针 
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
