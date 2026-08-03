#include "myhead.h"

/*
	设置线程的取消状态
	   1.有两种状态
	       可以被取消
		   不能被取消
*/

//线程的任务函数：你创建线程为了做什么事情，这个事情就依靠任务函数来实现
void *task(void *arg)
{
	//设置线程不能被取消
	pthread_setcancelstate(PTHREAD_CANCEL_DISABLE,NULL);
	
	while(1)
	{
		printf("当前这个线程的: %ld\n",pthread_self());
		sleep(1);
	}
	pthread_exit(NULL);
}

int main() //主线程 
{
	pthread_t id; //存放线程的ID号，由操作系统自动分配
	//创建线程
	pthread_create(&id,NULL,task,NULL);
	
	sleep(5);
	
	//取消刚才创建的线程
	pthread_cancel(id);
	
	//回收子线程
	pthread_join(id,NULL);
	return 0;
}