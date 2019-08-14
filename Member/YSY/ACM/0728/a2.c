#include <stdio.h>

int main()
{
    int m,n;
    double a,b;
    double money,min=999999999;
    scanf("%d %d",&n,&m);
    while(n--)
    {
        scanf("%lf %lf",&a,&b);
        money=(a/b)*m;
        if(money<min)
            min=money;
    }
    printf("%.8lf",min);
    return 0;
}
