#include <stdio.h>
#include <string.h>

int main()
{
    char *Haab[19]={"pop","no","zip","zotz","tzec","xul","yoxkin","mol","chen","yax","zac","ceh","mac","kankin","muan","pax","koyab","cumhu","uayet"};
    char *Tzol[20]={"imix","ik","akbal","kan","chicchan","cimi","manik","lamat","muluk","ok","chuen","eb","ben","ix","mem","cib","caban","eznab","canac","ahau"};
    int n,day,year,i,y,m,tday,tdd;
    char month[10],d[10];

    scanf("%d",&n);
    printf("%d\n",n);
    while(n--)
    {
        scanf("%d. %s %d",&day,month,&year);
        for(i=0;i<19;i++)
        {
            if(strcmp(Haab[i],month) == 0)
            {
                    tday=day+1+i*20;
                    break;
            }
        }
        tday+=year*365;

        y=tday/260;
        m=tday%13;
        if(m == 0)
            m=13;
        tdd=tday%20;
        if(tdd == 0)
            tdd=20;

        strcpy(d,Tzol[tdd-1]);

        if(tday % 260 == 0) 
        {
            printf("%d %s %d\n",m,d,y-1);
            continue;
        }
        printf("%d %s %d\n",m,d,y);
    }
    return 0;
}
