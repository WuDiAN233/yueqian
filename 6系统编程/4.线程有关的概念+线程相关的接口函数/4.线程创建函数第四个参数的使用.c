#include "myhead.h"

/*
	线程创建第四个参数的使用
	    1.跟LVGL的添加事件响应函数一模一样的用法
		     lv_obj_add_event_cb(  响应函数     传递参数)
*/

//线程的任务函数：你创建线程为了做什么事情，这个事情就依靠任务函数来实现
void *task(void *arg)
{
	int n=*((int *)arg);
	printf("你传递给线程的参数是: %d\n",n);
	while(1)
	{
		printf("当前这个线程的: %ld\n",pthread_self());
		sleep(1);
	}
}

int main() //主线程 
{
	int n1=56;
	float n2=89.9;
	pthread_t id; //存放线程的ID号，由操作系统自动分配
	//创建线程
	pthread_create(&id,NULL,task,&n1); //子线程
	
	//主函数：代表孙悟空
	while(1)
	{
		printf("孙悟空胖揍如来佛!\n");
		sleep(1);
	}
	return 0;
}