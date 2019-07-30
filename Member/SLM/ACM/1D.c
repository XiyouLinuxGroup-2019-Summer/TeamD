#include <stdio.h>
#include <string.h>

int main()
{
    int i,j,l;
    char a[30],b[220];

    while(gets(a))
    {
        if(strcmp(a,"ENDOFINPUT")==0)
            break;
        if(strcmp(a,"START")==0)
        {
            gets(b);
            l=strlen(b);

            for(i=0;i<l;i++)
            {
                if(b[i]>='A' && b[i]<='E')
                {
                    b[i]=86+(b[i]-'A');
                 //   printf("%s",86+b[i]);
                }
                else if(b[i]>'E' && b[i]<='Z')
                {
                    b[i]=b[i]-5;
                  //  printf("%s",b[i]-5);
                }
            }
            printf("%s\n",b);
            //printf("\n");
        }
    }
    return 0;
}
