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
#define normal           0  //平平无奇
#define out_redirect     1  //>
#define in_redirect      2  //<
#define have_pipe        3  //|
#define add_out_redirect 4  //>>

void my_print();      				//提示符
void get_input(char *);   			//输入参数
void inputs(char *,int *,char a[100][256]); 	//分析参数
void doit(int,char a[100][256]);   		//执行
int findit(char *);    				//查找

int main(int argc,char *argv[])
{
    int i;
    int ac=0;
    char al[100][256];
    char **arg= NULL;
    char *buf=NULL;
    signal(SIGINT,SIG_IGN);
    buf=(char *)malloc(256*sizeof(char *));
    if(buf == NULL)
    {
        perror("malloc error");
	exit(-1);     
    }


    while(1)
    {
        memset(buf,0,256); 
        my_print();  
        get_input(buf); 
        if((strcmp(buf,"exit\n") == 0) || strcmp(buf,"logout\n") == 0)

            break;
        for(i=0;i<100;i++)

            al[i][0]='\0'; 
        ac=0;
        inputs(buf,&ac,al);  


        doit(ac,al);
    }

    if(buf!=NULL)

    {
        free(buf);
	buf=NULL;
    }
    exit(0);
}

void my_print()		//我的提示符
{
    printf("cosineta@cosineta-PC:~$");
}


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





void inputs(char *buf,int *ac,char al[100][256])
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

            strncpy(al[*ac],p,number+1);
            al[*ac][number]='\0';
            *ac+=1;  
            p=q;
        }
    }
}

void doit(int ac,char al[100][256])
{

    int flag =0;
    int how =0;   
    int background =0;   
    int status;
    int i;
    int fd;
    char *arg[ac+1];  
    char *argnext[ac+1];
    char *file;
    pid_t pid;  

  
    for(i=0;i<ac;i++)
    {
        arg[i] =(char *)al[i];
    }

    arg[ac]=NULL;

      if((arg[0]!=NULL)&&(strcmp(arg[0],"cd") == 0))//cd功能
    {
        if(arg[1] ==NULL)
            return;

        if(strcmp(arg[1],"~")==0)
        {
            strcpy(arg[1],"/home/cosineta/");
        }

        if(chdir(arg[1]) == -1)
            perror("cd error!\n");
        return ;
    }




    for(i=0;i<ac;i++)	//检查有无&
    {
        if(strncmp(arg[i],"&",1)==0)
        {
            if(i== ac-1)
            {
                background = 1;
                arg[ac-1] = NULL;
                break;
            }
            else
            {
                printf("wrong command.\n");
                return;
            }
        }
    }


  
    for(i=0;arg[i]!=NULL;i++)	//检查有无>
    {
        if(strcmp(arg[i],">")==0)
        {
            flag++;
            how = out_redirect; 
	    if(arg[i+1]==  NULL)
            {
                flag ++;
            }
        }

        if(strcmp(arg[i],">>") == 0)//检查有无>>
        {
            flag++;
            how = add_out_redirect; 
	    if(arg[i+1] == NULL)
                flag++;
        }



        if(strcmp(arg[i],"<")==0)//检查有无<
        {
            flag++;
            how = in_redirect;   
	    if(i == 0)
            {
                flag ++;
            }
        }



        if(strcmp(arg[i],"|")==0)	//检查有无|
        {
            flag ++;
            how = have_pipe;  
	    if((i == 0) || (arg[i+1] == NULL))
            {
                flag ++;
            }
        }
    }

   
    if(flag>1)
    {
        printf("sorry,this command isn't supported\n");
        return;
    }




    if(how == out_redirect)
    {
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
        for(i=0;arg[i]!=NULL;i++)
        {
            if(strcmp(arg[i],"|")==0)
            {
                arg[i] == NULL;
                int j;
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
    case 0://平平无奇
        if(pid ==0)
        {
            if(  !(findit(arg[0]))  )
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

    case 1://>
        if(pid==0)
        {
            if( !(findit(arg[0])) )
            {
                printf("%s :command not found\n",arg[0]);
                exit(0);
            }
            fd = open(file,O_RDWR|O_CREAT|O_TRUNC,0644);


            dup2(fd,1);
            execvp(arg[0],arg);
            exit(0);
        }
        break;


    case 2://<
        if(pid == 0)
        {
            if( !(findit(arg[0])) )
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



    case 3://|

        if(pid == 0)
        {
            int pid2;
            int status2;
            int fd2;

           
            if( (pid2 = fork())<0 )
            {
                printf("fork error\n");
                return ;
            }
            else if(pid2 ==0)
            {
                if( !(findit(arg[0])) )
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

            if( !(findit(argnext[0])) )
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


    case 4://>>
        if(pid == 0)
        {
            if(findit(arg[0]) == 0)
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


    if(background == 1)
    {
        printf("[process id %d]\n",pid);
        return;
    }


    if(waitpid(pid,&status,0)== -1)
        printf("wait for child process error\n");
}



int findit(char *command)
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
