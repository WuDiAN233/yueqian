#include "myhead.h"

/*
	单进程：没有创建子进程
*/
int main() //本身编译运行就是一个独立的进程
{
	while(1)
	{
		printf("父进程负责杀毒!\n");
		sleep(1);
	}
}