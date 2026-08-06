#include "myhead.h"

/*
   多路复用实现tcp单向通信
   客户端键盘输入数据发送给服务器
   分析客户端：
       第一步：思考客户端需要监测哪些文件描述符
	           键盘是否有数据可读
			   键盘有数据可读：用户敲击键盘(对于键盘来说就是有数据可读)，键盘的驱动就会读取用户敲击的字符保存到数组中
	   第二步：定义文件描述符集合存放要监测的文件描述符
       第三步：select来监测键盘有没有数据可读	   
*/ 

int main()
{
	int ret;
	int tcpsock;
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
	serveraddr.sin_addr.s_addr=inet_addr("192.168.131.130"); //服务器的ip 
	serveraddr.sin_port=htons(10000); //服务器的端口号
	
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
	
	//定义文件描述符集合 
	fd_set myset;
	FD_ZERO(&myset); //清空集合
	FD_SET(0,&myset); //把键盘添加到集合中
	
	while(1)
	{
		//使用多路复用监测键盘是否有数据可读
		printf("select阻塞当前进程，如果文件描述符不发生状态改变，会一直阻塞\n");
		ret=select(0+1,&myset,NULL,NULL,NULL);
		if(ret>0) //说明键盘有数据可读--》用户敲击了键盘，打了字
		{
			bzero(sbuf,100);
			//调用scanf读取键盘输入的内容
			scanf("%s",sbuf);
			//发送给服务器
			send(tcpsock,sbuf,strlen(sbuf),0);
		}
	}
	
	//关闭套接字
	close(tcpsock);
	return 0;
	
}