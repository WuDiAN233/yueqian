#include "myhead.h"

/*
	创建多个子进程
	   
*/
pid_t id1,id2;

//父进程收到信号之后的响应函数
void fun1(int sig)
{
	//给两个子进程发送信号
	kill(id1,SIGUSR1);
	kill(id2,SIGUSR2);
}

//子进程1收到信号之后的响应函数
void fun2(int sig)
{
	printf("Child process 1 is killed my parent: %d\n",sig);
	exit(1); //退出子进程1
}

//子进程2收到信号之后的响应函数
void fun3(int sig)
{
	printf("Child process 2 is killed my parent: %d\n",sig);
	exit(2); //退出子进程2
}

int main() //本身编译运行就是一个独立的进程--》叫做父进程 
{
	//创建第一个子进程
	id1=fork();
	if(id1>0) //父进程
	{
		//创建第二个子进程
		id2=fork();
		if(id2==0) //第二个子进程
		{
			signal(SIGUSR2,fun3);
			pause();
		}
		else if(id2>0) //父进程
		{
			printf("父进程的ID是: %d\n",getpid());
			signal(SIGINT,fun1);
			pause();
		}
	}
	else if(id1==0) //第一个子进程
	{
		signal(SIGUSR1,fun2);
		pause();
	}
	
	wait(NULL);
	wait(NULL);
	printf("Parent process exit!\n");
	return 0;
}