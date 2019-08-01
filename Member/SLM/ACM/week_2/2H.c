#include <stdio.h>
#include <stdlib.h>

struct node
{
    int day;
    int grade;
}work[10000];

int cmp(const void *a,const void *b)
{
    struct node *aa=(struct node *)a;
    struct node *bb=(struct node *)b;
    return(((bb->grade)>(aa->grade))?1:-1);
}

int main()
{
    int t,i,n,min,tmp=99;
    scanf("%d",&t);
    while(t--)
    {
        min=0;
        scanf("%d",&n);
        for(i=0;i<n;i++)
        {
            scanf("%d",&work[i].day);
        }
        for(i=0;i<n;i++)
        {
            scanf("%d",&work[i].grade);
        }

        qsort(work,n,sizeof(work[0]),cmp);

        int test[10000]={};
        for(i=0;i<n;i++)
        {
            if(test[work[i].day] == 0)
            {
                test[work[i].day]++;
                continue;
            }
            else
            {
                tmp=work[i].day;
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
                    min+=work[i].grade;
            }
        }
        printf("%d\n",min);
    }
    return 0;
}
