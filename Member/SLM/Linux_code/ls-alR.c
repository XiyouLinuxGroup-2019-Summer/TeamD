#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <time.h>
#include <dirent.h>
#include <unistd.h>
#include <limits.h>
#include <errno.h>
#include <grp.h>
#include <pwd.h>

#define MAXROWLEN 80     //一行显示的最多字符数

void error(char *p,int line)    //错误处理函数！！
{
    printf("error!:%d",line);
    perror(p);
    exit(1);
}

void takeout(char *path,char *name)       //从路径中解析出文件名
{
    int i,j;
    for(i=0;i<strlen(path);i++)
    {
        if(path[i]=='/')
        {
            j=0;
            continue;
        }
        name[j++]=path[i];
    }
    name[j]='\0';
}

void display_l(struct stat buf,char *name)   //获取文件属性 打印
{
    //char buf_time[32];
    int n;
    struct passwd *pw;
    struct group *gr;
    struct tm *t;

    switch(buf.st_mode & S_IFMT)  //获取并转换之后打印文件类型
    {
    case S_IFREG:
        printf("-");
        break;
    case S_IFDIR:
        printf("d");
        break;
    case S_IFCHR:
        printf("c");
        break;
    case S_IFBLK:
        printf("b");
        break;
    case S_IFIFO:
        printf("p");
        break;
    case S_IFLNK:
        printf("l");
        break;
    case S_IFSOCK:
        printf("s");
        break;
    }

    for(n=8;n>=0;n--)   //文件权限
    {
        if(buf.st_mode & (1<<n))
        {
            switch(n%3)
            {
            case 2:
                printf("r");
                break;
            case 1:
                printf("w");
                break;
            case 0:
                printf("x");
                break;
            default:
                break;
            }
        }
        else
            printf("-");
    }

    printf("%3d",buf.st_nlink);  //硬链接数

    pw=getpwuid(buf.st_uid);   //所属用户名
    gr=getgrgid(buf.st_gid);   //所属用户组名
    printf("%-6s %-6s",pw->pw_name,gr->gr_name);

    printf(" %5ld",buf.st_size);  //字节数

    t=localtime(&buf.st_mtime);  //最后修改时间
    printf(" %d-%d-%d %02d:%02d:%02d",t->tm_year+1900,t->tm_mon+1,t->tm_mday,t->tm_hour,t->tm_min,t->tm_sec);

    if(S_ISLNK(buf.st_mode))  //判断是否为链接文件
    {
        printf("->");
        readlink(name,link,100);
        printf("%s",link);
    }

}

//  根据 *命令行参数* & *完整路径名*  显示<目标文件>
//  参数flag: 命令行参数
//  参数pathname: 包含了文件名的路径名

void display(int flag,char *pathname) 
{
    void display_R(int flag,char *path);
    int i,j;
    struct stat buf;
    char name[256];

    if(lstat(pathname,&buf) == -1)
    {
        error("display",__LINE__);
    }

    takeout(pathname,name);


    switch(flag)
    {
    case 1:                     //没有 a l R 参数
        if(name[0] != '.')
            printf("%-6s",name);
        break;
    case 3:                            // -a
        printf("%-6s",name);
        break;
    case 5:                            // -l
        if(name[0]!='.')
        {
            display_l(buf,name);
            printf("%s\n",name);
        }
        break;
    case 7:                             // -a -l
        display_l(buf,name);
        printf(" %-s\n",name);
    case 9:                             // -R
        display_R(flag,pathname);
        break;
    case 11:                            // -a -R
        printf(". ..\n");
        display_R(flag,pathname);
        break;
    case 13:                            //-l -R
        display_l(buf,name);
        printf(" ");
        display_R(flag,pathname);
    case 15:                            //-a -l -R
        display_l(buf,".");
        printf(".\n");
        display_l(buf,"..");
        printf("..\n");
        display_R(flag,pathname);
        break;

    }
}


void display_R(int flag,char *path)    //-R参数
{
    struct stat buf;
    struct stat buff;
    DIR *dir;
    struct dirent * ptr;
    char allname[256][260],name[256][260],a[260];
    int i,j,len,count;

    if(lstat(path,&buf)==-1)
    {
        if(errno==13)       //permission denied :请求的<文件/文件夹>没有权限导致<服务器拒绝请求>
        {
            return ;
        }
        else
        {
            printf("error : %s\n",path); //error("display_R",__LINE__);
            return ;
        }
    }

    if(S_ISDIR(buf.st_mode))    //该文件为目录文件，含有<文件>/<子目录>
    {
        printf("\n%s\n",path);  //打印目录名
        count=0;
        dir = opendir(path);
        if(dir == NULL)
        {
            error("display_R",__LINE__);
        }


        i=0;
        //获取子目录下的文件目录名，并连接成绝对路径
        while((ptr=readdir(dir))!=NULL)  
        {
            len=0;
            count++;
            strncpy(allname[i],path,strlen(path));
            allname[i][strlen(path)]='/';
            allname[i][strlen(path)+1]='\0';
            strncat(allname[i],ptr->d_name,strlen(ptr->d_name));
            allname[i][strlen(allname[i])]='\0';
            
            i++;
        }

        for(i=0;i<count;i++)
            takeout(allname[i],name[i]);

        for(i=0;i<count;i++)
        {
            if(name[i][0]!='.')
            {
                if(lstat(allname[i],&buff) == -1)
                    printf("error242");

                if(S_ISDIR(buff.st_mode))   //如果这个文件还是目录文件
                {
                    char *m=(char *)malloc(strlen(allname[i])*sizeof(char));
                    display_R(flag,m);                                        //  递归
                    free(m);
                }

                else
                {
                    if(flag>11)
                    {
                        display_l(buff,allname[i]);
                    }
                    printf(" %s\n",name[i]);
                }
            }
            else
            {
                printf("\n");
                continue;
            }
        }

    }
    else
    {
        takeout(path,a);

        if(a[0] != '.')
        {
            if(flag > 11)
            {
                display_l(buff,allname[i]);
            }
            printf(" %-s\n",a);
        }
    }

}


void display_dir(int flag_param,char *path)
{
    void display(int flag,char *pathname);
    DIR *dir;
    struct dirent *ptr;
    int count=0;
    char filename[256][260],fullname[256][260],name[256][260];
    char temp[PATH_MAX];       //PATH_MAX包含在(stdlib.h) 在vc中它的值为260

    //获取该目录下文件总数
    dir=opendir(path);
    if(dir == -1)
    {
        error("opendir",__LINE__);
    }

    int i=0,j,k,len;
    while((ptr= readdir(dir))!=NULL)
    {
        len=0;
        count++;

        memset(filename[i],0,strlen(filename[i]));
        memcpy(filename[i],ptr->d_name,sizeof(ptr->d_name));
        //strcpy(filename[i],ptr->d_name);
        len = strlen(ptr->d_name);
        filename[i][len]='\0';
        i++;

    }
    closedir(dir);

    if(count>256)
    {
        error("opendir",__LINE__);
    }

    //按字典序排序
    for(j=0;j<count-1;j++)
    {
        for(i=0;i<count-1-j;i++)
        {
            if(strcmp(filename[i],filename[i+1])>0)
            {
                strcpy(temp,filename[i]);
                strcpy(filename[i],filename[i+1]);
                strcpy(filename[i+1],temp);
            }
        }
    }
    for(i=0;i<count;i++)
    {
        strncat(fullname[i],path,strlen(path));
        fullname[i][strlen(path)]='/';
        fullname[i][strlen(path)+1]='\0';
        strncat(fullname[i],filename[i],strlen(filename[i]));
        fullname[i][strlen(fullname[i])]='\0';
    }

    for(i=0;i<count;i++)
    {
        takeout(fullname[i],name[i]);
    }

    for(i=0;i<count;i++)
    {
        if(flag_param ==9 || flag_param == 11 || flag_param == 13 || flag_param == 15)
        {
            int flag=1;
            if(name[i][0] == '.')
                flag=0;
            if(flag==1)
                display(flag_param,fullname[i]);
        }
        else
            display(flag_param,fullname[i]);

        printf("\n");

    }

}


int main(int argc,char *argv[])
{
    struct stat buf;
    int i,j,k;
    char path[260];
    char param[32];          //保存命令行参数，目标文件名和目录名不在 该数组
    int flag_param=1;       //参数种类，判断是否含有 -l -a -R  选项


    //解析命令行参数 (让程序知道输入了哪些参数)
    j=0;
    for(i=0;i<argc;i++)
    {
        if(argv[i][0]=='-')
        {
            //注意这里！！！！×××××××××××
            //k<=strlen(argv[i])?????????
            //
            for(k=1;k<strlen(argv[i]);k++,j++)
                param[j]=argv[i][k];   //获取 - 后面的参数 保存到param数组
        }
    }

    //只支持参数a l R 如果有其他选项就报错！！
    for(i=0;i<j;i++)
    {
        if(param[i]=='a')
            flag_param+=2;
        else if(param[i]=='l')
            flag_param+=4;
        else if(param[i]=='R')
            flag_param+=8;
        
        if(param[i]!='a' && param[i]!='l' && param[i]!='R')
        {
            printf("-myls don't have this functional-");
            exit(1);
        }
    }
    param[i]='\0';


    if(argc==1)   //如果没有输入参数 直接执行./a.out 就等于路径为当前目录
    {
        strcpy(path,'.');
        display_dir(flag_param,path);
        return 0;
    }
    //   有点问题
    else if(argc==2)
    {
        if(flag_param==1)
        {
            strcpy(path,argv[1]);
        }
        else
        {
            strcpy(path,'.');
        }
    }
    //  ???有点问题
    else if(argc==3)
    {
        strcpy(path,argv[2]);
    }



    //如果目标文件或目录不存在，报错并退出程序
    if(stat(path,&buf)==-1)
    {
        error("it doesn't exist",__LINE__);
    }

    if(S_ISDIR(buf.st_mode))  //是一个目录
        display_dir(flag_param,path);
    else 
        display(flag_param,path);

    return 0;

}
