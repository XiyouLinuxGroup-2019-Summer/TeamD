#include<stdio.h>
#include<stdlib.h>

typedef struct node{
	int cat;
	int mou;
	double rate;
}trade;

int coomp(const void *x,const void *y)
{
    trade *xx=(struct node *)x;
    trade *yy=(struct node *)y;
    return(((yy->rate)>(xx->rate))?1:-1);
}

int main()
{
	int m,n,i;
	double sum;	
	while(1)
	{
		scanf("%d %d",&m,&n);
		if(m==-1&&n==-1)
			break;
		trade a[n];
		sum=0;
		for(i=0;i<n;i++){
			scanf("%d %d",&a[i].mou,&a[i].cat);
			a[i].rate=(double)a[i].mou/a[i].cat;
		}
		
		qsort(a,n,sizeof(trade),coomp);

		for(i=0;i<n;i++)
		{
				if(m>=a[i].cat){
				m-=a[i].cat;
				sum+=a[i].mou;	
			}
			else{
				sum+=((double)m*a[i].rate);
				m=0;
				break;
			}
		}
		printf("%.3lf\n",sum);
	}
	return 0;
}
