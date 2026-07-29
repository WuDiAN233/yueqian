#include "myhead.h"

/*
	无名管道读写端不可以混淆
*/
int main() //本身编译运行就是一个独立的进程--》叫做父进程 
{
	pid_t id;
	int ret;
	char buf[100];
	//定义数组存放无名管道读端和写端的文件描述符
	int fd[2];
	//第一步：创建无名管道
	ret=pipe(fd);
	if(ret==-1)
	{
		perror("创建无名管道失败了\n");
		return -1;
	}
	
	//创建一个子进程
	id=fork(); //产生新的进程(子进程)，总共就有两个进程产生
	if(id>0)  //父进程 
	{
		while(1)
		{
			bzero(buf,100);
			printf("请输入要发送给子进程的信息!\n");
			scanf("%s",buf);
			//写入数据到无名管道：用错了文件描述符
			write(fd[0],buf,strlen(buf));
		}
	}
	else if(id==0) //子进程，把父进程的变量id拷贝了一份，名字跟父进程的是一样，但是相互独立
	{
		while(1)
		{
			bzero(buf,100);
			//读取无名管道中的数据
			//read的第三个不必纠结，假设你写是100个字节，无名管道有多少数据，read就读取多少
			//如果没有进程写入数据到无名管道，read会一直阻塞
			read(fd[1],buf,100); //用错了文件描述符
			printf("子进程读取到的信息是: %s\n",buf);
		}
		exit(0);
	}
	else
	{
		perror("创建子进程失败了\n");
		return -1;
	}
	
	wait(NULL); //参数NULL表示父进程只负责回收子进程，但是子进程的退出状态信息不关心
	//关闭无名管道
	close(fd[0]);
	close(fd[1]);
	return 0;
}