#include "myhead.h"

/*
	无名管道双向通信
*/
const char *info[] =
{
    "SIGUSR1",
    "SIGUSR2",
	"SIGUSR3"
};

volatile int i = 2;

void fun(int sig)
{
	printf("我接收到的信号序号是: %d\n",sig);
	if (sig == SIGINT)
	i=0;
	if (sig == SIGQUIT)
	i=1;
}

void tun(int sig)
{
	printf("stop the bus\n");
	if (sig == SIGTSTP)
	i=1;
}

int main() //本身编译运行就是一个独立的进程--》叫做父进程 
{
	pid_t id;
	int ret;
	char buf[100];
	char otherbuf[100];
	//定义数组存放无名管道读端和写端的文件描述符
	int fd1[2];
	int fd2[2];
	//第一步：创建无名管道
	ret=pipe(fd1);
	if(ret==-1)
	{
		perror("创建无名管道失败了\n");
		return -1;
	}
	ret=pipe(fd2);
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
			bzero(otherbuf,100);
			signal(SIGINT, SIG_IGN);
			signal(SIGQUIT, SIG_IGN);
			signal(SIGTSTP, tun);
			//写入数据到第一个无名管道
			strcpy(buf, info[i]);
			write(fd1[1],buf,strlen(buf));
			//判断是不是quit
			if(i == 0)
			{
    		i = 2;
			}
			if(strcmp(buf,"quit")==0)
				break;
			//从第二个无名管道中读取信息
			read(fd2[0],otherbuf,100);
			if(strcmp(otherbuf,"SIGUSR1")==0)
			printf("move to next station\n");
			if(strcmp(otherbuf,"SIGUSR2")==0)
			printf("stop the bus\n");
		}
	}
	else if(id==0) //子进程，把父进程的变量id拷贝了一份，名字跟父进程的是一样，但是相互独立
	{
		while(1)
		{
			bzero(buf,100);
			bzero(otherbuf,100);
			read(fd1[0],buf,100);
			if(strcmp(buf,"SIGUSR1")==0){
				printf("all get off the bus\n");
				break;
			}
			signal(SIGINT, fun);
			signal(SIGQUIT, fun);
			strcpy(otherbuf, info[i]);
			//发送给父进程，写入到第二个无名管道中
			write(fd2[1],otherbuf,strlen(otherbuf));
			if(strcmp(otherbuf,"quit")==0)
				break;
			if(i == 0)
			{
    		i = 2;
			}
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
	close(fd1[0]);
	close(fd1[1]);
	close(fd2[0]);
	close(fd2[1]);
	return 0;
}