#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<time.h>//-l需要显示时间
#include<sys/stat.h>//stat
#include<sys/types.h>//stat【c++?】
#include<linux/limits.h>
#include<dirent.h>
#include<grp.h>//struct group
#include<pwd.h>//struct passwd
#include<errno.h>//保存错误码
#include<unistd.h>

#define LMAX 100//行最多字符数

int PARAM; //分析ls的参数
int num;//argv[num]是最后一个laR

int l_max;//最长文件名的长度
int l_leave=LMAX;//剩余长度

//神奇海螺标示符
//@@@@@@@@@@@@微微困惑，但没必要？
//++++++++++++可能会报错
//------------令人玩味，值得注意

void lsparam(int argc,char ** argv);//ls的参数分析
void ls_l(struct stat buf,char* name);//l的打印
void ls_dir(char* path);//目录处理
void my_err(const char* errnotice,int line);//报错用
int ls_show(char* pathname);
void ls_a(char* name);//a的打印
void ls_R(char* pathname);

void ls_R(char *pathname)
{
	int a=ls_show(pathname);
	if(!a)
		printf("\n%s:\n",pathname);
	  
 /* struct stat     buf;
    DIR             *dir;
    struct dirent   *ptr;
    printf("\n%s:\n",name);
    dir = opendir(name);
    while( ( ptr = readdir( dir )) != NULL )
    {
        if(ptr->d_name[0] != '.')
        {
            ls_a( ptr->d_name );
        }
    }
    closedir(dir);
    dir = opendir( name );
    while(( ptr = readdir( dir )) != NULL)
    {
        if(ptr->d_name[0] != '.')
        {
            stat( ptr->d_name, &buf );
            if( S_ISDIR( buf.st_mode ) )
            {
                char *temp = name;
                strcat( temp, "/" );
                strcat( temp, ptr->d_name );
                ls_R(temp);
            }
        }
    }
    closedir( dir );
    return;
*/
    }
//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
void ls_a(char* name)
{
	int i,len;

	if(l_leave<l_max)
	{
		printf("\n");
		l_leave=LMAX;
	}
	
	len=strlen(name);
	//@@@@@@@@@@@@@@@@@@@@@@@
	len=l_max-len;
	printf("%-s",name);

	for(i=0;i<len;i++)
		printf(" ");
	printf("  ");//@@@@@@@@@@@@@@@@@@@

	l_leave-=(l_max+2);

}

int ls_show(char* pathname)
{
	int i,j;
	struct stat buf;
	char name[NAME_MAX+1];
	//printf("我在show:\n%s\n",pathname);

	for(i=0,j=0;i<strlen(pathname);i++)
	{
		if(pathname[i]=='/')
		{
			j=0;
			continue;
		}
		name[j++]=pathname[i];
	}
	
	name[j]='\0';

	if(lstat(pathname,&buf)==-1)
		my_err("stat",__LINE__);

	//判断参数
	//if(PARAM&2)，说明有l
	if((PARAM&4)&&name[0]=='.')
			return 1;
	if(!(PARAM&1))
		if(name[0]=='.')
			return 0;

	if(PARAM&2)
		ls_l(buf,name);
	else
		ls_a(name);	


	return 0;
	

}
//@@@@@@@@@@@@@@@@@@@@@@@@@
void my_err(const char* errnotice,int line)
{
	fprintf(stderr,"line:%d",line);
	perror(errnotice);
	exit(1);
}



void ls_dir(char* path)
{
	DIR *dir;
	struct dirent * ptr;
	int fnum=0;//文件数
	int i,j,k,len=strlen(path);
	char fname[256][PATH_MAX+1],min[PATH_MAX+1];//min排序临时

	dir=opendir(path);
	
	if(dir==NULL)//空目录报错
		my_err("opendir",__LINE__);
	
	while((ptr=readdir(dir))!=NULL)
	{
		//printf("ptr不空\n");
		if(l_max<strlen(ptr->d_name))
			l_max=strlen(ptr->d_name);
		fnum++;
	}
	closedir(dir);
	
	if(fnum>256)//@文件太多，报错

		my_err("too many files under this dir",__LINE__);
	
	dir=opendir(path);
	for(i=0;i<fnum;i++)	
	{
		ptr=readdir(dir);
		if(ptr==NULL)//读文件，报错
			my_err("readdir",__LINE__);

		//-----------------------
		strncpy(fname[i],path,len);
		fname[i][len]='\0';//++++++++++让最后一位是\0
		strcat(fname[i],ptr->d_name);
		fname[i][len+strlen(ptr->d_name)]='\0';
	}
	
	for(i=0;i<fnum;i++)//排序
	{
		k=i;
		for(j=i+1;j<fnum;j++)
		{
			if(strcmp(fname[k],fname[j])>0)
				k=j;
		}
		if(k!=i)
		{	strcpy(min,fname[k]);
			strcpy(fname[k],fname[i]);
			strcpy(fname[i],min);
		}
	}
	

	for(i=0;i<fnum;i++)
	{
		if(PARAM&4)
			ls_R(fname[i]);
		//return;??++++++++++++++++++++++++++++++++++
		else 
			ls_show(fname[i]);
	}
	
	closedir(dir);
	printf("\n");

}



void ls_l(struct stat buf,char* name)
{
	char buf_ctime[32];
	struct passwd *psd;
	struct group * grp;
	int l;//时间字符串长度
	psd=getpwuid(buf.st_uid);
	grp=getgrgid(buf.st_gid);
	
	//直接开始打印就好
	//第一位
	if(S_ISLNK(buf.st_mode))
		printf("l");
	else if(S_ISREG(buf.st_mode))
		printf("-");
	else if(S_ISDIR(buf.st_mode))
		printf("d");
	else if(S_ISCHR(buf.st_mode))
		printf("c");
	else if(S_ISBLK(buf.st_mode))
		printf("b");
	else if(S_ISFIFO(buf.st_mode))
		printf("f");
	else if(S_ISSOCK(buf.st_mode))
		printf("s");

	//USR
	if(buf.st_mode & S_IRUSR)
		printf("r");
	else    printf("-");
	if(buf.st_mode & S_IWUSR)
		printf("w");
	else    printf("-");
	if(buf.st_mode & S_IXUSR)
		printf("x");
	else    printf("-");

	//GRP
	if(buf.st_mode & S_IRGRP)
		printf("r");
	else    printf("-");
	if(buf.st_mode & S_IWGRP)
		printf("w");
	else    printf("-");
	if(buf.st_mode & S_IXGRP)
		printf("x");
	else    printf("-");

	
	//OTH
	if(buf.st_mode & S_IROTH)
		printf("r");
	else    printf("-");
	if(buf.st_mode & S_IWOTH)
		printf("w");
	else    printf("-");
	if(buf.st_mode & S_IXOTH)
		printf("x");
	else    printf("-");
	printf(" ");

	//链接数
	printf("%4d",buf.st_nlink);
	printf(" ");
	
	//用户名和组名
	printf("%-8s",psd->pw_name);
	printf("%-8s",grp->gr_name);
	printf(" ");
	
	//文件大小
	printf("%8d",buf.st_size);
	printf(" ");

	//修改时间
	strcpy(buf_ctime,ctime(&buf.st_mtime));
	l=strlen(buf_ctime);
	buf_ctime[l-1]='\0';//把时间字符串用\0结尾
	printf("%s",buf_ctime);
	//????????????????
	printf("    %-30s",name);
	printf("\n");

}
 



void lsparam(int argc,char ** argv)//分析ls的参数
{
	int i=1,j=0,k=1;
	num=0;//param[j]=argv[i][k]酱
	char param[10];//ls的参数
	char cc;//单独的参数

	
	/*for(i=0;i<argc;i++)
	{
		printf("%d:%s\n",i,argv[i]);
	}
	exit(1);
	*/

	/*
	 全局变量PARAM值，代表ls参数
	 if(PARAM&1)，说明有a
	 if(PARAM&2)，说明有l
	 if(PARAM&4)，说明有R
	*/
	for(i=0;i<argc;i++)
	{
		if(argv[i][0]=='-')
		{
			//printf("\n\nthe%d:%s",i,argv[i]);
			for(k=1;k<strlen(argv[i]);k++,j++)
			{param[j]=argv[i][k];}
			num++;//统计命令行到第几位是ls参数
			      //即：argv[num]是最后一个laR
		}
		
	}

	//printf("%s\n",param);
	for(i=0;i<j;i++)
	{
		cc=param[i];
	//	printf("canshu:%c\n",cc);
		switch(cc)
		{
			case 'a':PARAM+=1;break;
			case 'l':PARAM+=2;break;
			case 'R':PARAM+=4;break;
			//报错还是要有的
			default:printf("Myls: invalid option -%c\n",cc);
		}
	}
}


int main(int argc,char ** argv)
{
	char path[PATH_MAX+1];//一个字符串，用来写路径
	struct stat buf;
	int nn;//录入目录长度

	lsparam(argc,argv);
	//????????????????????????????
	strcpy(path,"./");
	
	if((num+1)==argc)//没有多余参数啦
	{
		path[2]='\0';
		//printf("%s\n23333333\n",path);
		//return 0;
		ls_dir(path);
		return 0;
	}
	stat(path,&buf);

	//+++++++++++++++++++++++++
	
	strcpy(path,argv[num+1]);

	nn=strlen(argv[num+1]);
	
	if(S_ISDIR(buf.st_mode))//是目录
	{
		if(path[nn-1]!='/')
			strcat(path,"/");
		//printf("aa\n");
		strcat(path,"\0");
		//printf("我在main:\n%s\n",path);
		ls_dir(path);
	}
	else//是文件
	
		ls_show(path);

	printf("\n");
	return 0;
}
