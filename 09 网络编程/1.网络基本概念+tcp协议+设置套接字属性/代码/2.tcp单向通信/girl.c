#include "myhead.h"

/*
    服务器代码
	为了演示方便，客户端和服务器都在教师机上运行
	单向通信：客户端键盘输入信息发送给服务器
	 1.accept函数是重点
	      (1)该函数是阻塞的，如果没有客户端连接服务器，服务器会一直阻塞到accept的位置
		                      如果有客户端连接服务器成功，accept就会立马解除阻塞
		  (2)第二个参数用来自动存放当前连接成功的那个客户端ip和端口号
               自动存放的理解：假设客户端张三连接服务器成功，此时accept函数会自动把张三的ip和端口号保存到第二个参数地址中
                               	假设客户端李四连接服务器成功，此时accept函数会自动把李四的ip和端口号保存到第二个参数地址中		   
               我想把这个客户端信息打印出来，可以不？
			   答案：可以，但是你需要调用专门的函数把大端序转换成小端序
		  (3)accept的返回值
		     成功：返回新的套接字
			 疑问：服务器最开始调用socket已经创建了一个套接字
			       为什么现在accept还要产生新的套接字
				   新的套接字用来干什么？
			 答案：tcp服务器允许多个客户端连接同一个服务器
                   服务器为了区分不同的客户端，accept产生新的不同的套接字用来区分不同的客户端			 
			 失败：返回-1
			      

*/ 

int main()
{
	int ret;
	int tcpsock; //旧的套接字
	int newsock; //存放accept返回的新套接字
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
	//错误示范：错误的理由sin_addr存放的是大端序格式的ip，而linux系统打印，需要是小端序
	//printf("目前连接成功的那个客户端ip地址: %s\n",clientaddr.sin_addr);
	//printf("目前连接成功的那个客户端端口号: %hu\n",clientaddr.sin_port);
	//正确的示范：大端序格式的ip--》转成小端序格式
	printf("目前连接成功的那个客户端ip地址: %s\n",inet_ntoa(clientaddr.sin_addr));
	printf("目前连接成功的那个客户端端口号: %hu\n",ntohs(clientaddr.sin_port));
	
	//循环接收客户端发送过来的信息
	while(1)
	{
		bzero(rbuf,100);
		//服务器有两个套接字：旧的套接字跟新的套接字
		//一定要使用新的套接字来跟客户端通信
		recv(newsock,rbuf,100,0); //前面三个参数跟学过read一模一样的含义
		printf("服务器收到的信息是: %s\n",rbuf);
	}
	
	//关闭套接字
	close(tcpsock);
	close(newsock);
	return 0;
}