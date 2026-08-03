#include "myhead.h"

/*
	p2 
*/

int main()
{
	int ret;
	int fd;
	int num;
	
	//判断有名管道是否存在
	if(access("/home/gec/fifo",F_OK)!=0) //有名管道不存在
	{
		//创建有名管道: 在家目录新建一个有名管道，管道文件的名字叫做fifo
		ret=mkfifo("/home/gec/fifo",0777);
		//错误示范：不要在共享文件夹中新建有名管道，会失败
		//ret=mkfifo("/mnt/hgfs/share/fifo",0777);
		if(ret==-1)
		{
			perror("创建有名管道失败了\n");
			return -1;
		}
	}
	
	
	//打开有名管道
	fd=open("/home/gec/fifo",O_RDWR);
	if(fd==-1)
	{
		perror("打开有名管道失败了\n");
		return -1;
	}
	
	//读取有名管道中的信息
	pid_t id;
	read(fd,&id,sizeof(id));
	
	while(1)
	{
		printf("请输入你要给进程1发送的信号的序号!\n");
		scanf("%d",&num);
		//kill函数发送信号给p1
		kill(id,num);
	}
	
	//关闭有名管道
	close(fd);
	return 0;
	
}