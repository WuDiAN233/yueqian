#include "myhead.h"

/*
     如果post、get请求字段中有中文
	 一般来说中文需要进行格式转换的
	 如果遇到中文请求失败--》你要想到使用我笔记网址把中文转换成URL编码
	                          https://config.net.cn/tools/UrlEncode.html
*/ 

int main()
{
	int ret;
	int tcpsock;
	char rbuf[2048]={0}; //存放服务器回复给我的应答信息
	//定义数组存放解析得到的天气预报网址服务器ip
	char ipstr[20]={0};
	
	//定义ipv4地址结构体变量存放需要绑定的ip和端口号
	struct sockaddr_in bindaddr;
	bzero(&bindaddr,sizeof(bindaddr));
	bindaddr.sin_family=AF_INET; //地址协议类型，ipv4
	bindaddr.sin_addr.s_addr=htonl(INADDR_ANY); 
	bindaddr.sin_port=htons(10086); //程序员自己指定1024到65535之间的某个端口号
	
	//调用gethostbyname函数获取天气预报网址服务器的ip地址
	//https://要去掉   .com后面的路径名去掉
	struct hostent *myhost=gethostbyname("jmlstqxxcx.market.alicloudapi.com");
	char *str=*(myhost->h_addr_list);
	//第一步:把char *强制类型转换成struct in_addr *
	struct in_addr *q=(struct in_addr *)str;
	//第二步：把q解引用不就搞定了吗
	strcpy(ipstr,inet_ntoa(*q));
	printf("获取天气预报网址服务器的ip地址是: %s\n",ipstr);
		
	//定义ipv4地址结构体变量存放服务器的ip和端口号
	struct sockaddr_in serveraddr;
	bzero(&serveraddr,sizeof(serveraddr));
	serveraddr.sin_family=AF_INET; 
	//指定服务器的ip地址
	serveraddr.sin_addr.s_addr=inet_addr(ipstr); //天气预报网址服务器的ip地址
	serveraddr.sin_port=htons(80); //http协议端口号固定就是80
	
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
	
	//连接我领取的天气预报网址服务器
	ret=connect(tcpsock,(struct sockaddr *)&serveraddr,sizeof(serveraddr));
	if(ret==-1)
	{
		perror("连接服务器失败了\n");
		return -1;
	}
	
	//给服务器发送post请求，得到天气预报信息
	char *postreq="POST /weather/query/history HTTP/1.1\r\n"
	              "Host:jmlstqxxcx.market.alicloudapi.com\r\n"
				  "Content-Type:application/x-www-form-urlencoded\r\n"
				  "Content-Length:27\r\n"
				  "Authorization:APPCODE 6cf57230cb594ccdb7c4a9bf64c137b1\r\n\r\n"
				  "city=广州&date=2026-08-03";
	
	//发送post请求
	send(tcpsock,postreq,strlen(postreq),0);
	
	//立马读取服务器回复的应答信息
	recv(tcpsock,rbuf,2048,0);
	printf("杭州安那其这家公司给我回复的天气信息如下: %s\n",rbuf);
	
	//关闭套接字
	close(tcpsock);
	return 0;
	
}