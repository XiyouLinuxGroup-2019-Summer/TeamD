#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<mysql.h>

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

int login_mysql(const char *pmingl, MYSQL mysql1)
{
	printf("use来啦\n");
	printf("执行\"%s\",是吗?\n",pmingl);

	int timess=0;

	int                 i;
	int                 ret;
	unsigned int        num_fields;//记录表的列数
	MYSQL               mysql = mysql1;
	MYSQL_RES           *result = NULL;//查询结果的所有列，返回数据集
	MYSQL_ROW           row;//查询出的某一行，mysql_fetch_row()
	MYSQL_FIELD         *field;//
	ret = mysql_query(&mysql, pmingl);//成功？返回0；
	if(ret){
		printf("query fail\n");
		return -1;
	}

	//成功
		result = mysql_store_result(&mysql);//获取完整数据集,失败?返回null
		if(result){
			num_fields = mysql_num_fields(result);//表的总列数
			field = mysql_fetch_field(result);
			printf("我们表有%u列\n",num_fields);
		//	printf("%-20s", field->name);//打印整个	}
			printf("\n2333333\n\n");

			while(row = mysql_fetch_row(result))//检索一个结果集合的下一行
							    //就问你！怕不怕？昂？！怕了吧！
			{				
				timess++;//在此循环
				printf("\n这是第%d次\n",timess);
				for(i = 0; i < num_fields; i++)
				{
					if(row[i]){
						printf("%-10s",row[i]);
					}
				}
				printf("\n本循环结束");

			}
			printf("\n所有循环结束\n");
		}
		mysql_free_result(result);
		

	printf("use over \n");
	return 0;
	
}



int main()
{
	char mingl[100];
	const char *pmingl;
	pmingl=mingl;
	MYSQL mysql;
	mysql=accept_mysql();
	printf("输入您的命令：");
	scanf("%[^\n]",mingl);
	printf("您的命令为：%s\n",mingl);
	printf("您的命令为：%s\n",pmingl);

	
	login_mysql(pmingl,mysql);

	printf("use back\n");
	close_mysql(mysql);

	printf("bybye~\n");
	return 0;
}
