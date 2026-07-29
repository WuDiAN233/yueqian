#include "myhead.h"

/*
	p1键盘输入信息发送给p2
	p1跟p2是没有任何血缘关系的进程
	   1.结合前面的知识
	     linux基础：文件有七种类型
		             管道 --》指的就是有名管道
		 无名管道压根就没有对应的管道文件生成，只有两个文件描述符而已
	   2.有名管道不可以在共享文件中新建(有名管道属于linux系统中特有一种文件类型，windows系统不支持)
	   3.有名管道只有一个文件描述符，这个文件描述符既可以用来读取信息，也可以用来写入信息到管道中
       4.有名管道不可以重复创建
	        方法1：文件IO学习过access()判断文件/文件夹是否存在
*/

int main()
{
	int ret;
	int fd;
	char buf[100];

	//创建有名管道: 在家目录新建一个有名管道，管道文件的名字叫做fifo
	ret=mkfifo("/home/gec/fifo",0777);
	//错误示范：不要在共享文件夹中新建有名管道，会失败
	//ret=mkfifo("/mnt/hgfs/share/fifo",0777);
	if(ret==-1)
	{
		//判断失败的具体原因，如果是管道存在我就直接打开
		if(errno==EEXIST) //管道存在
		{
			//打开有名管道
			fd=open("/home/gec/fifo",O_RDWR);
			if(fd==-1)
			{
				perror("打开有名管道失败了\n");
				return -1;
			}
		}
		else  //其它的错误类型
		{
			perror("创建有名管道失败了\n");
			return -1;
		}
		
	}
	
	//p1键盘输入发送信息给p2
	while(1)
	{
		bzero(buf,100);
		printf("请输入要发送给p2的信息!\n");
		scanf("%s",buf);
		//写入到有名管道
		write(fd,buf,strlen(buf));
		//判断是不是"quit"
		if(strcmp(buf,"quit")==0)
			break;
	}
	
	//关闭有名管道
	close(fd);
	return 0;
	
}