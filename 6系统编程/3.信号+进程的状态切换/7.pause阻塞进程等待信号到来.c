#include "myhead.h"

/*
	pause函数阻塞当前进程，等待信号到来，信号一直不来，就一直阻塞
*/

int main(int argc,char **argv)
{
	int i;
	//调用signal忽略信号
	for(i=1; i<=64; i++)
	{
		if(i==32 || i==33) //32和33号信号不存在
			continue;
		signal(i,SIG_IGN);  //SIG_IGN表示我要忽略指定的信号
	}
	
	//pause函数阻塞当前进程，等待信号到来，信号一直不来，就一直阻塞
	pause();
	return 0;
}