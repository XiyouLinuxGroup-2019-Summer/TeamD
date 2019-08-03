#include<stdio.h>
#include <string.h>
#include <stdlib.h>

int main()
{
    int n,d,i,j=0;
    scanf("%d %d",&n,&d);
    char a[n];
    int b[n];
    int t=0,max=0;
	
    for(j=0;j<d;j++)
    {
	scanf("%s",a);
        for(i=0;i<n;i++)
        {
            if(a[i] == '0')
	    {
		    b[j]=1;
		    break;
	    }
        }
    }
    

    for(i=0;i<d;i++)
    {
	 //   printf("%d,",b[i]);
        if(b[i] == 0)
            t=0;
        if(b[i] == 1)
        {
            t++;
            if(t>max)
                max=t;
        }
    }
    
    printf("%d\n",max);	


	return 0;
}
