#include "myhead.h"

/*
	忽略信号和阻塞(屏蔽)信号有什么区别
	    忽略信号：进程收到信号之后，直接丢弃这个信号，当它不存在
		阻塞(屏蔽)信号：进程暂时把信号挂起，不响应，如果你解除阻塞，依然可以响应
			 
*/

int main(int argc,char **argv)
{
	int i;
	//忽略信号 
	signal(1,SIG_IGN);
	signal(2,SIG_IGN);
	signal(3,SIG_IGN);
	
	for(i=0; i<12; i++)
	{
		printf("此时1，2，3都被我忽略了，无法响应这三个信号，但是你发过来这三个信号全部丢弃，没有了\n");
		sleep(1);
	}
	
	//立马恢复信号的默认动作
	signal(1,SIG_DFL);
	signal(2,SIG_DFL);
	signal(3,SIG_DFL);
	
	for(i=0; i<12; i++)
	{
		printf("此时1，2，3都被我恢复了，可以响应这三个信号\n");
		sleep(1);
	}
	
	//阻塞进程等到信号到来
	pause();
	return 0;
}