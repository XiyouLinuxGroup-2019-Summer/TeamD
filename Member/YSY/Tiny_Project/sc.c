#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>
#include <strings.h>
#include <pthread.h>
#include <fcntl.h>
#include <sys/types.h>

#define SERV_PORT 	4004		//服务器端口
#define SERV_IP 	"127.0.0.1"	//地址

#define LOGIN 100
#define MAX_CHAR	300

typedef struct datas{
    char     send_name[MAX_CHAR];
    char     recv_name[MAX_CHAR];
    int      send_fd;
    int      recv_fd;
    char     mes1[MAX_CHAR*2];
    char     mes2[MAX_CHAR*2];

}DATA;
 
typedef struct package{
    int   type;
    DATA  data;
}PACK;

//全局变量
int cfd;
char myname[MAX_CHAR];
//函数声明
int login_menu();
void c_login();
void c_logout();
void *recv_PACK();
void func_menu();
void func(int);
void all_friend();
void online_friend();

void fri_list(PACK);

void chat_p();
void chat_p_box();
void my_err(const char*,int);
void all_group();

void chat_g();
void chat_g_box();

void send_file();
void recv_file();

void down_file(PACK);

void add_friend();
void add_group();



/*
int login1(MYSQL mysql1)
{
	char login_id[20];
	char login_psw[20];
	char mingl[100];
	printf("=====登录页面=====\n");
	printf("请输入您的id：");
	scanf("%[^\n]",login_id);
	printf("请输入您的密码：");
	scanf("%*c%[^\n]",login_psw);

	sprintf(mingl," select password from `user` where uid = %s",login_id);
	
	printf("执行\"%s\",是吗?\n",mingl);
	
	if(!strcmp(row[0],login_psw))
		printf("密码正确！！！\n");
	else
		printf("密码错误\n");
	
	mysql_free_result(result);

	printf("login over \n");
	return 0;
	
}
*/
void my_err(const char* errnotice,int line)
{
	fprintf(stderr,"line:%d",line);
	perror(errnotice);
	exit(1);
}

int main(void)
{

    int n, c;
    int oper;
    struct sockaddr_in serv_addr;   //储存服务器IP地址
    char buf[BUFSIZ];
	
	int ifc;

    cfd = socket(AF_INET,SOCK_STREAM,0);

    bzero(&serv_addr,sizeof(serv_addr));		//清零serv_addr
    serv_addr.sin_family = AF_INET;				//
    serv_addr.sin_port = htons(SERV_PORT);
    inet_aton("127.0.0.1",&serv_addr.sin_addr);	//把本地地址传到.sin_addr
    ifc=connect(cfd,(struct sockaddr *)&serv_addr,sizeof(serv_addr));	//连接
    if(ifc){									   //连接报错
	printf("connect failed\n");
	exit(0);
    }

    
    oper=login_menu();
    if(oper!=0){
    	switch (oper){		
    		case 1:
			c_login();
		case 0:
			c_logout();
    	}
    }

 //   sleep(10);
    close(cfd);
    
    return 0;

}

void c_logout()
{
	PACK pack;
	pack.type=100;
	strcpy(pack.data.send_name,myname);
	send(cfd,&pack,sizeof(PACK),0);
	printf("用户%s退出\n",myname);
}

int login_menu()
{
	int oper;
	printf("\n\t\t=============================\n");
	printf("\t\t|                           |\n");
   	printf("\t\t|           1.登录          |\n");
    	printf("\t\t|           0.退出          |\n");
   	printf("\t\t=============================\n");
    	scanf("%d",&oper);    
    	return oper;
}

void c_login()
{
    PACK pack; 
    pack.type = 1;
   
    pthread_t recv_packkk;

    printf("用户名：");
    scanf("%s",pack.data.send_name);
    printf("密码：");
    scanf("%s",pack.data.mes1);

    strcpy(myname,pack.data.send_name);

    if(send(cfd, &pack, sizeof(PACK),0)<0){
        perror("send");
    }
  //  else
  //  printf("fabaochenggong\n");

    while(1) {
        int ret;
        memset(&pack, 0, sizeof(PACK));
        if((ret = recv(cfd, &pack,sizeof(PACK), 0))>0) 
           	    break;
    }
//    printf("shoubao:type=%d\n",pack.type);
    if(pack.type == 0) {
	pthread_create(&recv_packkk, NULL, recv_PACK, NULL);
       	printf(">>>登录成功\n");

	if(pack.data.recv_fd)
		printf("您有%d条未读消息\n",pack.data.recv_fd);
	else
		printf("暂无未读消息\n");

	if(pack.data.send_fd)
		printf("您有待接收文件\n",pack.data.recv_fd);

	if(strcmp(pack.data.mes2,myname)==0)
		printf("您有未读群信息\n",pack.data.recv_fd);

	
	func_menu();
	
    }
    else {
        login_menu(cfd);
        printf("登录失败T^T\n");
    }

}  

void *recv_PACK()
{
//	printf("收包程序ok\n");
    	PACK pack;
    	while (1) {
        	bzero(&pack,sizeof(PACK));
        	int ret;
        	if((ret = recv(cfd, &pack, sizeof(PACK),0)) < 0) {
            		perror("client_recv");
        	}
		
//		printf("收到包%d\n",pack.type);

        	switch (pack.type) {
			case 0:
			//	printf("########################\n服务器已宕机\n########################\n");
				exit(-1);

			case 2:
			case 3:
	               		fri_list(pack);
                		break; 
			case 4:
				printf("已成功添加%s为好友\n",pack.data.recv_name);
				break;
			
			case 6:
				printf("<%s>发来私聊消息\n",pack.data.send_name);
				break;
			case 7:
				printf("<%s>说：%s\n",pack.data.send_name,pack.data.mes1);
				break;
			case 8:
				printf("%s\n",pack.data.mes1);
				break;
			case 10:
				printf("已成功加入群聊[%s]\n",pack.data.recv_name);
				break;


			case 13:
				printf("群[%s]有新消息\n",pack.data.mes2);
				break;
			case 14:
				printf("[%s]的<%s>说：%s\n",pack.data.mes2,pack.data.send_name,pack.data.mes1);
				break;
			case 15:
				printf("<%s>向您发送文件'%s'\n",pack.data.send_name,pack.data.mes2);
				break;
			case 16:
				down_file(pack);
				break;



		}			
    	}
   
}

void func_menu()
{
    int noc;
    do {
        printf("\n\t\t=============================\n");
       	printf("\t\t|           主菜单          |\n");
       	printf("\t\t|                           |\n");


        printf("\t\t|        2.  全部好友       |\n");
        printf("\t\t|        3.  在线好友       |\n");
        printf("\t\t|        4.  添加好友       |\n");
        printf("\t\t|        5.  删除好友       |\n");
 	printf("\t\t|        6.  发起聊天       |\n");
	printf("\t\t|        7.  对话框         |\n");
	
	printf("\t\t|        8.  我的群         |\n");
        printf("\t\t|        9.  创建新群       |\n");
        printf("\t\t|       10.  加入群         |\n");
        printf("\t\t|       11.  退出群         |\n");
        printf("\t\t|       12.  聊天记录       |\n");

        printf("\t\t|       13.  发群消息       |\n");
	printf("\t\t|       14.  群对话框       |\n");

        printf("\t\t|       15.  发送文件       |\n");
	printf("\t\t|       16.  接收文件       |\n");
        printf("\t\t|        0.  退出           |\n");

       	printf("\t\t|                           |\n");
   
        printf("\t\t=============================\n");
        
	printf("请输入：\n");
	scanf("%d",&noc);
	if (noc==0)
		break;
	func(noc);
        sleep(3);
        }while(1);
} 



void func(int noc)
{
	switch(noc)
	{
		case 2:
			printf("您的好友有：\n\n");
			all_friend();
			break;
		case 3:
			printf("您的在线好友有：\n\n");
			online_friend();
			break;
		case 4:
			add_friend();
			break;
		case 6:
			chat_p();
			break;
		case 7:
			chat_p_box();
			break;
		case 8:
			printf("您的群组有：\n\n");
			all_group();
			break;
		case 10:
			add_group();
			break;

		case 13:
			chat_g();
			break;
		case 14:
			chat_g_box();
			break;
		case 15:
			send_file();
			break;
		case 16:
			recv_file();
			break;



	}
	return;
}

void all_friend()
{
	PACK pack;
	pack.type=2;
	strcpy(pack.data.send_name,myname);

	send(cfd,&pack,sizeof(PACK),0);
}

void online_friend()
{
	PACK pack;
	pack.type=3;
	strcpy(pack.data.send_name,myname);

	send(cfd,&pack,sizeof(PACK),0);
}

void fri_list(PACK pack)
{
	printf("===>%s\n",pack.data.mes1);

}

void chat_p()
{
	PACK pack;
	pack.type=6;

	printf("聊天用户：\n");
	scanf("%s",pack.data.recv_name);
	strcpy(pack.data.send_name,myname);
	do{
	printf("聊天内容：\n");
	scanf("%*c%[^\n]",pack.data.mes1);
	send(cfd,&pack,sizeof(PACK),0);
	
	}while(strcmp(pack.data.mes1,"...")!=0);

}

void chat_p_box()
{
	PACK pack;
	pack.type=7;
	printf("========================================\n");
	printf("未读消息：\n\n");
	strcpy(pack.data.send_name,myname);
	send(cfd,&pack,sizeof(PACK),0);

}



void all_group()
{
	PACK pack;
	pack.type=8;
	strcpy(pack.data.send_name,myname);

	send(cfd,&pack,sizeof(PACK),0);
}


void chat_g()
{
	PACK pack;
	pack.type=13;

	printf("聊天群：\n");
	scanf("%s",pack.data.mes2);
	strcpy(pack.data.send_name,myname);
	do{
	printf("聊天内容：\n");
	scanf("%*c%[^\n]",pack.data.mes1);
	send(cfd,&pack,sizeof(PACK),0);
	
	}while(strcmp(pack.data.mes1,"...")!=0);
}


void chat_g_box()
{
	PACK pack;
	pack.type=14;
	printf("========================================\n");
	printf("未读群消息：\n\n");
	strcpy(pack.data.send_name,myname);
	send(cfd,&pack,sizeof(PACK),0);

}


void send_file(){
	PACK pack;
	char filename[1000];
	struct stat buf;

	int fd;
	int fsize;


	memset(&pack, 0, sizeof(PACK));
	pack.type=15;
	strcpy(pack.data.send_name,myname);

	printf("收件人：\n");
	scanf("%s",pack.data.recv_name);
	

	printf("文件绝对路径：\n");
	scanf("%s",filename);

	printf("发送文件名：\n");
	scanf("%s",pack.data.mes2);


	if(lstat(filename, &buf) == -1) {
        printf("找不到该文件\n");
    	}
    
	if( S_ISDIR(buf.st_mode)) {
        printf("暂不支持传送文件夹\n");
    	}
    
	else 
	{
        	if ((fd = open(filename, O_RDONLY)) == -1) {
            		perror("open_the_file");
        	}
        	while ((fsize = read(fd,pack.data.mes1, sizeof(pack.data.mes1))) > 0) 
		{
            		pack.data.send_fd = fsize;
            		if(send(cfd, &pack, sizeof(PACK), 0) < 0) {
                		perror("send_the_file");
            		}

            		memset(pack.data.mes1, 0, sizeof(pack.data.mes1));
                        
       		}

		printf("文件上传成功\n");
               
    	}
        close(fd);

}


void recv_file()
{
	PACK pack;
	pack.type=16;
	printf("========================================\n");
	printf("文件传输中：\n\n");
	strcpy(pack.data.send_name,myname);
	send(cfd,&pack,sizeof(PACK),0);

}


void down_file(PACK pack)
{
	int fd;
    	int fsize;

	if( (fd = open(pack.data.mes2, O_RDWR|O_CREAT|O_APPEND, S_IRUSR| S_IWUSR)) == -1) {
        	perror("down_file");
    	}
        if( (fsize = write(fd, pack.data.mes1, pack.data.send_fd)) < 0) 		{
        	perror("write_file");
    	}	

	if(fsize<sizeof(pack.data.mes1))	
	{
		printf("文件下载成功\n");
	}
	close(fd);
}

void add_friend()
{
	PACK pack;
	
	pack.type=4;

	printf("请输入好友昵称：\n");
	scanf("%s",pack.data.recv_name);
	strcpy(pack.data.send_name,myname);

	send(cfd,&pack,sizeof(PACK),0);
}


void add_group()
{
	PACK pack;
	
	pack.type=10;

	printf("要加入的群组：\n");
	scanf("%s",pack.data.recv_name);
	strcpy(pack.data.send_name,myname);

	send(cfd,&pack,sizeof(PACK),0);
}



