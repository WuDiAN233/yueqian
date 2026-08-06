#include "myhead.h"
#include "cJSON.h"

/*
    get请求获取物流信息
	   https://wuliu.market.alicloudapi.com/kdi
*/ 

int main()
{
	int ret;
	int tcpsock;
	char rbuf[10000]={0}; //存放服务器回复给我的应答信息
	//定义数组存放解析得到的物流信息网址服务器ip
	char ipstr[20]={0};
	
	//定义ipv4地址结构体变量存放需要绑定的ip和端口号
	struct sockaddr_in bindaddr;
	bzero(&bindaddr,sizeof(bindaddr));
	bindaddr.sin_family=AF_INET; //地址协议类型，ipv4
	bindaddr.sin_addr.s_addr=htonl(INADDR_ANY); 
	bindaddr.sin_port=htons(10086); //程序员自己指定1024到65535之间的某个端口号
	
	//调用gethostbyname函数获取天气预报网址服务器的ip地址
	//https://要去掉   .com后面的路径名去掉
	struct hostent *myhost=gethostbyname("wuliu.market.alicloudapi.com");
	char *str=*(myhost->h_addr_list);
	//第一步:把char *强制类型转换成struct in_addr *
	struct in_addr *q=(struct in_addr *)str;
	//第二步：把q解引用不就搞定了吗
	strcpy(ipstr,inet_ntoa(*q));
	printf("获取物流信息网址服务器的ip地址是: %s\n",ipstr);
		
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
	
	//连接我领取的物流信息网址服务器
	ret=connect(tcpsock,(struct sockaddr *)&serveraddr,sizeof(serveraddr));
	if(ret==-1)
	{
		perror("连接服务器失败了\n");
		return -1;
	}
	
	//给服务器发送get请求，得到物流信息
	char *getreq="GET /kdi?no=465562122253261 HTTP/1.1\r\n"
	              "Host:wuliu.market.alicloudapi.com\r\n"
				  "Authorization:APPCODE 6cf57230cb594ccdb7c4a9bf64c137b1\r\n\r\n";
	
	//发送get请求
	send(tcpsock,getreq,strlen(getreq),0);
	
	//立马读取服务器回复的应答信息
	recv(tcpsock,rbuf,10000,0);

	//把rbuf中真实的json数据截取出来
	//从头到尾查找"{"
	char *start=strstr(rbuf,"{");
	if(start==NULL)
	{
		printf("没有找到{\n");
		return -1;
	}
	//从尾到头查找"}"
	char *end=strrchr(rbuf,'}');
	if(end==NULL)
	{
		printf("没有找到}\n");
		return -1;
	}

	
	//从start开始到end结束，整个数据都是真实的json数据
	char otherbuf[10000]={0};
	strncpy(otherbuf,start,end-start+1);
	//验证一下：是否把正确的json数据截取出来
	//printf("真实的json数据是: %s\n",otherbuf);
	
	//把真实的json数据转换
	cJSON *myjson=cJSON_Parse(otherbuf);
	
	//获取对象中的键值对
	cJSON *val1=cJSON_GetObjectItem(myjson,"status");
	printf("status对应的值: %s\n",val1->valuestring);
	
	cJSON *val2=cJSON_GetObjectItem(myjson,"result");
	//观察发现val2是对象，解析继续
	cJSON *val3=cJSON_GetObjectItem(val2,"list");
	//观察发现val3是数组，解析继续
	cJSON *val4=cJSON_GetArrayItem(val3,0);
	//观察发现val4是对象，解析继续
	cJSON *val5=cJSON_GetObjectItem(val4,"time");
	printf("配送时间: %s\n",val5->valuestring);
	cJSON *val6=cJSON_GetObjectItem(val4,"status");
	printf("物流状态: %s\n",val6->valuestring);
	
	//关闭套接字
	close(tcpsock);
	return 0;
	
}