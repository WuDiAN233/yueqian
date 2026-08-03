#include "myhead.h"

int main()
{
	while(1)
	{
		printf("当前进程正在运行中......id号: %d\n",getpid());
		sleep(1);
	}
}