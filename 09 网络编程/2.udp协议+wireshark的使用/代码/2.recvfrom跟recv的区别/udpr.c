#include "myhead.h"

/*
    recvfrom跟recv的区别
	   1. recv对方断开连接--》不阻塞，返回0
	      recvfrom对方退出--》依然阻塞，不会返回0，recvfrom返回值代表收到的字节数
*/ 

int main()
{
	int ret;
	int udpsock;
	char rbuf[100];
	//定义ipv4地址结构体变量存放需要绑定的ip和端口号
	struct sockaddr_in bindaddr;
	bzero(&bindaddr,sizeof(bindaddr));
	bindaddr.sin_family=AF_INET; //地址协议类型，ipv4
	bindaddr.sin_addr.s_addr=htonl(INADDR_ANY);
	bindaddr.sin_port=htons(10000); //程序员自己指定1024到65535之间的某个端口号
	
	
	//定义ipv4地址结构体变量存放对方的ip和端口号
	struct sockaddr_in otheraddr;
	bzero(&otheraddr,sizeof(otheraddr));
	int size=sizeof(otheraddr);
	
	//创建udp套接字
	udpsock=socket(AF_INET,SOCK_DGRAM,0);
	if(udpsock==-1)
	{
		perror("创建udp套接字失败了\n");
		return -1;
	}
	
	//设置端口号复用(程序退出之后，虽然端口号被占用，但是立马运行程序，依然可以使用之前的端口号)
	int on=1; //非零,相当于是开关，开启设置端口号复用
	setsockopt(udpsock,SOL_SOCKET,SO_REUSEADDR,&on,sizeof(on));
	
	//绑定ip和端口号
	ret=bind(udpsock,(struct sockaddr *)&bindaddr,sizeof(bindaddr));
	if(ret==-1)
	{
		perror("绑定ip和端口号失败了\n");
		return -1;
	}
	
	while(1)
	{
		bzero(rbuf,100);
		//接收对方发送过来的信息
		/*
			recvfrom前面四个参数跟recv一模一样
			第五个参数：不需要程序员赋值，自动存放对方的ip和端口号
			      自动存放对方的ip和端口号
				  跟tcp中的accept函数第二个参数类似
				  对方：指的就是给你发送信息的那个主机
			
		*/
		ret=recvfrom(udpsock,rbuf,100,0,(struct sockaddr *)&otheraddr,&size);
		printf("对方发送过来，我成功接收的字节数: %d\n",ret);
		printf("给你发送信息的那台主机ip地址%s,端口号:%hu，发过来的信息:%s\n",inet_ntoa(otheraddr.sin_addr),ntohs(otheraddr.sin_port),rbuf);
	}
	
	//关闭套接字
	close(udpsock);
	return 0;
}