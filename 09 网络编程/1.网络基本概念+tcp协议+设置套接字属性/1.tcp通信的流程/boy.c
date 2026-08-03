#include "myhead.h"

/*
	讲解tcp通信的流程
	   1.学习某个网络协议，都是从流程开始
	     流程：指的这种协议写代码的固定思路，步骤
	   2.bind函数使用的时候，必须字节序转换
	       ubuntu(linux)属于小端序
		   计算机网络协议要求数据采用大端序存放
		   第一组：小端序ip--》转成大端序ip
		           主机字节序转成网络字节序 
		     int inet_aton(const char *cp, struct in_addr *inp);
             in_addr_t inet_addr(const char *cp);
			      返回值：转换得到的大端序ip地址
                   参数：小端序字符串格式的ip的地址
		   第二组：小端序端口号--》转成大端序端口号
		     uint16_t htons(uint16_t hostshort);
		  概念：
		      老外：小端序在计算机网络编程中--》叫做主机字节序
			        大端序在计算机网络编程中--》叫做网络字节序 
*/ 

int main()
{
	int ret;
	int tcpsock;
	//定义ipv4地址结构体变量存放需要绑定的ip和端口号
	struct sockaddr_in bindaddr;
	bzero(&bindaddr,sizeof(bindaddr));
	bindaddr.sin_family=AF_INET; //地址协议类型，ipv4
	//逻辑不正确：ubuntu采用小端序存放ip地址
	//但是：计算机网络要求tcp协议采用大端序存放ip地址
	//bindaddr.sin_addr.s_addr="192.168.3.3";
	//正确的写法：linux提供专门的函数把小端序ip地址转成大端序存放
	bindaddr.sin_addr.s_addr=inet_addr("192.168.3.3"); //绑定自己ubuntu的ip地址
	
	//逻辑不正确：ubuntu采用小端序存放端口号
	//但是：计算机网络要求tcp协议采用大端序存放端口号
	//bindaddr.sin_port=10086; //我自己指定一个端口号
	//正确的写法：linux提供专门的函数把小端序端口号转成大端序存放
	bindaddr.sin_port=htons(10086); //程序员自己指定1024到65535之间的某个端口号
	
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
	
}