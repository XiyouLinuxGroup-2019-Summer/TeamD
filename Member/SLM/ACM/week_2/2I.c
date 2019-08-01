#include <stdio.h>
#include <stdlib.h>

struct node 
{
    int start;
    int aim;
}move[1000];

int cmp(const void *a,const void *b)
{
    struct node *aa=(struct node *)a;
    struct node *bb=(struct node *)b;
    return(((aa->aim)>(bb->aim))?1:-1);
}

int main()
{
    int t,n,i,j,count,ending;
    scanf("%d",&t);
    while(t--)
    {
        int test[1000]={  };
        scanf("%d",&n);
        for(i=0;i<n;i++)
        {
            scanf("%d %d",&move[i].start,&move[i].aim);
        }

        qsort(move,n,sizeof(move[0]),cmp);  
        

        for(i=0;i<n;i++)
        {
            for(j=move[i].start;j<=move[i].aim;j++)
                test[j-1]++;
        }


        ending=0;
        for(i=move[0].start;i<j;i++)
        {
            if(test[i]>ending)
                ending=test[i];
        }

        printf("%d\n",ending*10);

        /*
        count=1;
        ending=move[0].aim;
        for(i=1;i<n;i++)
        {
            if(ending>move[i].start)
            {
                count++;
                ending=move[i].aim;
            }
            else
            {
                ending=move[i].aim;
                continue;
            }
        }
        printf("%d\n",count*10);
        */
    }
    return 0;
}
