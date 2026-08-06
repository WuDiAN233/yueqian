#include "myhead.h"

/*
    1.服务器：创建一个链表，存放当前连接成功的客户端信息
    2.复盘一下
	    (1)单链表的操作是否熟
		     头节点初始化
			 尾插新的节点
		(2)按图索骥--》照葫芦画瓢
		(3)创建线程，给线程传递参数
*/ 
//定义结构体存放客户端信息
struct clientlist
{
	//数据域
	char ip[20]; //存放客户端ip地址
	unsigned short port; //存客户端端口号
	int sock; //存放客户端套接字
	//拓展添加你认为有用的其它信息
	//指针域
	struct clientlist *next;
};

//定义全局变量存放链表的头节点
struct clientlist *myhead;

//初始化头节点：不存放任何有效数据
struct clientlist *list_init()
{
	struct clientlist *head=malloc(sizeof(struct clientlist));
	head->next=NULL;
	return head;
}

//尾插新的节点: 把node这个节点插入到head代表的单链表的尾部
int insert_tail(struct clientlist *node,struct clientlist *head)
{
	struct clientlist *p=head;
	//找到链表的尾部
	while(p->next!=NULL)
		p=p->next;
	
	//尾插新的节点
	p->next=node;
	return 0;
}

//专门用来接收客户端发送过来的信息
void *myrecv_msg(void *arg)
{
	printf("服务器现在创建了一个线程，该线程专门接收某个客户端发送过来的信息，线程的ID：%ld\n",pthread_self());
	char rbuf[100];
	int ret;
	//正确的套接字，arg就有
	struct clientlist *p=(struct clientlist *)arg;
	while(1)
	{
		bzero(rbuf,100);
		ret=recv(p->sock,rbuf,100,0);
		if(ret==0) //说明客户端断开连接了
		{
			//把这个客户端从链表中删除
			
			//退出该线程
			pthread_exit(NULL);
		}
		printf("客户端ip是:%s，端口号是:%hu给我发送的信息是:%s\n",p->ip,p->port,rbuf);
	}
}

void *send_msg_client(void *arg)
{
	char someip[20];
	unsigned short someport;
	char sbuf[100];
	
	while(1)
	{
		bzero(sbuf,100);
		bzero(someip,20);
		printf("请输入客户端ip和端口号\n");
		scanf("%s",someip);
		scanf("%hu",&someport);
		printf("请输入要发送给这个客户端的信息\n");
		scanf("%s",sbuf);
		
		struct clientlist *p=myhead; //遍历链表，找到要聊天的那个客户端
		while(p->next!=NULL)
		{
			p=p->next;
			//判断当前p这个节点是不是你要聊天的那个客户端信息
			if(strcmp(p->ip,someip)==0 && p->port==someport)
			{
				send(p->sock,sbuf,strlen(sbuf),0);
			}	
		}	
	}
}

int main()
{
	int ret;
	int newsock; //存放accept返回的新套接字
	int tcpsock; //旧的套接字
	pthread_t id;
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
	
	//初始化链表的头节点
	myhead=list_init();
	
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
	
	//创建一个线程：服务器专门键盘输入，发送信息给指定客户端
	pthread_create(&id,NULL,send_msg_client,NULL);
	
	//死循环里面不断地接收客户端连接请求
	while(1)
	{
		//愿意接收客户端的连接请求(重点)
		printf("服务器阻塞在accept的位置\n");
		newsock=accept(tcpsock,(struct sockaddr *)&clientaddr,&size);
		if(newsock==-1)
		{
			perror("服务器接收客户端连接请求失败了\n");
			return -1;
		}
		
		//新建一个节点，存放ip，端口号，套接字
		struct clientlist *newnode=malloc(sizeof(struct clientlist));
		strcpy(newnode->ip,inet_ntoa(clientaddr.sin_addr));
		newnode->port=ntohs(clientaddr.sin_port);
		newnode->sock=newsock;
		
		//尾插到链表中
		insert_tail(newnode,myhead);
		
		//立马新建一个线程，该线程专门接收目前连接成功的客户端发送过来的信息
		pthread_create(&id,NULL,myrecv_msg,newnode);
	}

	//关闭套接字
	close(tcpsock);
	close(newsock);
	return 0;
}