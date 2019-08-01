#include <stdio.h>

int main()
{
    long int n;
    int i,k,rem,div;  //remai:余数 divide:除
    scanf("%d",&k); 
    if(k>36)
        printf("-1\n");
    else
    {
        div=k/2;   //表示8出现的次数
        rem=k%2;   //如果rem=1，则出现一次循环为1的数字，如果=0就全部为8
    
        if(rem==1)
        {
            printf("4");
            for(i=0;i<div;i++)
                printf("8");
            printf("\n");
        }
        else if(rem == 0)
        {
            for(i=0;i<div;i++)
                printf("8");
            printf("\n");
        }
    }
    return 0;
}
