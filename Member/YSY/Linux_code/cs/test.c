#include<stdio.h>
#include<string.h>
#include<unistd.h>
int main()
{
   const char*msg="hello\n";
   int len = strlen(msg);
   write(1,msg,len);//write所对应的文件描述符为1
   char buf[1024]={0};
   read(0,buf,len);//read所对应的文件描述符为0
   return 0;
}

