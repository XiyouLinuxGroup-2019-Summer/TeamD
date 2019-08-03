#include <stdio.h>
#include <stdlib.h>

typedef struct node
{
    int day;
    int score;
}homework;



int cmp(const void *a,const void *b)
{
    homework *aa=(homework *)a;
    homework *bb=(homework *)b;
    return(((bb->score)>(aa->score))?1:-1);

}

int main()
{
    int t,i,n,sum,tmp=99;
   
    scanf("%d",&t);
    while(t--)
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

        qsort(hw,n,sizeof(hw[0]),cmp);
	int test[10000]={};

        
        for(i=0;i<n;i++)
        {
            if(test[hw[i].day] == 0)
            {
                test[hw[i].day]++;
                continue;
            }
            else
            {
                tmp=hw[i].day;
                while(tmp--)
                {
               		if(test[tmp] != 0)
                        	continue;
                 	else
                    	{
                       		test[tmp]++;
                        	break;
                    	}
                }
               if(tmp<1)
                   sum+=hw[i].score;
           }
        }
        printf("%d\n",sum);
    }
    return 0;
}
