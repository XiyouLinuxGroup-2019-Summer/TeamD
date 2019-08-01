#include <stdio.h>
#include <stdlib.h>

struct node
{
    int start;
    int end;
}show[1000];

int cmp(const void *a,const void *b)
{
    struct node *aa=(struct node *)a;
    struct node *bb=(struct node *)b;
    return(((aa->end)>(bb->end))?1:-1);
}

int main()
{
    int n,i,count,ending;
    while(1)
    {
        scanf("%d",&n);
        if(n == 0)
            break;
        else
        {
            for(i=0;i<n;i++)
            {
                scanf("%d %d",&show[i].start,&show[i].end);
            }

            qsort(show,n,sizeof(show[0]),cmp);

            count=1;
            ending=show[0].end;
            for(i=1;i<n;i++)
            {
                if(show[i].start>=ending)
                {
                    count++;
                    ending=show[i].end;
                }
            }
            printf("%d\n",count);
        }
    }
    return 0;
}
