#include <stdio.h>

int main(){

    int a[10]={  };
    int i,j, n, t;

    for(i=0;i<4;i++){
        scanf("%d",&a[i]);
    }

    for(i=0;i<4;i++){
        for(j=i+1;j<4;j++){
            if(a[i]<a[j]){
                t=a[i];
                a[i]=a[j];
                a[j]=t;
            }
        }
    }

    if((a[0]<(a[1]+a[2])) || (a[0]<(a[2]+a[3])) || (a[0]<(a[1]+a[3])) || (a[1]<(a[2]+a[3])) ){
        printf("TRIANGLE\n");
    }
    else if((a[0] == (a[1]+a[2])) || (a[0] == (a[2]+a[3])) || (a[0] == (a[1]+a[3])) || (a[1] == (a[2]+a[3])) ){
        printf("SEGMENT\n");
    }
    else
        printf("IMPOSSIBLE\n");

    return 0;
}

