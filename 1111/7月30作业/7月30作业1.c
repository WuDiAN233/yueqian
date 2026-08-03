#include "myhead.h"

/*
	父子进程模拟司机和售票员              
*/
pid_t id;
//定义标志位，让父进程退出
int flag=1;

//售票员收到信号之后的响应函数
void fun1(int sig)
{
	if(sig==SIGINT)
		//发送SIGUSR1给司机
		kill(getppid(),SIGUSR1);
	if(sig==SIGQUIT)
		//发送SIGUSR2给司机
		kill(getppid(),SIGUSR2);
	if(sig==SIGUSR1)
	{
		printf("all get off bus\n");
		//结束子进程
		exit(0);
	}
		
}
//司机收到信号之后的响应函数
void fun2(int sig)
{
	if(sig==SIGUSR1)
		printf("move to next station!\n");
	if(sig==SIGUSR2)
		printf("stop the bus!\n");
	if(sig==SIGTSTP)
	{
		kill(id,SIGUSR1);
		//修改标志位，让父进程的死循环可以结束
		flag=0;
	}
		
}
int main() //本身编译运行就是一个独立的进程--》叫做父进程 
{
	//创建一个子进程
	id=fork(); //产生新的进程(子进程)，总共就有两个进程产生
	if(id>0)  //司机
	{
		printf("司机ID：%d\n",getpid());
		signal(SIGUSR1,fun2);
		signal(SIGUSR2,fun2);
		signal(SIGTSTP,fun2);
		while(flag)
		{
			
		}
	}
	else if(id==0)  //售票员
	{
		printf("售票员ID：%d\n",getpid());
		signal(SIGINT,fun1);
		signal(SIGQUIT,fun1);
		signal(SIGUSR1,fun1);
		while(1)
		{
			
		}
	}
	else
	{
		perror("创建子进程失败了\n");
		return -1;
	}
	
	//回收子进程
	wait(NULL);
	printf("父进程回收了子进程\n");
	return 0;
}