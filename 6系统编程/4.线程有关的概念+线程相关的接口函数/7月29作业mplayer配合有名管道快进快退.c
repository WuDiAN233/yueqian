#include "myhead.h"

/*
	mplayer配合有名管道快进快退
	   1.mplayer也可以用信号实现暂停，继续，退出，格式跟madplay一模一样
*/

int main()
{
	int ret;
	int fd;
	int n;
	//定义数组存放需要使用的命令
	char cmd[30];
	
	//判断有名管道是否存在
	if(access("/fifo",F_OK)!=0) //有名管道不存在
	{
		//创建有名管道: 在开发板的根目录新建有名管道，管道文件的名字叫做fifo
		ret=mkfifo("/fifo",0777);
		if(ret==-1)
		{
			perror("创建有名管道失败了\n");
			return -1;
		}
	}
	
	
	//打开有名管道
	fd=open("/fifo",O_RDWR);
	if(fd==-1)
	{
		perror("打开有名管道失败了\n");
		return -1;
	}
	
	//调用mplayer播放某个视频
	system("mplayer -idx -slave -quiet -input file=/fifo 1.avi &");
	while(1)
	{
		printf("请输入要快进，还是快退 +5表示快进5秒   -5快退5秒\n");
		scanf("%d",&n);
		//拼接字符串得到命令
		bzero(cmd,30);
		sprintf(cmd,"seek %d\n",n);
		//写入命令到有名管道中
		write(fd,cmd,strlen(cmd));
	}
	
	//关闭有名管道
	close(fd);
	return 0;
	
}