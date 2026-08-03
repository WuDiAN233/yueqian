#include "myhead.h"

/*
	三个信号模拟交通信号灯
*/
void fun(int sig)
{
	if(sig==SIGQUIT)
	{
		printf("红灯停\n");
	}
	if(sig==SIGILL)
	{
		printf("绿灯停\n");
	}
	if(sig==SIGTRAP)
	{
		printf("黄灯注意观察\n");
	}


}
int main(int argc,char **argv)
{
	//共有fun
	signal(SIGQUIT,fun);
	signal(SIGILL,fun);
	signal(SIGTRAP,fun);
	
	pause();
}