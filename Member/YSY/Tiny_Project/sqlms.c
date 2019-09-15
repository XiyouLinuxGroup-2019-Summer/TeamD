#include<stdio.h>
#include<string.h>
#include<unistd.h>
#include<sys/socket.h>
#include<sys/epoll.h>
#include<stdlib.h>
#include<arpa/inet.h>
#include<netinet/in.h>
#include<fcntl.h>
#include<errno.h>
#include<ctype.h>
#include<strings.h>
#include<mysql/mysql.h>

//宏定义
#define SERV_PORT 	4004
#define SERV_IP 	"192.168.3.212"

#define EPOLL_MAX	200000
#define MAX_CHAR	300

//结构体
typedef struct datas{
    char     send_name[MAX_CHAR];
    char     recv_name[MAX_CHAR];
    int      send_fd;
    int      recv_fd;
    char     mes1[MAX_CHAR*2];
    char	mes2[MAX_CHAR*2];
}DATA;

typedef struct package{
    int   type;
    DATA  data;
}PACK;
 

typedef struct friends{
    int fid[30];
    char name[30][30];
    int online[30];
    int len;
}FRI;
 
struct pchats{
    char     send_name[MAX_CHAR];
    char     recv_name[MAX_CHAR];
    int      send_fd;
    int      recv_fd;
    char     mes1[MAX_CHAR*2];
    int	     type;
    struct pchats *     next;    
};

typedef struct pchats PC;


struct gchats{
    char     send_name[MAX_CHAR];
    char     gname[MAX_CHAR];
    char     mes1[MAX_CHAR*2];
    int      root[50];
    struct gchats *     next;    };

typedef struct gchats GC;


struct fnodes{
    char     send_name[MAX_CHAR];
    char     recv_name[MAX_CHAR];
    int      send_fd;
    int      recv_fd;
    char     mes1[MAX_CHAR*2];
    char     mes2[MAX_CHAR*2];
    struct fnodes *     next;    
};

typedef struct fnodes FL;
//全局变量

MYSQL		mysql;
MYSQL_RES           *result = NULL;//查询结果的所有列，返回数据集
MYSQL_ROW           row;//查询出的某一行，mysql_fetch_row()
MYSQL_FIELD         *field;//

struct epoll_event ev, events[1000];


static int	now_fd;//dangqiande

PC * pchead=NULL;
PC * pctail=NULL;

GC * gchead=NULL;
GC * gctail=NULL;

FL * flhead=NULL;
FL * fltail=NULL;
int epollfd;
//函数声明

void my_err(const char* ,int);
MYSQL accept_mysql(void);
int close_mysql(MYSQL);
void deal(PACK);
void deal_login(PACK);
void send_PACK(PACK);
void deal_all_friend(PACK);
void deal_online_friend(PACK);
void add_friend(PACK);


void deal_chat_p(PACK);
void deal_p_box(PACK);

void deal_all_group(PACK);
void deal_chat_g(PACK);
void deal_g_box(PACK);
void deal_file_in(PACK);
void deal_file_out(PACK);
void deal_quit(PACK);
void add_group(PACK);
//数据库的函数们
void my_err(const char* errnotice,int line)
{
	fprintf(stderr,"line:%d",line);
	perror(errnotice);
	exit(1);
}


MYSQL accept_mysql(void)
{
	char mingl[1000];
	
	if(NULL == mysql_init(&mysql)){
		my_err("mysql_init", __LINE__);
	}

	//初始化数据库
	if(mysql_library_init(0, NULL, NULL) != 0){
		my_err("mysql_library_init", __LINE__);
	}

	//连接数据库
	if(NULL == mysql_real_connect(&mysql, "127.0.0.1", "root", "hiahiahia", "test88", 0, NULL, 0)){
		my_err("mysql_real_connect", __LINE__);
	}

	//设置中文字符集
	if(mysql_set_character_set(&mysql, "utf8") < 0){
		my_err("mysql_set_character_set", __LINE__);
	}
	
	printf("连接mysql数据库成功!\n");

	//用户状态初始化
	sprintf(mingl,"update user set sockkk = NULL");
        if (mysql_query(&mysql, mingl)) {
                    perror("init_mysql_sock:mysql_query");
        }

	sprintf(mingl,"update user set online = 0");
        if (mysql_query(&mysql, mingl)) {
                    perror("init_mysql_online:mysql_query");
        }
        printf("用户状态初始化成功\n");


	return mysql;
}

int close_mysql(MYSQL mysql)
{
	printf("start close\n");
	mysql_close(&mysql);
	printf("halouha\n");
	mysql_library_end();
	printf("end\n");
	return 0;
}
void deal_quit(PACK pack)
{
	char mingl[1000];
	sprintf(mingl,"update `user` set online = 0,sockkk=NULL where name = '%s'",pack.data.send_name);
	mysql_query(&mysql,mingl);	
	printf("\n'%s' already quit\n\n",pack.data.send_name);
	//注销
	epoll_ctl(epollfd, EPOLL_CTL_DEL, now_fd, &ev);
        close(now_fd);
}




int main(void)
{
    int lfd,cfd,fd_num,socketfd;
    struct sockaddr_in serv_addr,clie_addr;
    socklen_t clie_addr_len, clie_IP_len;
    char buf[BUFSIZ],clie_IP[BUFSIZ];
    int err;
//    struct epoll_event ev, events[1000];

	accept_mysql();

//链表初始化	
	if (pchead==NULL){
		pchead=(PC*)malloc(sizeof(PC));
		pchead->next=NULL;
		pctail=pchead;
	}

	if (gchead==NULL){
		gchead=(GC*)malloc(sizeof(GC));
		gchead->next=NULL;
		gctail=gchead;
	}

	if (flhead==NULL){
		flhead=(FL*)malloc(sizeof(FL));
		flhead->next=NULL;
		fltail=flhead;
	}

//listenfd
    lfd = socket(AF_INET, SOCK_STREAM, 0);

    bzero(&serv_addr,sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(SERV_PORT);
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY); //inet_pton(SERV_PORT);
    bind(lfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr));

 //   listen(lfd,128);
//create
	epollfd = epoll_create(EPOLL_MAX);

 	err = 1;

    setsockopt(lfd,SOL_SOCKET,SO_REUSEADDR,&err,sizeof(int)); 
 
    ev.data.fd = lfd;//设置与要处理事件相关的文件描述符
    ev.events = EPOLLIN;//设置要处理的事件类型

	epoll_ctl(epollfd, EPOLL_CTL_ADD, lfd, &ev);//注册事件函数

	listen(lfd,10);
//epoll框架
       	while(1)
    	{

        fd_num = epoll_wait(epollfd, events, EPOLL_MAX, 1000);
	if(fd_num==-1){
		printf("wait_error\n");
		exit(0);
	}
	for(int i=0; i<fd_num; i++)
        {
            if(events[i].data.fd == lfd)
            {
		//accept
              //  socketfd = accept(lfd,(SA*)&fromaddr,&len);
		socketfd = accept(lfd,(struct sockaddr *)&clie_addr,&clie_addr_len);
                printf("%d get conect!\n",socketfd);
                ev.data.fd = socketfd;
                ev.events = EPOLLIN;//设置监听事件可写
                //新增套接字
                epoll_ctl(epollfd, EPOLL_CTL_ADD, socketfd, &ev);
            }
            else if(events[i].events & EPOLLIN)//可读
            {
	//	printf("可读\n");
		PACK pack;
		now_fd=events[i].data.fd;
	//	printf("now_fd=%d\n",now_fd);
		int ret;
		//错误
		if((ret=recv(now_fd,&pack,sizeof(PACK),0))<0)
			perror("server_recv");
	//	else
	//		printf("shoubaochengong:ret=%d\n",ret);
		//退出
		if(ret==0){
			char mingl[1000];
			sprintf(mingl,"update `user` set online = 0 where name = '%s'",pack.data.send_name);
			mysql_query(&mysql,mingl);	
			printf("'%s' already quit\n",pack.data.send_name);
			//注销
			epoll_ctl(epollfd, EPOLL_CTL_DEL, now_fd, &ev);
            
			close(now_fd);
		    }
	    	//普通好包
		deal(pack);
	    }
	   else if(events[i].events & EPOLLOUT)//         
	   {
		   printf("可写\n");
		   //子函数
	   }

        }
    }
    return 0;
}



//deal
void deal(PACK pack)
{
	int type=pack.type;
	printf("bao:type=%d\n",type);
	
	switch(type)
	{
		case 1:
			deal_login(pack);
			break;
		case 2:
			deal_all_friend(pack);
			break;
		case 3:
			deal_online_friend(pack);
			break;

		case 4:
			add_friend(pack);
			break;

		case 6:
			deal_chat_p(pack);
			break;
		case 7:
			deal_p_box(pack);
			break;
		case 8:
			deal_all_group(pack);
			break;

		case 10:
			add_group(pack);
			break;
		case 13:
			deal_chat_g(pack);
			break;
		case 14:
			deal_g_box(pack);
			break;
		case 15:
			deal_file_in(pack);
			break;
		case 16:
			deal_file_out(pack);
			break;
		case 100:
			deal_quit(pack);
			


	}

}


void deal_login(PACK pack)
{
    PACK send_pack;
  
    int ret;
    char mingl[1000];
    PC *p;
    FL *p1;
    GC *p2;
    int flag=0;
    int uuid;

    sprintf(mingl, "select password,uid from user where name ='%s'",pack.data.send_name);
    ret = mysql_query(&mysql,mingl);
    if(!ret)
	    printf("chazhaochenggong\n");

    send_pack.type = -1;

    if (!ret) {
        result = mysql_store_result(&mysql);    //返回查询结果
        if (!result) {
            perror("login:mysql_store_result");
        }  

	row=mysql_fetch_row(result);
	uuid=atoi(row[1]);
	
       	if (strcmp(pack.data.mes1, row[0]) == 0)
	{//密码正确
                printf("%s 登录成功\n",pack.data.send_name);
                sprintf(mingl,"update user set online = 1 ,sockkk = %d where name ='%s' ",now_fd,pack.data.send_name);
                if (mysql_query(&mysql, mingl)) {
                    perror("login:mysql_query");
                }

                mysql_free_result(result);

                send_pack.type = 0;
        	
		p=pchead->next;
		while(p!=NULL)
		{
			if(strcmp(p->recv_name,pack.data.send_name)==0){
				p->recv_fd=now_fd;
				flag++;
			}
			p=p->next;
		}
		send_pack.data.recv_fd=flag;

		flag=0;
		p1=flhead->next;
		while(p1!=NULL)
		{
			if(strcmp(p1->recv_name,pack.data.send_name)==0){
				flag++;
			}
			p1=p1->next;
		}
		send_pack.data.send_fd=flag;

		flag=0;
		p2=gchead->next;
		while(p2!=NULL)
		{
	//		printf("root:uuid=%d",p2->root[uuid]);
			if(p2->root[uuid]){
				flag++;
			}
			p2=p2->next;
		}
	//	printf("flag=%d",flag);
		if(flag)
			strcpy(send_pack.data.mes2,pack.data.send_name);



		printf("================\n链表调整完毕\n===============\n");

	}
    }

    send_PACK(send_pack);
   
}


void send_PACK(PACK pack)
{
    int ret;
    if((ret = send(now_fd,&pack,sizeof(PACK),0))<0) {
        perror("server_deal ：send");
    }
}

void deal_all_friend(PACK pack)
{
	PACK send_pack;
	char mingl[1000];
	int uid;

	FRI myfri;
	int i=0;
	
	//对应id
	sprintf(mingl, "select uid from user where name ='%s'",pack.data.send_name);
	mysql_query(&mysql,mingl);
	result=mysql_store_result(&mysql);

	row=mysql_fetch_row(result);
	uid=atoi(row[0]);
	printf("%s的id是%d\n",pack.data.send_name,uid);
	//查询好友
	sprintf(mingl, "select fid from friend where uid =%d",uid);
	mysql_query(&mysql,mingl);
	result=mysql_store_result(&mysql);
	while(row=mysql_fetch_row(result)){
		myfri.fid[i]=atoi(row[0]);
		i++;
	}
	myfri.len=i;
	myfri.fid[i]=-1;
	//转换name
	for(i=0;i<myfri.len;i++){
		sprintf(mingl, "select name , online from user where uid = %d",myfri.fid[i]);
		mysql_query(&mysql,mingl);
		result=mysql_store_result(&mysql);

		row=mysql_fetch_row(result);
		strcpy(myfri.name[i],row[0]);
		myfri.online[i]=atoi(row[1]);	
	}
	//开始发包
	send_pack.type=2;
	for(i=0;i<myfri.len;i++)
	{
		strcpy(send_pack.data.mes1,myfri.name[i]);
		send_PACK(send_pack);
	}
}



void deal_online_friend(PACK pack)
{
	PACK send_pack;
	char mingl[1000];
	int uid;

	FRI myfri;
	int i=0;
	
	//对应id
	sprintf(mingl, "select uid from user where name ='%s'",pack.data.send_name);
	mysql_query(&mysql,mingl);
	result=mysql_store_result(&mysql);

	row=mysql_fetch_row(result);
	uid=atoi(row[0]);
	printf("%s的id是%d\n",pack.data.send_name,uid);
	//查询好友
	sprintf(mingl, "select fid from friend where uid =%d",uid);
	mysql_query(&mysql,mingl);
	result=mysql_store_result(&mysql);
	while(row=mysql_fetch_row(result)){
		myfri.fid[i]=atoi(row[0]);
		i++;
	}
	myfri.len=i;
	myfri.fid[i]=-1;
	//转换name
	for(i=0;i<myfri.len;i++){
		sprintf(mingl, "select name , online from user where uid = %d",myfri.fid[i]);
		mysql_query(&mysql,mingl);
		result=mysql_store_result(&mysql);

		row=mysql_fetch_row(result);
		strcpy(myfri.name[i],row[0]);	
		myfri.online[i]=atoi(row[1]);	
	}
	//开始发包
	send_pack.type=3;
	for(i=0;i<myfri.len;i++)
	{
		if(myfri.online[i]!=0)
		{strcpy(send_pack.data.mes1,myfri.name[i]);
		send_PACK(send_pack);
		}
	}
}

void deal_chat_p(PACK pack)
{
	char mingl[1000];
	int uid;
	int fid;
	int usock;
	int fsock;
	int fonline;
	PC * pcnew;

	//一、破译包内信息
	sprintf(mingl, "select uid,sockkk from user where name ='%s'",pack.data.send_name);
	mysql_query(&mysql,mingl);
	result=mysql_store_result(&mysql);

	row=mysql_fetch_row(result);
	uid=atoi(row[0]);
	usock=atoi(row[1]);

	sprintf(mingl, "select uid,sockkk,online from user where name ='%s'",pack.data.recv_name);
	mysql_query(&mysql,mingl);
	result=mysql_store_result(&mysql);

	row=mysql_fetch_row(result);
	fid=atoi(row[0]);

	fonline=atoi(row[2]);
	if(fonline){
		fsock=atoi(row[1]);
		send(fsock,&pack,sizeof(PACK),0);

	}
	//二、记录信息到数据库
	sprintf(mingl,"insert into pchat(uid,fid,pmes) values(%d,%d,\"%s\")",uid,fid,pack.data.mes1);
        if (mysql_query(&mysql, mingl)) {
                    perror("pchat:mysql_query");
        }

	printf("mysql is okkkkkkkk\n");
	//三、存至链表

	pcnew=(PC*)malloc(sizeof(PC));

	strcpy(pcnew->send_name,pack.data.send_name);
	strcpy(pcnew->recv_name,pack.data.recv_name);
	strcpy(pcnew->mes1,pack.data.mes1);
	pcnew->recv_fd=fsock;
	pcnew->send_fd=usock;
	pcnew->next=NULL;
	
	pctail->next=pcnew;
	pctail=pcnew;


}


void deal_p_box(PACK pack)
{
	PACK send_pack;
	PC *r,*p;

	r=pchead;
	p=r->next;

	while(r->next!=NULL)
	{
			if(strcmp(p->recv_name,pack.data.send_name)){
			r=p;
			p=p->next;
		}
		else{
			send_pack.type=7;
			strcpy(send_pack.data.send_name,p->send_name);
			strcpy(send_pack.data.mes1,p->mes1);
			send(p->recv_fd,&send_pack,sizeof(PACK),0);		

			p=p->next;
			free(r->next);
			r->next=p;
		}
	}
	pctail=r;
}


void deal_all_group(PACK pack)
{
	PACK send_pack;
	char mingl[1000];

	sprintf(mingl, "select * from groups  where gid in(select gid from members where uid in (select uid from user where name = '%s'))",pack.data.send_name);
	mysql_query(&mysql,mingl);
	result=mysql_store_result(&mysql);

	while(row=mysql_fetch_row(result)){
		send_pack.type=8;
		strcpy(send_pack.data.mes1,row[1]);
		send_PACK(send_pack);
	}
}

void deal_chat_g(PACK pack)
{
	char mingl[1000];
	int uid;
	int gid;
//	int usock;
	int fsock;
	int fonline;
	
//	int i=0;
	GC * gcnew;
//	int mems[50];

	gcnew=(GC*)malloc(sizeof(GC));

	//一、破译包内信息
	sprintf(mingl, "select uid from user where name ='%s'",pack.data.send_name);
	mysql_query(&mysql,mingl);
	result=mysql_store_result(&mysql);

	row=mysql_fetch_row(result);
	uid=atoi(row[0]);
	memset(&(gcnew->root),0,sizeof(int)*50);
//	usock=atoi(row[1]);

	sprintf(mingl, "select gid from groups where gname ='%s'",pack.data.mes2);
	mysql_query(&mysql,mingl);
	result=mysql_store_result(&mysql);

	row=mysql_fetch_row(result);
	gid=atoi(row[0]);


	sprintf(mingl, "select uid from members where gid =%d",gid);
	mysql_query(&mysql,mingl);
	result=mysql_store_result(&mysql);
	while(row=mysql_fetch_row(result)){
		gcnew->root[atoi(row[0])]=1;
	}
	gcnew->root[uid]=0;

	sprintf(mingl, "select name,online,sockkk from user where uid in (select uid from members where gid = %d)",gid);
	mysql_query(&mysql,mingl);
	result=mysql_store_result(&mysql);
	while(row=mysql_fetch_row(result)){
		if(!strcmp(row[0],pack.data.send_name))
			continue;
		fonline=atoi(row[1]);
		if(fonline){
			fsock=atoi(row[2]);
		send(fsock,&pack,sizeof(PACK),0);
		}
	}
	
	//二、记录信息到数据库
	sprintf(mingl,"insert into gchat(uid,gid,gmes) values(%d,%d,\"%s\")",uid,gid,pack.data.mes1);
        if (mysql_query(&mysql, mingl)) {
                    perror("gchat:mysql_query");
        }
	printf("gchat to mysql is okkkkkkkk\n");
	//三、存至链表

	
	strcpy(gcnew->send_name,pack.data.send_name);
	strcpy(gcnew->gname,pack.data.mes2);
	strcpy(gcnew->mes1,pack.data.mes1);
	
	gcnew->next=NULL;

	gctail->next=gcnew;
	gctail=gcnew;

}





void deal_g_box(PACK pack)
{
	PACK send_pack;
	GC *p;
	char mingl[1000];
	int uid;
	int recv_fd;

	p=gchead->next;

	sprintf(mingl, "select uid,sockkk from user where name ='%s'",pack.data.send_name);
	mysql_query(&mysql,mingl);
	result=mysql_store_result(&mysql);

	row=mysql_fetch_row(result);
	uid=atoi(row[0]);
	recv_fd=atoi(row[1]);
	while(p!=NULL)
	{
		if(p->root[uid]){
			send_pack.type=14;
			strcpy(send_pack.data.send_name,p->send_name);
			strcpy(send_pack.data.mes1,p->mes1);
			strcpy(send_pack.data.mes2,p->gname);

			send(recv_fd,&send_pack,sizeof(PACK),0);		
			p->root[uid]=0;
		}
		p=p->next;
		
	}
}


void deal_file_in(PACK pack)
{

	char mingl[1000];
	int uid;
	int fid;
	int usock;
	int fsock;
	int fonline;
	FL * flnew;
	
	
	flnew=(FL*)malloc(sizeof(FL));

	strcpy(flnew->send_name,pack.data.send_name);
	strcpy(flnew->recv_name,pack.data.recv_name);
	strcpy(flnew->mes1,pack.data.mes1);
	strcpy(flnew->mes2,pack.data.mes2);

	flnew->send_fd=pack.data.send_fd;

	flnew->next=NULL;
	
	fltail->next=flnew;
	fltail=flnew;


	if(flnew->send_fd<sizeof(pack.data.mes1))
	{
		printf("文件%s已储存\n",flnew->mes2);

		sprintf(mingl, "select sockkk,online from user where name ='%s'",pack.data.recv_name);
		mysql_query(&mysql,mingl);
		result=mysql_store_result(&mysql);

		row=mysql_fetch_row(result);
	
		fonline=atoi(row[1]);
		if(fonline){
			fsock=atoi(row[0]);
			send(fsock,&pack,sizeof(PACK),0);

		}

	}		
}


void deal_file_out(PACK pack)
{
	PACK send_pack;
	FL *r,*p;
	char mingl[1000];
	int fsock;


	sprintf(mingl, "select sockkk from user where name ='%s'",pack.data.send_name);
	mysql_query(&mysql,mingl);
	result=mysql_store_result(&mysql);

	row=mysql_fetch_row(result);
	fsock=atoi(row[0]);


	r=flhead;
	p=r->next;

	while(r->next!=NULL)
	{
		if(strcmp(p->recv_name,pack.data.send_name)){
			r=p;
			p=p->next;
		}
		else{
			send_pack.type=16;
			strcpy(send_pack.data.send_name,p->send_name);
			strcpy(send_pack.data.recv_name,p->recv_name);
			strcpy(send_pack.data.mes1,p->mes1);
			strcpy(send_pack.data.mes2,p->mes2);
			send_pack.data.send_fd = p->send_fd;

			send(fsock,&send_pack,sizeof(PACK),0);		

			p=p->next;
			free(r->next);
			r->next=p;
		}
	}
	fltail=r;
}

void add_friend(PACK pack){

	int uid;
	int fid;
	char mingl[1000];


	sprintf(mingl, "select uid from user where name ='%s'",pack.data.send_name);
	mysql_query(&mysql,mingl);
	result=mysql_store_result(&mysql);

	row=mysql_fetch_row(result);
	uid=atoi(row[0]);

	sprintf(mingl, "select uid from user where name ='%s'",pack.data.recv_name);
	mysql_query(&mysql,mingl);
	result=mysql_store_result(&mysql);

	row=mysql_fetch_row(result);
	fid=atoi(row[0]);

	sprintf(mingl,"insert into friends(uid,fid) values(%d,%d)",uid,fid);
        if (mysql_query(&mysql, mingl)) {
                    perror("addfriend:mysql_query");
        }

	send(now_fd,&pack,sizeof(PACK),0);


}

void add_group(PACK pack){

	int uid;
	int gid;
	char mingl[1000];


	sprintf(mingl, "select uid from user where name ='%s'",pack.data.send_name);
	mysql_query(&mysql,mingl);
	result=mysql_store_result(&mysql);

	row=mysql_fetch_row(result);
	uid=atoi(row[0]);

	sprintf(mingl, "select gid from groups where gname ='%s'",pack.data.recv_name);
	mysql_query(&mysql,mingl);
	result=mysql_store_result(&mysql);

	row=mysql_fetch_row(result);
	gid=atoi(row[0]);

	sprintf(mingl,"insert into members(uid,gid) values(%d,%d)",uid,gid);
        if (mysql_query(&mysql, mingl)) {
                    perror("addgroup:mysql_query");
        }

	send(now_fd,&pack,sizeof(PACK),0);


}
