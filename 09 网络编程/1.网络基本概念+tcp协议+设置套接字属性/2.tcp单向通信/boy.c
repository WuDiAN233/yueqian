#include "myhead.h"

/*
    客户端代码
	为了演示方便，客户端和服务器都在教师机上运行
	单向通信：客户端键盘输入信息发送给服务器
	1.绑定ip总结三种写法
	2.接着流程写
	     connect需要注意的问题
		     (1)第二个参数代表服务器的ip和端口号
			    网络编程中，只要参数是地址结构体变量，都是用来存放ip和端口号的
		     (2)服务器是谁啊，服务器的ip和端口号怎么填写啊？
			    例子1：假设彭老师演示代码的时候，客户端和服务器都在教师机上运行(演示现象方便，就跟前面学习的有名管道类似，同一台主机内部进程间通信)
				       此时：服务器的ip地址跟客户端ip一模一样，都是教师机的ubuntu的ip地址
					         服务器的端口，由程序员自己指定(客户端连接服务器，跟服务器自己绑定的端口号要求一致)
			    例子2：假设彭老师的ubuntu运行客户端代码
				           张同学的ubuntu充当服务器，运行服务器代码
					   服务器的ip地址，填写张同学ubuntu的ip地址
					   服务器的端口，由程序员自己指定(客户端连接服务器，跟服务器自己绑定的端口号要求一致)
				例子3：假设张同学的ubuntu运行客户端代码
				           彭老师ubuntu充当服务器，运行服务器代码
					   服务器的ip地址，填写彭老师教师机ubuntu的ip地址
					   服务器的端口，由程序员自己指定(客户端连接服务器，跟服务器自己绑定的端口号要求一致)
				htonl(INADDR_ANY) 只能配合bind函数使用，其它场合只能使用inet_addr或者inet_aton     
	3.发送接收网络中的信息
      两组函数都可以正常使用
          send和write发送
          recv和read接收		  
			    
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
	//逻辑不正确：ubuntu采用小端序存放ip地址
	//但是：计算机网络要求tcp协议采用大端序存放ip地址
	//bindaddr.sin_addr.s_addr="192.168.213.130";
	//正确的写法：linux提供专门的函数把小端序ip地址转成大端序存放
	//写法1：指定了本地ubuntu的ip地址
	//bindaddr.sin_addr.s_addr=inet_addr("192.168.213.130"); //绑定自己ubuntu的ip地址
	//写法2：指定了本地ubuntu的ip地址
	//inet_aton("192.168.213.130",&(bindaddr.sin_addr));     //绑定自己ubuntu的ip地址
	//写法3：任何同学的电脑上都能用 
	bindaddr.sin_addr.s_addr=htonl(INADDR_ANY);
	
	//逻辑不正确：ubuntu采用小端序存放端口号
	//但是：计算机网络要求tcp协议采用大端序存放端口号
	//bindaddr.sin_port=10086; //我自己指定一个端口号
	//正确的写法：linux提供专门的函数把小端序端口号转成大端序存放
	bindaddr.sin_port=htons(10086); //程序员自己指定1024到65535之间的某个端口号
	
	
	//定义ipv4地址结构体变量存放服务器的ip和端口号
	struct sockaddr_in serveraddr;
	bzero(&serveraddr,sizeof(serveraddr));
	serveraddr.sin_family=AF_INET; 
	//写法1：指定服务器的ip地址
	serveraddr.sin_addr.s_addr=inet_addr("192.168.213.130"); //服务器的ip 
	//写法2：指定服务器的ip地址
	//inet_aton("192.168.213.130",&(serveraddr.sin_addr));   //服务器的ip 
	serveradd.sin_port=htons(10000); //服务器的端口号
	
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
	
	//连接服务器:三个参数跟bind一模一样的类型
	ret=connect(tcpsock,(struct sockaddr *)&serveraddr,sizeof(serveraddr));
	if(ret==-1)
	{
		perror("连接服务器失败了\n");
		return -1;
	}
	
	while(1)
	{
		printf("请输入要发送给服务器的信息!\n");
		scanf("%s",sbuf);
		//发送信息给服务器
		send(tcpsock,sbuf,strlen(sbuf),0);
	}
	
	//关闭套接字
	close(tcpsock);
	return 0;
}