#include <stdio.h>
#include <stdlib.h>

typedef struct node
{
    int day;
    int score;
}homework;

int cmp(const void *a,const void *b)
{
    struct node *aa=(struct node *)a;
    struct node *bb=(struct node *)b;
    return(((bb->score)>(aa->score))?1:-1);
}

int main()
{
    int times,num,sum,i,n;
    scanf("%d",&times);
    while(times--)
    {
	scanf("%d",&n);
	homework hw[n];
        sum=0;
        
	for(i=0;i<n;i++)
        {
            scanf("%d",&hw[i].day);
        }
        for(i=0;i<n;i++)
        {
            scanf("%d",&hw[i].score);
        }

        qsort(hw,n,sizeof(homework),cmp);

    }
	return 0;
}



