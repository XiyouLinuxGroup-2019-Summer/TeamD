#include<stdio.h>
#include<stdlib.h>
#include<string.h>
int main()
{
    int n;
    scanf("%d",&n);
    int left[10000],right[10000];
    int i,l,m;

    while(n--)
    {
        scanf("%d %d",&l,&m);
        l *= 100;
        if(m == 0)
        {
            printf("0\n");
            continue;
        }

        char bank[10];
        int left_n = 0,right_n = 0,t;

        for(i = 0;i < m;i++){   //输入并分别存储数据
            scanf("%d %s",&t,bank);
            
            if(bank[0] == 'l')
            {
                left[left_t++]=t;
            }
            else
            {
                right[right_t++]=t;
            }
        }

        int times = 0，len_car;
        for(int i = 0,j = 0;i < left_n || j < right_n;)
        {
            len_car = 0;    //能装上的汽车长度
            times++;        //right
            
            if(i == left_n && j == right_n)
            { 
                //已经没有东西了
                times--;
                break;
            }

            while(i < left_n && len_car + left[i] <= l){    //left
                len_car += left[i];
                i++;
            }
            len_car = 0;    //lenth =0
            times++; // 要left 

            if(i == left_n && j == right_n) 
                times--;

            while(j < right_n && len_car + right[j] <= l){
                len_car += right[j];
                j++;
            }
        }
        printf("%d\n",times);
    }
    return 0;
}
