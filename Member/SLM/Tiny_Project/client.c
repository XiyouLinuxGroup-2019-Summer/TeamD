#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>
#include <strings.h>

 
#define SERV_PORT 6666
#define SERV_IP "127.0.0.1"

int main(void)
{

    int cfd, n, c;
    struct sockaddr_in serv_addr;   //储存服务器IP地址
    //socklen_t serv_addr_len;
    char buf[BUFSIZ];

    cfd = socket(AF_INET,SOCK_STREAM,0);

    bzero(&serv_addr,sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(SERV_PORT);
    inet_aton("127.0.0.1",&serv_addr.sin_addr);
    connect(cfd,(struct sockaddr *)&serv_addr,sizeof(serv_addr));

    while(1){
    fgets(buf,sizeof(buf),stdin);   
    //fgets()函数在输入结束时以'\n'为标识，但在最后为键入的字符串末尾加上'\0';
    //所以接下来write()函数，从buf写入strlen(buf)长度的字符串到cfd;
    write(cfd,buf,strlen(buf));

    n=read(cfd,buf,sizeof(buf));
    write(STDOUT_FILENO, buf, n);
    }
    close(cfd);
    
    return 0;

}
