#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<mysql.h>
#include<unistd.h>

void my_err(const char* errnotice,int line)
{
	fprintf(stderr,"line:%d",line);
	perror(errnotice);
	exit(1);
}


MYSQL accept_mysql(void)
{
	MYSQL               mysql;
	
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

int login_mysql(MYSQL mysql1)
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

	int                 i;
	int                 ret;
	unsigned int        num_fields;//记录表的列数
	MYSQL               mysql = mysql1;
	MYSQL_RES           *result = NULL;//查询结果的所有列，返回数据集
	MYSQL_ROW           row;//查询出的某一行，mysql_fetch_row()
	MYSQL_FIELD         *field;//
	ret = mysql_query(&mysql, mingl);//成功？返回0；
	if(ret){
		printf("query fail\n");
		return -1;
	}

	result = mysql_store_result(&mysql);//获取完整数据集,失败?返回null
	if(result){
			if(row = mysql_fetch_row(result)){//获取密码，在row[0]
				printf("%-20s\n", row[0]);
				}
			}
	if(!strcmp(row[0],login_psw))
		printf("密码正确！！！\n");
	else
		printf("密码错误\n");
	
	mysql_free_result(result);

	printf("login over \n");
	return 0;
	
}



int main()
{
	char mingl[100];
//	const char *pmingl;
//	pmingl=mingl;
	MYSQL mysql;
	mysql=accept_mysql();
//	printf("输入您的命令：");
//	scanf("%[^\n]",mingl);
//	printf("您的命令为：%s\n",mingl);
//	printf("您的命令为：%s\n",pmingl);

	
	login_mysql(mysql);

	printf("use back\n");
	close_mysql(mysql);

	printf("bybye~\n");
	return 0;
}
