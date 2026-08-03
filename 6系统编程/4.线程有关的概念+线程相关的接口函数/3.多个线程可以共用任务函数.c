#include "myhead.h"

/*
	多个线程可以共用任务函数
	   1. pthread_t pthread_self(void) 函数返回当前线程的ID号

*/

//线程的任务函数：你创建线程为了做什么事情，这个事情就依靠任务函数来实现
void *task(void *arg)
{
	while(1)
	{
		printf("当前这个线程的: %ld\n",pthread_self());
		sleep(1);
	}
}

int main() //主线程 
{
	//同一个变量反复使用
	pthread_t id; //存放线程的ID号，由操作系统自动分配
	//创建多个线程
	pthread_create(&id,NULL,task,NULL); //子线程
	printf("线程的ID是: %ld\n",id);
	pthread_create(&id,NULL,task,NULL); //子线程
	printf("线程的ID是: %ld\n",id);
	pthread_create(&id,NULL,task,NULL); //子线程 
	printf("线程的ID是: %ld\n",id);

	
	//主函数：代表孙悟空
	while(1)
	{
		printf("孙悟空胖揍如来佛!\n");
		sleep(1);
	}
	return 0;
}