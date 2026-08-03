#include "myhead.h"

/*
    多个客户端连接同一个服务器
	  1.服务器：accept会不断地产生新的套接字
*/ 



int main()
{
	int ret;
	int newsock; //存放accept返回的新套接字
	int tcpsock; //旧的套接字
	char rbuf[100];
	//定义ipv4地址结构体变量存放需要绑定的ip和端口号
	struct sockaddr_in bindaddr;
	bzero(&bindaddr,sizeof(bindaddr));
	bindaddr.sin_family=AF_INET; //地址协议类型，ipv4
	bindaddr.sin_addr.s_addr=htonl(INADDR_ANY); //自动匹配本地ubuntu的ip地址
	bindaddr.sin_port=htons(10000); //服务器的端口号，要跟客户端连接时候使用的一致
	
	//定义ipv4地址结构体变量存放目前连接成功的那个客户端的ip和端口号
	struct sockaddr_in clientaddr;
	bzero(&clientaddr,sizeof(clientaddr));
	int size=sizeof(clientaddr);
	
	//创建tcp套接字
	tcpsock=socket(AF_INET,SOCK_STREAM,0);
	if(tcpsock==-1)
	{
		perror("创建tcp套接字失败了\n");
		return -1;
	}
	
	//绑定ip和端口号
	ret=bind(tcpsock,(struct sockaddr *)&bindaddr,sizeof(bindaddr));
	if(ret==-1)
	{
		perror("绑定ip和端口号失败了\n");
		return -1;
	}
	
	//监听
	ret=listen(tcpsock,5);
	if(ret==-1)
	{
		perror("监听失败了\n");
		return -1;
	}
	printf("旧的套接字文件描述符是: %d\n",tcpsock);
	
	//死循环里面不断地接收客户端连接请求
	while(1)
	{
		//愿意接收客户端的连接请求(重点)
		printf("服务器阻塞在accept的位置\n");
		newsock=accept(tcpsock,(struct sockaddr *)&clientaddr,&size);
		if(newsock==-1)
		{
			perror("服务器接收客户端连接请求失败了\n");
			return -1;
		}
		printf("accept产生的新的套接字文件描述符是: %d\n",newsock);
		//打印目前连接成功的那个客户端信息
		printf("目前连接成功的那个客户端ip地址: %s\n",inet_ntoa(clientaddr.sin_addr));
		printf("目前连接成功的那个客户端端口号: %hu\n",ntohs(clientaddr.sin_port));
	}

	
	//关闭套接字
	close(tcpsock);
	close(newsock);
	return 0;
}