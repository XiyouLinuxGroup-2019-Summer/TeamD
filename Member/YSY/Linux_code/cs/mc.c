#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>
#include <strings.h>

 
#define SERV_PORT 4004		//服务器端口
#define SERV_IP "127.0.0.1"	//地址

int main(void)
{

    int cfd, n, c;
    struct sockaddr_in serv_addr;   //储存服务器IP地址
    char buf[BUFSIZ];
	
	int ifc;

    cfd = socket(AF_INET,SOCK_STREAM,0);

    bzero(&serv_addr,sizeof(serv_addr));		//清零serv_addr
    serv_addr.sin_family = AF_INET;				//
    serv_addr.sin_port = htons(SERV_PORT);
    inet_aton("127.0.0.1",&serv_addr.sin_addr);	//把本地地址传到.sin_addr
    ifc=connect(cfd,(struct sockaddr *)&serv_addr,sizeof(serv_addr));	//连接

	if(ifc){									//连接报错
		printf("connect failed\n");
		exit(0);
	}

    while(1){
		fgets(buf,sizeof(buf),stdin);   
    
		write(cfd,buf,strlen(buf));
		printf("\twrite over,cfd=%d\n",cfd);
		n=read(cfd,buf,sizeof(buf));
		printf("\tread over,cfd=%d\n",cfd);
		write(STDOUT_FILENO, buf, n);
		printf("\t循环over,STDOUT_FILENO=%d\n\n",STDOUT_FILENO);
    }
    close(cfd);
    
    return 0;

}

