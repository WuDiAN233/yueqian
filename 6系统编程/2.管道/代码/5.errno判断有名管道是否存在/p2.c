#include "myhead.h"

/*
	p2接收p1发送过来的信息
	p1跟p2是没有任何血缘关系的进程
	  
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
	
	//p2接收管道中的信息
	while(1)
	{
		bzero(buf,100);
		//从有名管道中读取数据
		read(fd,buf,100);
		//判断是不是"quit"
		if(strcmp(buf,"quit")==0)
			break;
		printf("p1给我发送过来的信息是: %s\n",buf);
	}
	
	//关闭有名管道
	close(fd);
	return 0;
	
}