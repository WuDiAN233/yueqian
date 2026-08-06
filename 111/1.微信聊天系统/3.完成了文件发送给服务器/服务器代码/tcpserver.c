#include "list.h"

/*
	多个客户端连接同一个服务器
*/

struct singlelist *myhead;

//所有的客户端发送给服务器的信息，都是用这个任务函数来接收
void *recv_fun(void *arg)
{
	int ret;
	struct singlelist *p=(struct singlelist *)arg;
	char rbuf[2048];
	while(1)
	{
		bzero(rbuf,2048);
		/*
			接收客户端发送的信息,依据彭老师刚才分析的通信协议
			服务器收到的客户端信息应该有四种
			  聊天        chat@对方ip@对方端口@真实信息
			  发送文件    sendfile@对方ip@对方端口@文件大小@文件类型    文件真实内容
			  发送表情包  emoji@对方ip@对方端口@表情包文件大小  表情包数据
			  获取服务器上存放的客户端信息  getlist
		*/
		ret=recv(p->sock,rbuf,2048,0);
		if(ret==0) //说明客户端断开了
		{
			printf("客户端:%s  %hu断开连接了\n",p->ip,p->port);
			//从链表删除对应结点
			list_delete(p->ip,p->port,myhead);
			//退出当前线程
			pthread_exit(NULL);
		}
		//依据彭老师制定的通信规则判断接收到的信息类型
		if(strcmp(rbuf,"getlist")==0) //获取服务器上存放的客户端信息
		{
			//遍历链表，拼接字符串(ip和端口号)
			//提供参考拼接格式：张三ip@张三端口#李四ip@李四端口#王五的ip@王五的端口		
			struct singlelist *q=myhead;
			char allmsg[2048]={0};
			strcpy(allmsg,"getlist#"); //用来标识这个信息的类型
			while(q->next!=NULL)
			{
				q=q->next;  
				sprintf(allmsg+strlen(allmsg),"%s@%hu#",q->ip,q->port);
			}
			send(p->sock,allmsg,strlen(allmsg)-1,0);
		}
		else  //其他三种功能(聊天，发送文件，发送表情包)
		{
			//切割字符串
			char *p1=strtok(rbuf,"@");
			//判断p1就能够知道究竟是三种功能的哪一种
			if(strcmp(p1,"chat")==0) //说明是聊天
			{
				//继续切割得到对方的ip，对方的端口号，真实信息
				char *p2=strtok(NULL,"@"); //对方的ip
				char *p3=strtok(NULL,"@"); //对方的端口号
				//端口转成整数
				int curport=atoi(p3);
				char *p4=strtok(NULL,"@"); //真实的信息 
				//遍历链表找到对应的客户端，把真实信息发送过去
				struct singlelist *q=myhead;
				while(q->next!=NULL)
				{
					q=q->next;  
					//判断是不是我需要转发的那个客户端
					if(strcmp(q->ip,p2)==0 && q->port==curport)
						break;
				}
				char allmsg[2048]={0};
				sprintf(allmsg,"chat#消息来自%s:%hu这个客户端#%s",p->ip,p->port,p4);
				send(q->sock,allmsg,strlen(allmsg),0);
			}
			if(strcmp(p1,"sendfile")==0) //说明是发送文件
			{
				//继续切割得到对方的ip，对方的端口号，真实信息
				char *p2=strtok(NULL,"@"); //对方的ip
				char *p3=strtok(NULL,"@"); //对方的端口号
				//端口转成整数
				int curport=atoi(p3);
				char *p4=strtok(NULL,"@"); //文件大小 
				//文件大小转成整数
				int filesize=atoi(p4);
				char *p5=strtok(NULL,"@"); //文件类型
				printf("文件大小: %d  文件类型: %s\n",filesize,p5);
				
				//依据文件类型，新建文件
				char newfilepath[100]={0};
				sprintf(newfilepath,"./new%s",p5);
				int newfd;
				newfd=open(newfilepath,O_CREAT|O_TRUNC|O_RDWR,0777);
				if(newfd==-1)
				{
					perror("新建文件失败了!\n");
					return NULL;
				}
				//循环调用recv(按照文件大小接收文件内容)
				while(1)
				{
					bzero(rbuf,2048);
					ret=recv(p->sock,rbuf,2048,0);
					filesize=filesize-ret; //剩余字节数
					//写入目标文件
					write(newfd,rbuf,ret);	
					if(filesize<=0) //按照文件大小接收完毕
						break;
				}
				close(newfd);
				//遍历链表找到对应的客户端，把文件内容发送过去
			}
			if(strcmp(p1,"emoji")==0) //说明是发送表情包
			{
				//循环调用recv(按照表情包文件大小接收文件内容)
				//遍历链表找到对应的客户端，把文件内容发送过去
			}
		}
		
	}
}

//服务器发送信息给客户端
void *send_fun(void *arg)
{
	char ipbuf[20];
	char sbuf[100];
	unsigned short portnum; 
	while(1)
	{
		bzero(ipbuf,20);
		printf("目前连接服务器成功的客户端信息如下!,请您输入客户端ip和端口号选择跟他聊天!\n");
		struct singlelist *p=myhead;
		while(p->next!=NULL)
		{
			p=p->next;
			printf("%s  %hu\n",p->ip,p->port);
		}
		scanf("%s",ipbuf);
		scanf("%hu",&portnum);
		printf("请输入要发送给他的信息！\n");
		scanf("%s",sbuf);
		//遍历链表找到正确的套接字
		p=myhead;
		while(p->next!=NULL)
		{
			p=p->next;
			if(strcmp(p->ip,ipbuf)==0 && p->port==portnum)
				break;
		}
		//发送给对应的客户端
		send(p->sock,sbuf,strlen(sbuf),0);
	}
}

int main()
{
	pthread_t id;
	pthread_t id1;
	int newsock; 
	int tcpsock;
	int ret;
	char rbuf[100];
	pthread_attr_t myattr;
	
	//定义ipv4地址结构体存放需要绑定的ip和端口号
	struct sockaddr_in bindaddr;
	bzero(&bindaddr,sizeof(bindaddr));
	bindaddr.sin_family=AF_INET;
	bindaddr.sin_addr.s_addr=htonl(INADDR_ANY); //绑定服务器自己的ip地址
	bindaddr.sin_port=htons(10000);  //绑定服务器的端口号
	
	//定义ipv4地址结构体存放连接成功的客户端ip和端口号
	struct sockaddr_in clientaddr;
	bzero(&clientaddr,sizeof(clientaddr));
	int size=sizeof(clientaddr);
	
	//初始化链表
	myhead=list_init();
	
	//初始化线程属性
	pthread_attr_init(&myattr);
	
	//设置线程分离属性
	pthread_attr_setdetachstate(&myattr,PTHREAD_CREATE_DETACHED);
	
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
	
	//监听
	ret=listen(tcpsock,10);
	if(ret==-1)
	{
		perror("监听失败了!\n");
		return -1;
	}
	
	//创建一个线程专门键盘输入发送信息给任意一个客户端
	pthread_create(&id1,NULL,send_fun,NULL);
	
	
	//由于该代码要求：服务器可以接收很多个客户端的连接请求，所以accept应该要反复调用
	while(1)
	{
		//接收客户端的连接请求
		newsock=accept(tcpsock,(struct sockaddr *)&clientaddr,&size);
		if(newsock==-1)
		{
			perror("接收客户端的连接请求失败!\n");
			return -1;
		}
		//准备新的结点
		struct singlelist *newnode=malloc(sizeof(struct singlelist)); 
		newnode->sock=newsock;     //存放套接字
		strcpy(newnode->ip,inet_ntoa(clientaddr.sin_addr)); //存放ip地址
		newnode->port=ntohs(clientaddr.sin_port);     //存放端口号
		newnode->next=NULL;    //指针域
	
		//尾插客户端信息到链表中 
		list_insert_tail(newnode,myhead);
		
		//立马创建一个线程(可分离)接收这个客户端发送过来的信息
		pthread_create(&id,&myattr,recv_fun,newnode);
	}
	
	//关闭套接字
	close(tcpsock);
	close(newsock);
	return 0;
	
}