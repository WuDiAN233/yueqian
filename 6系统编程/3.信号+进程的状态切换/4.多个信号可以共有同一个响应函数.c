#include "myhead.h"

/*
	多个信号可以共有同一个响应函数
*/
void fun(int sig)
{
	if(sig==1)
	{
		printf("显示图片\n");
	}
	if(sig==2)
	{
		printf("播放歌曲\n");
	}
	if(sig==3)
	{
		printf("播放视频\n");
	}
	if(sig==4)
	{
		printf("打开led灯\n");
	}
	if(sig==9)
	{
		printf("打开蜂鸣器\n");
	}
	if(sig==19)
	{
		printf("打开蓝牙\n");
	}
}
int main(int argc,char **argv)
{
	//1到4号信号共有fun
	signal(SIGINT,fun); //外界如果发送SIGINT信号，fun会被自动调用，通过signal修改了SIGINI的响应动作
	signal(1,fun);
	signal(3,fun);
	signal(4,fun);
	
	//9和19号两个信号不能改变
	signal(9,fun);   //毫无意义，不起作用
	signal(19,fun);  //毫无意义，不起作用
	
	//防止主程序退出
	while(1)
	{
		
	}
}