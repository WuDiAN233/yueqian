#include "myhead.h"

/*
	p1和p2双向通信
	    目前这个代码的逻辑：只能是p1和p2轮流发送，无法做到自由发送
		如果你想实现：p1和p2自由发送(后天学了多线程就能实现)
*/

int main()
{
	int ret;
	int fd1,fd2;
	char buf[100];
	char otherbuf[100];

	//创建有名管道
	ret=mkfifo("/home/gec/fifo1",0777);
	if(ret==-1)
	{
		//判断失败的具体原因，如果是管道存在我就直接打开
		if(errno==EEXIST) //管道存在
		{
			//打开有名管道
			fd1=open("/home/gec/fifo1",O_RDWR);
			if(fd1==-1)
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
	ret=mkfifo("/home/gec/fifo2",0777);
	if(ret==-1)
	{
		//判断失败的具体原因，如果是管道存在我就直接打开
		if(errno==EEXIST) //管道存在
		{
			//打开有名管道
			fd2=open("/home/gec/fifo2",O_RDWR);
			if(fd2==-1)
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
	
	while(1)
	{
		bzero(buf,100);
		bzero(otherbuf,100);
		printf("请输入要发送给p2的信息!\n");
		scanf("%s",buf);
		//写入到第一个有名管道
		write(fd1,buf,strlen(buf));
		//判断是不是"quit"
		if(strcmp(buf,"quit")==0)
			break;
		//从第二个有名管道中读取数据
		read(fd2,otherbuf,100);
		if(strcmp(otherbuf,"quit")==0)
			break;
		printf("p2给我发送的信息是: %s\n",otherbuf);
	}
	
	//关闭有名管道
	close(fd1);
	close(fd2);
	return 0;
	
}