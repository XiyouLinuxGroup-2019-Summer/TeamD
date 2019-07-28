#include <stdio.h>
#include <math.h>

int main()
{
    int v,n,k,low=1,high,mid,sum,tmp,i;
    scanf("%d %d",&n,&k);
    high=n;
    while(low<=high)
    {
        //if(low==high)
          //  break;
        mid=(low+high)/2;

        //printf("mid:%d\n",mid);

        sum=0;
        i=0;
        while(++i)
        {
            tmp=(int)mid/pow(k,(i-1));
            //printf("tmp=%d\n",tmp);
            sum+=tmp;
            if(tmp <= 0)
                break;
        }
        //printf("i=%d\n",i);
        if(sum>=n)
        {
            v=mid;
            high=mid-1;
        }
        else if(sum<n)
            low=mid+1;
        if(low == high)
            break;
    }
    printf("%d\n",v);

    return 0;
}
