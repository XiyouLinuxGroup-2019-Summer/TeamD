#include<stdio.h>
#include<string.h>
#include<stdlib.h>
int cc(int x,int a,int b);
char *out;
int i=0;
int x;

int main()
{
	int a,b;

	int n,num,s=0;
	int *str;

	scanf("%d",&n);
	str=(int*)malloc(n*sizeof(int));//动态数组喔
	out=(char*)malloc(n*sizeof(char));
	for(i=0;i<n;i++)
	{
		scanf("%d",&str[i]);
	}

	x=0;
	a=0;
	b=n-1;

	s=cc(x,str[a],str[b]);

	return 0;

}
	
int cc(int x,int a,int b)
{	if(x<a)
		if(x<b&&b<a){
			x=b;
  			out[i]='R';

		}
		else{
			x=a;
			out[i]='L';
		}
	else 
		if(x<b){
			x=b;
			out[i]='R';
		}
		else
			return 0;

