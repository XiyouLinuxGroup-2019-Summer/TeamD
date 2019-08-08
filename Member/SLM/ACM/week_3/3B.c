#include <stdio.h>
#include <math.h>
int a[20], n=8,count;
int num;
int check(int k)
{
    int i;
    for(i=1;i<k;i++)
    {
        if(abs(a[i]-a[k]) == abs(i-k) || (a[i] == a[k]))
            return 0;
    } 
        return 1;
}

int output(int n)
{
    count++;
    int i;
    if(count == num)
    {
        for(i=1;i<=n;i++)
        {
            printf("%d",a[i]);
        }
        printf("\n");
    }
}

int backdate(int n)
{
    int k;
    a[1] = 0;
    k = 1;
    while(k>0)
    {
        a[k]++;
        while((a[k] <= n)&&(check(k) == 0))
            a[k]++;
        if(a[k] <= n)
        {
            if(k == n)
            {
                output(n);
            }
            else
            {
                k++;
                a[k] = 0;
            }
        }
        else
        {
            k--;
        }
    }
    return 0;
}

int main(void)
{
    int t;
    scanf("%d",&t);
    while(t--)
    {
        count = 0;
        scanf("%d",&num);
        //scanf("%d",&n);
        backdate(n);
    }
}
