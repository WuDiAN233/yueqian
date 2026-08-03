#include "myhead.h"

/*
	p1通过有名管道把id发送给p2，p2控制p1
*/

int main()
{
	int ret;
	int fd;
	
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

	//获取p1的ID
	pid_t id=getpid();
	//发送给p2
	write(fd,&id,sizeof(id));
	
	//p1接着运行
	while(1)
	{
		printf("p1进程接着运行\n");
		sleep(1);
	}
	
	//关闭有名管道
	close(fd);
	return 0;
	
}