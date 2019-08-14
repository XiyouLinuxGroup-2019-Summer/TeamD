#include<stdio.h>
#include<string.h>
#include<stdlib.h>
int cc(int x,int a,int b);

int main()
{
	int a,b;
	int *str;
	char *out;
	int i,n,x,num=0;

	scanf("%d",&n);
	str=(int*)malloc(n*sizeof(int));//动态数组喔
	out=(char*)malloc(n*sizeof(char));
	for(i=0;i<n;i++)
	{
		scanf("%d",&str[i]);
	}

	x=0;
	a=0;
	b=x-1;

	return 0;

}
	
int cc(int x,int a,int b)
{	if(x<str[a])
		if(x<str[b]&&str[b]<str[a]){
			x=str[b];
  			out[i]='R';

		}
		else{
			x=str[a];
			out[i]='L';
		}
	else 
		if(x<str[b]){
			x=str[b];
			out[i]='R';
		}
		else
			break;

	








	return 0;
}
