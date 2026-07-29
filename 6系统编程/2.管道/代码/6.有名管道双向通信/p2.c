#include "myhead.h"

/*
	p1和p2双向通信
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
		//从第一个有名管道中读取数据
		read(fd1,buf,100);
		//判断是不是"quit"
		if(strcmp(buf,"quit")==0)
			break;
		printf("p1给我发送过来的信息是: %s\n",buf);
		printf("请输入要回复给p1的信息\n");
		scanf("%s",otherbuf);
		//通过第二个有名管道发送
		write(fd2,otherbuf,strlen(otherbuf));
		if(strcmp(otherbuf,"quit")==0)
			break;
	}
	
	//关闭有名管道
	close(fd1);
	close(fd2);
	return 0;
	
}