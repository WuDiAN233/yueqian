#include "myhead.h"  //我自定义的头文件，里面包含了所有常用的头文件

/*
	多个客户端连接服务器
*/   

//定义结构体存放连接成功的客户端信息
struct clientmsg
{
	//数据域
	int sock; //目前连接成功客户端套接字
	char ip[20]; //客户端ip地址
	unsigned short port; //客户端端口号
	
	//指针域
	struct clientmsg *next;
};

struct clientmsg *myhead; //全局变量
int linkflag=0; //如果有新的客户端连接就改变这个标志位

//初始化链表的头结点
struct clientmsg *client_init()
{
	struct clientmsg *head=malloc(sizeof(struct clientmsg));
	head->next=NULL;
	return head;
}

//尾插某个节点到链表的尾部
int insert_tail(struct clientmsg *node,struct clientmsg *head)
{
	//找到链表的尾部
	struct clientmsg *p=head;
	while(p->next!=NULL)
		p=p->next;
	
	//把节点插入到尾部
	p->next=node;
	return 0;
}

//删除节点
int list_delete(char ipbuf[20],unsigned short delport,struct clientmsg *head)
{
	//定义两个指针,一前一后遍历链表
	struct clientmsg *p=head->next;
	struct clientmsg *q=head;
	
	while(p!=NULL)
	{
		if(strcmp(p->ip,ipbuf)==0 && p->port==delport)
			break;
		
		//p和q往后挪动
		p=p->next;
		q=q->next;
	}
	
	//删除p这个节点
	q->next=p->next;
	p->next=NULL;
	free(p);
}

//线程的任务函数：专门负责接收某个客户端发送过来的信息
void *recv_client_msg(void *arg)
{
	int ret;
	struct clientmsg *p=(struct clientmsg *)arg;
	char clientbuf[2048];
	char rbuf[2048];
	int msglen=0;
	while(1)
	{
		bzero(rbuf,2048);
		ret=recv(p->sock,&msglen,sizeof(msglen),0); //接收信息长度/大小
		//接收客户端的信息
		ret=recv(p->sock,rbuf,msglen,0);
		/*
			判断接收的信息,服务器收到的信息有四种类型(按照我们制定的通信协议)
			   1.getlist
			   2.聊天文字信息  msg@对方ip@对方端口@真实信息
			   3.文件信息      
			   4.表情包信息 
		*/
		printf("第一次recv的返回值是: %d\n",ret);
		
		if(strcmp(rbuf,"getlist")==0) //说明客户端要求服务器给它发送在线好友信息
		{
			bzero(clientbuf,2048);
			//遍历链表，拼接字符串把所有在线的客户端信息发给对应的客户端
			//拼接格式： 张三ip:张三端口号@李四的ip:李四端口号
			struct clientmsg *temp=myhead;
			while(temp->next!=NULL)
			{
				temp=temp->next;
				//拼接字符串 
				sprintf(clientbuf+strlen(clientbuf),"%s:%hu@",temp->ip,temp->port);
			}
			//发送所有在线的客户端信息给对应的客户端
			send(p->sock,clientbuf,strlen(clientbuf),0);
			
		}
		else  //其他三种类型的信息(文字信息  文件信息  表情包信息)
		{
			//按照协议分类处理三种信息
			char *p1=strtok(rbuf,"@");
			if(strcmp(p1,"msg")==0) //说明客户端发过来是文字聊天信息
			{
				//继续切割
				char *p2=strtok(NULL,"@"); //对方ip 
				char *p3=strtok(NULL,"@"); //对方端口
				char *p4=strtok(NULL,"@"); //真实的信息
				//遍历链表，找到跟p2，p3对应的客户端，然后把信息转发出去即可
				struct clientmsg *temp=myhead;
				while(temp->next!=NULL)
				{
					temp=temp->next;
					if(strcmp(temp->ip,p2)==0 && temp->port==atoi(p3))
					{
						bzero(clientbuf,2048);
						//拼接字符串把信息是谁发的也拼接上去
						sprintf(clientbuf,"%s:%hu发送的信息:%s",p->ip,p->port,p4);
						send(temp->sock,clientbuf,strlen(clientbuf),0);
						break;
					}				
				}
				
			}
			if(strcmp(p1,"file")==0) //说明客户端发过来是文件
			{
				
			}
			if(strcmp(p1,"emoji")==0) //说明客户端发过来是表情包
			{
				//继续切割
				char *p2=strtok(NULL,"@"); //对方ip 
				char *p3=strtok(NULL,"@"); //对方端口
				char *p4=strtok(NULL,"@"); //表情包大小
				long size=atoi(p4);
				//接收客户端接下来发送的表情包数据
				char buf[size];
				
				/*
					循环接收表情包数据
					原因：实际recv接收表情包、接收文件有可能一下子无法接收你指定的字节数，因此需要用到循环接收
				*/
				ret=0;
				long remainingbytes=size; //存放剩余要接收的字节数
				char *p5=buf; //指向数组的起始位置
				//循环接收客户端发送过来的表情包数据
				while(1)
				{
					ret=recv(p->sock,p5,500,0); 
					//更新剩余字节数
					remainingbytes=remainingbytes-ret;
					//更新p5
					p5=p5+ret;
					printf("接收到表情包数据字节数是: %d\n",ret);
					//退出条件
					if(remainingbytes<=0) //剩余要接收的字节数
						break;
				}
				
				printf("服务器表情包文件接收完毕！\n");
				//新建空白的jpeg文件，保存表情包数据
				int fd;
				fd=open("./new.jpeg",O_CREAT|O_TRUNC|O_RDWR,0777);
				if(fd==-1)
				{
					perror("新建空白的jpeg文件失败了!\n");
					return NULL;
				}
				//写入到新建的文件中
				write(fd,buf,size);
				close(fd);
			}	
		}
			
		if(ret==0)
		{
			printf("客户端ip %s  端口号%hu断开了\n",p->ip,p->port);
			//从单链表中删除这个断开的客户端
			list_delete(p->ip,p->port,myhead);
			//退出当前线程
			pthread_exit(NULL);
		}
	}
}

void *send_msgto_client(void *arg)
{
	char ipbuf[20];
	char sbuf[100];
	unsigned short someport;
	while(1)
	{
		bzero(ipbuf,20);
		bzero(sbuf,100);
		printf("请输入你要跟哪个客户端聊天，输入这个客户端ip和端口号!\n");
		scanf("%s",ipbuf);
		scanf("%hu",&someport);
		printf("请输入要给这个客户端发生的信息!\n");
		scanf("%s",sbuf);
		//把这个信息发送给对应的客户端
		//遍历链表找到你选择的那个客户端
		struct clientmsg *p=myhead;
		while(p->next!=NULL)
		{
			p=p->next;
			if(strcmp(p->ip,ipbuf)==0 && p->port==someport)
				break;
		}
		send(p->sock,sbuf,strlen(sbuf),0);
	}
}

//线程任务函数：只要有客户端连接，都自动遍历链表打印
void *show_client(void *arg)
{
	while(1)
	{
		if(linkflag==1)
		{
			system("clear"); //清屏
			//遍历当前这个链表，把目前在线的客户端信息打印一遍，方便我选择究竟要跟谁聊天
			struct clientmsg *p=myhead;
			while(p->next!=NULL)
			{
				p=p->next;
				printf("目前在线的客户端%s  %hu\n",p->ip,p->port);
			}
			linkflag=0;
		}	
	}
}
			
int main()
{
	int ret;
	pthread_t id;
	pthread_t otherid;
	int tcpsock;
	int newsock; 
	char rbuf[100];
	//定义ipv4地址结构体变量存放客户端需要绑定的ip和端口号
	struct sockaddr_in bindaddr;
	bzero(&bindaddr,sizeof(bindaddr));
	bindaddr.sin_family=AF_INET;  //地址协议，IPV4
	bindaddr.sin_addr.s_addr=inet_addr("192.168.11.4");    //绑定服务器自己的ip地址
	bindaddr.sin_port=htons(30000);  //服务器要绑定的端口号
	
	//定义ipv4地址结构体变量保存连接成功的那个客户端信息
	struct sockaddr_in clientaddr;
	int size=sizeof(clientaddr);
	
	//定义属性变量并初始化
	pthread_attr_t myattr;
	pthread_attr_init(&myattr);
	
	//调用设置分离属性的函数
	pthread_attr_setdetachstate(&myattr,PTHREAD_CREATE_DETACHED); //可分离
	
	//初始化链表的头结点
	myhead=client_init();
	
	//创建tcp套接字--》买手机
	tcpsock=socket(AF_INET,SOCK_STREAM,0);
	if(tcpsock==-1)
	{
		perror("买手机失败(创建tcp套接字)失败!\n");
		return -1;
	}
	
	//设置端口复用
	int on=1; //非0即可,是个开关，开启端口复用功能
	setsockopt(tcpsock,SOL_SOCKET,SO_REUSEADDR,&on,sizeof(on));
	
	//绑定ip地址和端口号
	ret=bind(tcpsock,(struct sockaddr *)&bindaddr,sizeof(bindaddr));
	if(ret==-1)
	{
		perror("绑定ip和端口号失败了!\n");
		return -1;
	}
	
	//监听
	ret=listen(tcpsock,5);
	if(ret==-1)
	{
		perror("监听失败了!\n");
		return -1;
	}
	
	//创建一个线程，专门负责遍历打印在线客户端(如果有新的客户端连接就打印)
	pthread_create(&otherid,NULL,show_client,NULL);
	
	//创建一个线程：从键盘输入内容发送给对应的客户端
	pthread_create(&otherid,NULL,send_msgto_client,NULL);
	
	while(1)
	{
		//接收客户端的连接请求，产生新的套接字
		newsock=accept(tcpsock,(struct sockaddr *)&clientaddr,&size);
		//printf("目前连接成功的那个客户端ip是: %s  端口号是: %hu\n",inet_ntoa(clientaddr.sin_addr),ntohs(clientaddr.sin_port));
	
		//准备新的节点
		struct clientmsg *newnode=malloc(sizeof(struct clientmsg));
		newnode->sock=newsock;  //存放套接字
		strcpy(newnode->ip,inet_ntoa(clientaddr.sin_addr)); //存放ip地址
		newnode->port=ntohs(clientaddr.sin_port); //存放端口号
		newnode->next=NULL;
		
		//客户端的信息尾插到链表
		insert_tail(newnode,myhead);
		
		//标志位修改
		linkflag=1;
		
		//立马新建一个线程(分离的)，该线程专门负责接收这个客户端发送过来的信息
		pthread_create(&id,&myattr,recv_client_msg,newnode);
	}
	//挂机
	close(tcpsock);
	close(newsock);
	return 0;
}