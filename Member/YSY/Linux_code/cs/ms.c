#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <ctype.h>
#include <strings.h>

#define SERV_PORT 4004
#define SERV_IP "127.0.0.1"

int main(void)
{
    int lfd,cfd;
    struct sockaddr_in serv_addr,clie_addr;
    socklen_t clie_addr_len, clie_IP_len;
    char buf[BUFSIZ],clie_IP[BUFSIZ];
    int n,i;
	int timess=0;

	int xx=7,yy;

    lfd = socket(AF_INET, SOCK_STREAM, 0);

    bzero(&serv_addr,sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(SERV_PORT);
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY); //inet_pton(SERV_PORT);
    bind(lfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr));

    listen(lfd,128);

    clie_addr_len = sizeof(clie_addr);
    cfd = accept(lfd,(struct sockaddr *)&clie_addr,&clie_addr_len);
    printf("client IP:%s, client PORT:%d\n",
           inet_ntop(AF_INET, &(clie_addr.sin_addr.s_addr), clie_IP, sizeof(clie_IP)),
           ntohs(clie_addr.sin_port));

    while(1)
    {
        n = read(cfd,buf,sizeof(buf));
//		xx=read(cfd,buf,sizeof(buf));
		timess++;
		yy=(int)buf[0]-48;
		printf("第%d次传入为：%syy=%d\n",timess,buf,yy);
       
		if(yy==xx)
			printf("\t登录成功\n");
		else
			printf("\t失败\n");
		
		
		
/*		 for(i=0;i<n;i++)
        {
           buf[i]=toupper(buf[i]);
        }
*/		
        write(cfd,buf,n);
		printf("write::cfd==%d\n\n",cfd);
    }
        close(lfd);
        close(cfd);

    return 0;
}

