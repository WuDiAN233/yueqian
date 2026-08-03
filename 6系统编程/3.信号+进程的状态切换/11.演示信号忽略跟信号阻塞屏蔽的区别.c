#include "myhead.h"

/*
	忽略信号和阻塞(屏蔽)信号有什么区别
	    忽略信号：进程收到信号之后，直接丢弃这个信号，当它不存在
		阻塞(屏蔽)信号：进程暂时把信号挂起，不响应，如果你解除阻塞，依然可以响应
			 
*/

int main(int argc,char **argv)
{
	int i;
	//第一步：定义集合变量，存放所有你要阻塞的信号
	sigset_t myset;
	//清空集合
	sigemptyset(&myset);
	//往集合中添加你需要阻塞的信号
	sigaddset(&myset,1);
	sigaddset(&myset,2);
	sigaddset(&myset,3);
	
	//第二步：调用sigprocmask这个函数设置阻塞信号
	//SIG_BLOCK表示阻塞信号
	sigprocmask(SIG_BLOCK,&myset,NULL);
	
	for(i=0; i<12; i++)
	{
		printf("此时1，2，3都被我阻塞了，无法响应这三个信号，但是你发过来这三个信号只是被我挡住了(挂起)\n");
		sleep(1);
	}
	
	//立马解除阻塞
	//SIG_UNBLOCK表示解除阻塞信号
	sigprocmask(SIG_UNBLOCK,&myset,NULL);
	
	for(i=0; i<12; i++)
	{
		printf("此时1，2，3都被我解除阻塞了，可以响应这三个信号\n");
		sleep(1);
	}
	
	//阻塞进程等到信号到来
	pause();
	return 0;
}