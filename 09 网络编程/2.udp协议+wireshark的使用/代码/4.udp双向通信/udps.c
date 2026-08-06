#include "myhead.h"

/*
    udp切换不同的人聊天	    
*/ 
int udpsock;
void *myrecv_msg(void *arg)
{
	char rbuf[100];
	struct sockaddr_in addr;
	int size=sizeof(addr);
	while(1)
	{
		bzero(rbuf,100);
		recvfrom(udpsock,rbuf,100,0,(struct sockaddr *)&addr,&size);
		printf("对方给我回复的信息是: %s\n",rbuf);
	}
}
int main()
{
	int ret;
	int flag=1; //标志位
	pthread_t id;
	char sbuf[100];
	char ip[20];
	unsigned short port;
	//定义ipv4地址结构体变量存放需要绑定的ip和端口号
	struct sockaddr_in bindaddr;
	bzero(&bindaddr,sizeof(bindaddr));
	bindaddr.sin_family=AF_INET; //地址协议类型，ipv4
	bindaddr.sin_addr.s_addr=htonl(INADDR_ANY);
	bindaddr.sin_port=htons(10086); //程序员自己指定1024到65535之间的某个端口号
	
	
	//定义ipv4地址结构体变量存放对方的ip和端口号
	struct sockaddr_in otheraddr;
	
	
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
	
	//创建线程接收
	pthread_create(&id,NULL,myrecv_msg,NULL);
	
	while(1)
	{
		bzero(sbuf,100);
		if(flag==1)
		{
			bzero(ip,20);
			bzero(&otheraddr,sizeof(otheraddr));
			printf("你想跟谁聊，你直接输入对方的ip地址，端口号就能聊\n");
			scanf("%s",ip);
			scanf("%hu",&port);
			otheraddr.sin_family=AF_INET; 
			//指定对方的ip地址
			otheraddr.sin_addr.s_addr=inet_addr(ip); //对方的ip，你要跟谁发送信息，这里就写谁的ip
			otheraddr.sin_port=htons(port); //对方的端口号
			flag=0;
		}
		
		printf("请输入要发送给对方的信息!\n");
		scanf("%s",sbuf);
		//发送信息给对方
		/*
			sendto前面四个参数跟send一模一样
			第五个参数：存放对方的ip和端口号
			     send(tcp套接字,sbuf,strlen(sbuf),0) tcp通信的时候双方建立连接了(双方都知道对方是谁)
				 sendto(必须写上对方的ip和端口号才可以)
		*/
		sendto(udpsock,sbuf,strlen(sbuf),0,(struct sockaddr *)&otheraddr,sizeof(otheraddr));
	}
	
	//关闭套接字
	close(udpsock);
	return 0;
}