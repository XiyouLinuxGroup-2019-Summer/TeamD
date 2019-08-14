#include<stdio.h>
#include<algorithm>
//using namespace std;
int main(){
    int n;
    int i;
    scanf("%d",&n);
    
    int a[200010];
    for(i = 1;i < n+1;i++){
        scanf("%d",&a[i]);
    }
    sort(a+1,a+n+1);
       int count = 0;
    for(i = 1,j = 1;i < n+1;i++){    
        if(a[i] >= j){
            //printf("a[%d] = %d >= %d\n",i,a[i],j);
            count++;
            j++;
        }
    }
    printf("%d",count);
    return 0;
}

