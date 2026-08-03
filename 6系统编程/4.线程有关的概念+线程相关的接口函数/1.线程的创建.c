#include "myhead.h"

/*
	线程的创建
	   1.线程，进程都是为了实现多任务并发
	   2.线程跟进程比，有啥区别
	     
*/

//线程的任务函数：你创建线程为了做什么事情，这个事情就依靠任务函数来实现
void *task1(void *arg)
{
	while(1)
	{
		printf("线程1胖揍哪吒!\n");
		sleep(1);
	}
}

int main() //孙悟空
{
	pthread_t id; //存放线程的ID号，由操作系统自动分配
	//创建一个线程用来模拟孙悟空的小猴子--》打哪吒
	pthread_create(&id,NULL,task1,NULL);
	printf("线程的ID是: %ld\n",id);
	
	//主函数：代表孙悟空
	while(1)
	{
		printf("孙悟空胖揍如来佛!\n");
		sleep(1);
	}
	
}