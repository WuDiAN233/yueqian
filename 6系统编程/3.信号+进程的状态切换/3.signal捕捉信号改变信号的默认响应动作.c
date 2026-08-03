#include "myhead.h"

/*
	1.绝大部分信号都是终止进程
	2.signal函数有三个作用
	    作用1：改变信号的默认响应动作
		       例如：我要修改SIGINT的响应动作，当外界发送这个信号过来，当前进程不终止，打印helloworld
*/
void fun(int sig)
{
	printf("我接收到的信号序号是: %d\n",sig);
	printf("当前进程收到SIGINT信号了，我打印helloworld\n");
}
int main(int argc,char **argv)
{
	//调用signal改变SIGINT信号的响应动作
	signal(SIGINT,fun); //外界如果发送SIGINT信号，fun会被自动调用，通过signal修改了SIGINI的响应动作
	
	//防止主程序退出
	while(1)
	{
		
	}
}