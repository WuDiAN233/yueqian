#include "myhead.h"

/*
   多路复用实现tcp单向通信
   客户端键盘输入数据发送给服务器
   分析服务器：
       第一步：思考服务器需要监测哪些文件描述符
	           新的套接字是否有数据可读
			   新的套接字有数据可读：客户端给服务器发了信息，服务器才有数据可读
	   第二步：定义文件描述符集合存放要监测的文件描述符
       第三步：select来监测新的套接字有没有数据可读	   
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
	
	//设置端口号复用(程序退出之后，虽然端口号被占用，但是立马运行程序，依然可以使用之前的端口号)
	int on=1; //非零,相当于是开关，开启设置端口号复用
	setsockopt(tcpsock,SOL_SOCKET,SO_REUSEADDR,&on,sizeof(on));
	
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
	
	//定义文件描述符集合 
	fd_set myset;
	FD_ZERO(&myset); //清空集合
	FD_SET(newsock,&myset); //把新的套接字添加到集合中
	
	while(1)
	{
		//使用多路复用监测新的套接字是否有数据可读
		printf("select阻塞当前进程，如果文件描述符不发生状态改变，会一直阻塞\n");
		ret=select(newsock+1,&myset,NULL,NULL,NULL);
		if(ret>0) //说明新的套接字有数据可读--》说明客户端发送信息过来，才有数据可读
		{
			bzero(rbuf,100);
			//调用recv接收信息
			ret=recv(newsock,rbuf,100,0);
			if(ret==0) //对方客户端断开了
			{
				printf("客户端断开了\n");
				exit(0);
			}
			printf("客户端发送过来的信息是: %s\n",rbuf);
		}
	}
	
	//关闭套接字
	close(tcpsock);
	close(newsock);
	return 0;
}

