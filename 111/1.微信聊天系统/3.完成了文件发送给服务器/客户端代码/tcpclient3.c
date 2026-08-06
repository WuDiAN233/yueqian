#include "myhead.h"

/*
	多个客户端连接同一个服务器	  
*/
int tcpsock;
	
void *recv_fun(void *arg)
{
	int ret;
	char rbuf[2048];
	while(1)
	{
		bzero(rbuf,2048);
		ret=recv(tcpsock,rbuf,2048,0);
		if(ret==0) //服务器断开了
		{
			exit(-1); //结束整个进程
		}
		//切割字符串，判断服务器给客户端究竟发的是什么信息
		/*
			聊天信息：  chat#真实信息
			文件信息:   sendfile#文件类型#文件大小  文件内容
			表情包：    emoji#表情包大小  表情包内容
			所有在线客户端信息  getlist#在线客户端信息
		*/
		char *p1=strtok(rbuf,"#");
		if(strcmp(p1,"getlist")==0) //说明服务器给客户端发过来的是好友列表
		{
			char *p2;
			printf("所有在线的好友信息如下:\n");
			while((p2=strtok(NULL,"#"))!=NULL)
			{
				printf("%s\n",p2);
			}
		}
		if(strcmp(p1,"chat")==0)  //说明服务器给客户端发过来的是聊天信息
		{
			//继续切割得到真实信息
			char *p2=strtok(NULL,"#"); //对方的ip和端口号
			char *p3=strtok(NULL,"#"); //真实的信息
			printf("%s发给我的信息: %s\n",p2,p3);
		}
	}
}
int main()
{
	pthread_t id;
	int ret;
	char sbuf[100];
	char allmsg[2048];
	char ipbuf[20];
	unsigned short portnum;
	int n;
	
	//定义ipv4地址结构体存放需要绑定的ip和端口号
	struct sockaddr_in bindaddr;
	bzero(&bindaddr,sizeof(bindaddr));
	bindaddr.sin_family=AF_INET;
	bindaddr.sin_addr.s_addr=htonl(INADDR_ANY); //绑定客户端自己的ip地址
	bindaddr.sin_port=htons(10088);  //绑定客户端的端口号
	
	//定义ipv4地址结构体存放服务器的ip和端口号
	struct sockaddr_in serveraddr;
	bzero(&serveraddr,sizeof(serveraddr));
	serveraddr.sin_family=AF_INET;
	serveraddr.sin_addr.s_addr=inet_addr("192.168.72.3"); //服务器的ip地址
	serveraddr.sin_port=htons(10000);  //服务器的端口号
	
	
	//创建tcp套接字
	tcpsock=socket(AF_INET,SOCK_STREAM,0);
	if(tcpsock==-1)
	{
		perror("创建tcp套接字失败了!\n");
		return -1;
	}
	
	//设置取消端口绑定限制
	int on=1;  //开关
	setsockopt(tcpsock,SOL_SOCKET,SO_REUSEADDR,&on,sizeof(on));
	
	//绑定ip和端口号
	ret=bind(tcpsock,(struct sockaddr *)&bindaddr,sizeof(struct sockaddr_in));
	if(ret==-1)
	{
		perror("绑定ip和端口号失败了!\n");
		return -1;
	}
	
	//连接服务器
	ret=connect(tcpsock,(struct sockaddr *)&serveraddr,sizeof(struct sockaddr_in));
	if(ret==-1)
	{
		perror("连接服务器失败!\n");
		return -1;
	}
	
	//创建线程接收信息
	pthread_create(&id,NULL,recv_fun,NULL);
	
	//发送信息给服务器
	while(1)
	{
		printf("请选择你想要的功能!\n");
		printf("1.跟其他客户端聊天!\n");
		printf("2.发送文件给其他客户端!\n");
		printf("3.发送表情包给其他客户端!\n");
		printf("4.获取服务器上存放的在线客户端信息!\n");
		scanf("%d",&n);
		switch(n)
		{
			case 1:  //聊天 
				bzero(ipbuf,20);
				bzero(sbuf,100);
				bzero(allmsg,2048);
				printf("请输入对方的ip!\n");
				scanf("%s",ipbuf);
				printf("请输入对方的端口号！\n");
				scanf("%hu",&portnum);
				printf("请输入你要发给对方的信息!\n");
				scanf("%s",sbuf);
				//按照制定好的协议拼接字符串
				sprintf(allmsg,"chat@%s@%hu@%s",ipbuf,portnum,sbuf);
				//发送给服务器
				send(tcpsock,allmsg,strlen(allmsg),0);
				break;
			case 2:  //发送文件
				bzero(ipbuf,20);
				bzero(sbuf,100);
				bzero(allmsg,2048);
				printf("请输入对方的ip!\n");
				scanf("%s",ipbuf);
				printf("请输入对方的端口号！\n");
				scanf("%hu",&portnum);
				printf("请输入你要发给对方的文件路径名!\n");
				scanf("%s",sbuf);
				//求出文件大小
				struct stat mystat;
				stat(sbuf,&mystat);
				//获取文件后缀名
				char *p1=strstr(sbuf,".");
				
				//按照制定好的协议(sendfile@对方ip@对方端口@文件大小@文件类型)拼接字符串
				sprintf(allmsg,"sendfile@%s@%hu@%ld@%s",ipbuf,portnum,mystat.st_size,p1);
				//先发送文件基本信息发送给服务器
				send(tcpsock,allmsg,strlen(allmsg),0);
				
				int fd;
				fd=open(sbuf,O_RDWR);
				if(fd==-1)
				{
					perror("打开文件失败了!\n");
					return -1;
				}
				//循环读取文件内容，再发文件内容
				while(1)
				{
					bzero(allmsg,2048);
					ret=read(fd,allmsg,2048);
					if(ret==0)
						break;
					//发送给服务器
					send(tcpsock,allmsg,ret,0);
				}
				close(fd);
				break;
			case 3:
				break;
			case 4:
				send(tcpsock,"getlist",7,0);
				break;
		}
	}
	
	//关闭套接字
	close(tcpsock);
	return 0;
	
}