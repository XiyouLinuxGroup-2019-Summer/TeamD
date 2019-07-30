#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>

int main(void)
{
    int count = 1;
    int child;
    printf("Before create son, the father's count is:%d\n", count);

    if((child = vfork())< 0)
    {
        perror("fork error : ");
    }
    else if(child == 0)     
    {
        printf("This is son, his count is: %d (%p). and his pid is: %d\n", ++count, &count, getpid());
        exit(0);
    }
    else     
    {
        printf("After son, This is father, his count is: %d (%p), his pid is: %d\n", ++count, &count, getpid());
        exit(0);
    }

    return EXIT_SUCCESS;
}
