#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <dirent.h>
#include <errno.h>
#define normal           0  //一般命令，没有其他特殊选项
#define out_redirect     1  //输出重定向
#define in_redirect      2  //输入重定向
#define have_pipe        3  //命令中有管道
#define add_out_redirect 4  //输出追加重定向


void print_first();      //打印提示符
void get_input(char *);    //获取用户输入
void explain_input(char *,int *,char a[100][256]);   //对输入的命令进行解析
void do_cmd(int,char a[100][256]);    //执行命令
int find_cmd(char *);      //查找命令中的可执行程序

//主函数
int main(int argc,char *argv[])
{
    int i;
    int argcount=0;
    char arglist[100][256];
    char **arg= NULL;
    char *buf=NULL;



    signal(SIGINT,SIG_IGN);
    buf=(char *)malloc(256*sizeof(char *));
    if(buf == NULL)
    {
        perror("malloc error");
        exit(-1);      // void exit(int status); 如果 status == -1 表示某指针为空
    }

    while(1)
    {

        memset(buf,0,256);  //清空所分配的内存

        print_first();  //打印提示符

        get_input(buf);  //获取用户输入


        if((strcmp(buf,"exit\n") == 0) || strcmp(buf,"logout\n") == 0)
            break;

        for(i=0;i<100;i++)
            arglist[i][0]='\0'; 

        argcount=0;

        explain_input(buf,&argcount,arglist);  //解析用户输入的命令

        do_cmd(argcount,arglist);
    }

    if(buf!=NULL)
    {
        free(buf);
        buf=NULL;
    }
    exit(0);
}

//打印提示符
void print_first()
{
    printf("\033[37;1mmatery@matery-SHELL:~$ \033[0m");
}


//获取用户输入
void get_input(char *buf)
{
    int len=0;
    int ch;

    ch=getchar();
    while(len<256 && ch!='\n')
    {
        buf[len++]=ch;
        ch=getchar();
    }

    if(len==256)
    {
        printf("command is too long. \n");
        exit(-1);
    }

    buf[len]='\n';
    len++;
    buf[len]='\0';
}


//解析buf中的命令，将结果存入arglist中，命令以\n结束
void explain_input(char *buf,int *argcount,char arglist[100][256])
{
    char *p=buf;
    char *q=buf;
    int number=0;

    while(1)
    {
        if(p[0] == '\n')
            break;

        if(p[0] == ' ')
            p++;
        else
        {
            q=p;
            number=0;
            while((q[0]!='\n') && (q[0]!= ' '))
            {
                q++;
                number++;
            }

            strncpy(arglist[*argcount],p,number+1);
            arglist[*argcount][number]='\0';
            *argcount+=1;   //去保存下一个参数
            p=q;
        }
    }
}

//执行输入的命令
void do_cmd(int argcount,char arglist[100][256])
{
    int flag =0;
    int how =0;   //查看命令中时候含有>、<、|   
    int background =0;   //标示命令中是否有后台运行标识符
    int status;
    int i;
    int fd;
    char *arg[argcount+1];  //把命令取出来放到这里
    char *argnext[argcount+1];
    char *file;
    pid_t pid;  //进程ID

    //将命令取出来
    for(i=0;i<argcount;i++)
    {
        arg[i] =(char *)arglist[i];
    }
    arg[argcount]=NULL;


    //判断有没有输入cd命令(内部命令)
    if((arg[0]!=NULL)&&(strcmp(arg[0],"cd") == 0))
    {
        if(arg[1] ==NULL)
            return;

        if(strcmp(arg[1],"~")==0)
        {
            strcpy(arg[1],"/home/matery/");
        }
        //经当前工作目录更换
        if(chdir(arg[1]) == -1)
            perror("cd error!\n");
        return ;
    }


    //查看命令行是否有后台运行符 
    //后台运行符 $必须在命令最后出现
    for(i=0;i<argcount;i++)
    {
        if(strncmp(arg[i],"&",1)==0)
        {
            if(i== argcount-1)
            {
                background = 1;
                arg[argcount-1] = NULL;
                break;
            }
            else
            {
                printf("wrong command.\n");
                return;
            }
        }
    }

    //检查是否有 输出重定向>、输出追加重定向、输入重定向<、管道符|
    for(i=0;arg[i]!=NULL;i++)
    {

        if(strcmp(arg[i],">")==0)
        {
            flag++;
            how = out_redirect;  // how=1
            if(arg[i+1]==  NULL)
            {
                flag ++;
            }
        }

        if(strcmp(arg[i],">>") == 0)
        {
            flag++;
            how = add_out_redirect; //how=4
            if(arg[i+1] == NULL)
                flag++;
        }

        if(strcmp(arg[i],"<")==0)
        {
            flag++;
            how = in_redirect;   // how=2
            if(i == 0)
            {
                flag ++;
            }
        }

        if(strcmp(arg[i],"|")==0)
        {
            flag ++;
            how = have_pipe;   //  how=3
            if((i == 0) || (arg[i+1] == NULL))
            {
                flag ++;
            }
        }
    }


    //如果flag>1，说明命令行中有多个><|符号，本程序不支持这样的命令，或者命令格式不对
    if(flag>1)
    {
        printf("sorry,this command isn't supported\n");
        return;
    }


    if(how == out_redirect)
    {
        //命令行只有一个输出重定向符号
        for(i=0;arg[i]!=NULL;i++)
        {
            if(strcmp(arg[i],">")==0)
            {
                file=arg[i+1];
                arg[i]=NULL;
            }
        }

    }

    if(how == add_out_redirect)
    {
        //命令含有输出追加重定向符号
        for(i=0;arg[i]!=NULL;i++)
        {
            if(strcmp(arg[i],">>")==0)
            {
                file = arg[i+1];
                arg[i]=NULL;
            }
        }
    }

    if(how == in_redirect)
    {
        //命令行只有一个输入重定向符号
        for(i=0;arg[i]!=NULL;i++)
        {
            if(strcmp(arg[i],"<")==0)
            {
                file=arg[i+1];
                arg[i]=NULL;
            }
        }
    }

    if(how == have_pipe)
    {
        //命令行只有一个管道符符号，把管道符号后面的部分存入argnext中
        //管道符号后面是一个可执行的shell命令
        
        for(i=0;arg[i]!=NULL;i++)
        {
            if(strcmp(arg[i],"|")==0)
            {
                arg[i] == NULL;
                int j;
                //把管道后面的部分放进argnext
                for(j=i+1;arg[j]!=NULL;j++)
                {
                    argnext[j-1-i]=arg[j];
                }
                argnext[j-1-i]=arg[j];
                break;
            }
        }
    
    }

    if((pid = fork()) < 0)
    {
        printf("fork error\n");
        return ;
    }


    switch(how)
    {
    case 0:
        //输入的命令中没有>,<,| 
        //pid=0说明是在子进程下执行输入的命令
        if(pid ==0)
        {

            if(  !(find_cmd(arg[0]))  )
            {
                printf("%s :command not found\n",arg[0]);
                exit(0);
            }
            if(execvp(arg[0],arg) < 0 )
            {
                printf("%s",stderr);
            }
            printf(" execvp\n");
            exit(0);
        }
        break;

    case 1:
        //有输出重定向符号 >
        if(pid==0)
        {

            if( !(find_cmd(arg[0])) )
            {
                printf("%s :command not found\n",arg[0]);
                exit(0);
            }
            fd = open(file,O_RDWR|O_CREAT|O_TRUNC,0644);

            //复制文件描述符
            dup2(fd,1);
            execvp(arg[0],arg);
            exit(0);
        }
        break;

    case 2:
        //命令行有输入定向符
        if(pid == 0)
        {
            if( !(find_cmd(arg[0])) )
            {
                printf("%s :command not fonud\n",arg[0]);
                exit(0);
            }
            fd = open(file,O_RDONLY);
            dup2(fd,0);
            execvp(arg[0],arg);
            exit(0);
        }
        break;

    case 3:
        //只有管道命令 （管道的本质是-->固定大小的内核缓冲区）
        //管道也是一个特殊文件，所以操作文件描述符就是操作管道文件
        
        if(pid == 0)
        {
            int pid2;
            int status2;
            int fd2;

            //子进程再创建一个子进程
            if( (pid2 = fork())<0 )
            {
                printf("fork error\n");
                return ;
            }
            else if(pid2 ==0)
            {
                if( !(find_cmd(arg[0])) )
                {
                    printf("%s :command not found\n",arg[0]);
                    exit(0);
                }
                fd2=open("/tmp/myshell",O_WRONLY | O_CREAT | O_TRUNC,0644);
                dup2(fd2,1);
                execvp(arg[0],arg);
                exit(0);
            }

            if(waitpid(pid2,&status2,0)==-1)
            {
                printf("wait for child process\n");
            }

            if( !(find_cmd(argnext[0])) )
            {
                printf("%s :command not found\n",argnext[0]);
                exit(0);
            }
            fd2 = open("/tmp/myshell",O_RDONLY);
            dup2(fd2,0);
            execvp(argnext[0],argnext);

            if(remove("/tmp/myshell") == -1)
            {
                printf("remove error\n");
            }
            exit(0);

        }
        break;

    case 4:
        if(pid == 0)
        {
            if(find_cmd(arg[0]) == 0)
            {
                printf("%s : command can't found\n",arg[0]);
                exit(0);
            }
            close(1);
            fd=open(file,O_WRONLY | O_CREAT | O_APPEND,0644);
            execvp(arg[0],arg);
            close(fd);
            exit(0);
        }
        break;

    default:
        break;
    }

    //如果命令行末有后台执行符号 ，父进程直接返回，不等待子进程结束
    if(background == 1)
    {
        printf("[process id %d]\n",pid);
        return;
    }

    //没有后台运行符号，父进程等待子进程结束
    if(waitpid(pid,&status,0)== -1)
        printf("wait for child process error\n");

}

//查找命令中的可执行程序
int find_cmd(char *command)
{
    DIR * dp;
    struct dirent * dir;
    char * path[]={"./","/bin","/usr/bin",NULL};
    if(strncmp(command,"./",2)==0)
        command=command+2;

    int i=0;
    while(path[i]!=NULL)
    {
        if((dp=opendir(path[i]) )==NULL)
            printf("can't open /bin \n");
        while((dir=readdir(dp))!=NULL)
        {
            if(strcmp(dir->d_name,command)==0)
            {
                closedir(dp);
                return 1;
            }
        }
        closedir(dp);
        i++;
    }
    return 0;
}
