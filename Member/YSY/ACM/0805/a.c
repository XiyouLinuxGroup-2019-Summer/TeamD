#include<stdio.h>
#include<stdlib.h>
int cmp( const void *a , const void *b ) 
{ 
  return *(int *)b - *(int *)a; 
} 


int main()
{
	int a[5],b[4];
	int i=0,j;

	//memset(b,0,sizeof(int)*4);
	while(i<4)
	{
		scanf("%d",&a[i]);
		i++;	
	}
	
	qsort(a,4,sizeof(int),cmp);

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
