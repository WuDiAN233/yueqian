#include "myhead.h"

/*
     本地的ubuntu跟阿里云服务器采用tcp双向通信
*/ 
int tcpsock;

//线程的任务函数，专门接收服务器的信息
void *recv_server_msg(void *arg)
{
	char rbuf[100];
	int ret;
	while(1)
	{
		bzero(rbuf,100);
		//接收服务器回复的信息
		ret=recv(tcpsock,rbuf,100,0);
		if(ret==0) //说明对方(服务器)断开了(强行终止退出程序了)
		{
			printf("服务器宕机了，此时recv返回值是： %d\n",ret);
			exit(0); //结束整个进程
		}
		printf("阿里云服务器回复给我的信息是: %s\n",rbuf);
	}
}

int main()
{
	int ret;
	pthread_t id;
	char sbuf[100];
	//定义ipv4地址结构体变量存放需要绑定的ip和端口号
	struct sockaddr_in bindaddr;
	bzero(&bindaddr,sizeof(bindaddr));
	bindaddr.sin_family=AF_INET; //地址协议类型，ipv4
	bindaddr.sin_addr.s_addr=htonl(INADDR_ANY); 
	bindaddr.sin_port=htons(10086); //程序员自己指定1024到65535之间的某个端口号
		
	//定义ipv4地址结构体变量存放服务器的ip和端口号
	struct sockaddr_in serveraddr;
	bzero(&serveraddr,sizeof(serveraddr));
	serveraddr.sin_family=AF_INET; 
	//指定服务器的ip地址
	serveraddr.sin_addr.s_addr=inet_addr("8.148.191.21"); //阿里云服务器的ip(阿里云公网的ip地址)
	serveraddr.sin_port=htons(10000); //阿里云服务器的端口号(可以自己指定)
	
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
	
	//连接服务器:三个参数跟bind一模一样的类型
	ret=connect(tcpsock,(struct sockaddr *)&serveraddr,sizeof(serveraddr));
	if(ret==-1)
	{
		perror("连接服务器失败了\n");
		return -1;
	}
	
	//新建一个线程：专门接收服务器的信息
	pthread_create(&id,NULL,recv_server_msg,NULL);
	
	while(1)
	{
		printf("请输入要发送给阿里云服务器的信息!\n");
		scanf("%s",sbuf);
		//发送信息给服务器
		send(tcpsock,sbuf,strlen(sbuf),0);
	}
	
	//关闭套接字
	close(tcpsock);
	return 0;
	
}