#include "myhead.h"

/*
	kill函数发送信号
*/
int main(int argc,char **argv)
{
	if(argc>=3)
	{
		kill(atoi(argv[1]),atoi(argv[2]));
		return 0;
	}
	else
	{
		printf("您忘记传递进程ID和信号的序号  ./程序名  id  信号序号\n");
		return -1;
	}
	
}