#include <stdio.h>
#include <stdlib.h>

struct node
{
    float J;
    float F;
    float rate;
}Room[10000];

int cmp(const void *a,const void *b)
{
    struct node *aa=(struct node *)a;
    struct node *bb=(struct node *)b;
    return (((bb->rate)>(aa->rate))?1:-1);
}

int main()
{
    int m,n,i;
    double count;
    while(1)
    {
        count=0;
        scanf("%d %d",&m,&n);
        if((m==-1)&&(n==-1))
            break;
        for(i=0;i<n;i++)
        {
            scanf("%f %f",&Room[i].J,&Room[i].F);
            Room[i].rate=(Room[i].J)/(Room[i].F);
        }

        qsort(Room,n,sizeof(Room[0]),cmp);

        for(i=0;i<n;i++)
        {
            if(m>=Room[i].F)
            {
                m-=Room[i].F;
                count+=Room[i].J;
                continue;
            }
            if(m<Room[i].F&&m>0)
            {
                count+=Room[i].rate*m;
                m=0;
            }
        }
        printf("%.3lf\n",count);
    }
    return 0;
}
