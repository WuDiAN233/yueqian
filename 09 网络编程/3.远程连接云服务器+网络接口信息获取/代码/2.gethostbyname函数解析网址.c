#include "myhead.h"

/*
	把你指定的网址：解析成对应的服务器ip
	得到网址服务器的ip地址，后面就能使用tcp协议去连接这个服务器，给这个服务器发送http请求
*/

int main(int argc,char **argv)
{
	//判断参数个数
	if(argc<2)
	{
		printf("对不起，你忘记传递网址了,./程序名  某个网址\n");
		return -1;
	}
	
	struct hostent *myhost=gethostbyname(argv[1]);
	//打印结构体中的成员
	printf("你这个网址，官方名字叫做: %s\n",myhost->h_name);
	printf("你这个网址，别名叫做: %s\n",*(myhost->h_aliases));
	
	//打印解析出来的ip地址
	//错误的写法：h_addr_list存放的大端序格式的ip地址，无法直接打印
	//printf("你这个网址，服务器的ip地址是: %s\n",*(myhost->h_addr_list));
	
	//解决方法： inet_ntoa(struct in_addr in)这个函数把大端序ip转成我们需要的小端序ip，才能正常打印出服务器的ip地址
	char *ipstr=*(myhost->h_addr_list);
	//第一步:把char *强制类型转换成struct in_addr *
	struct in_addr *q=(struct in_addr *)ipstr;
	//第二步：把q解引用不就搞定了吗
	printf("你这个网址，服务器的ip地址是: %s\n",inet_ntoa(*q));
	
}