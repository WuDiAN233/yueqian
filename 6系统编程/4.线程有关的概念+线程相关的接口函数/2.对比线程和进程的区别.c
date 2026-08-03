#include "myhead.h"

/*
	线程跟进程比，有啥区别
	   1.创建多个进程
            不能这么写代码，子进程会拷贝父进程的资源
                 fork()
				 fork()
				 fork()
		 创建多个线程 
		    可以这么写代码,子线程不会拷贝主线程的资源
			     pthread_create();
				 pthread_create();
				 pthread_create();
	   2.子进程不需要依赖父进程
	     子线程依赖于主线程，主线程结束，所有的子线程全部结束
	   3.子进程跟父进程不共用全局变量，也不共用局部变量
	     子线程跟主线程共用同一个全局变量，主线程定义的局部变量，子线程无法使用(子线程没有拷贝局部变量)
	   4.概念
	      主线程：你调用pthread_create函数对应的主程序，叫做主线程
		  子线程：pthread_create函数创建出来的都叫做子线程
*/

//定义全局变量--》三个线程跟主线程共用
int num=654;

//线程的任务函数：你创建线程为了做什么事情，这个事情就依靠任务函数来实现
void *task1(void *arg)
{
	while(1)
	{
		printf("线程1胖揍哪吒!全局变量是: %d\n",num);
		sleep(1);
	}
}
void *task2(void *arg)
{
	while(1)
	{
		printf("线程2胖揍托塔李天王!全局变量是: %d\n",num);
		sleep(1);
	}
}
void *task3(void *arg)
{
	while(1)
	{
		printf("线程3胖揍杨戬!全局变量是: %d\n",num);
		sleep(1);
	}
}

int main() //主线程 
{
	//定义局部变量
	int n=0;
	pthread_t id1,id2,id3; //存放线程的ID号，由操作系统自动分配
	//创建多个线程
	pthread_create(&id1,NULL,task1,NULL); //子线程
	pthread_create(&id2,NULL,task2,NULL); //子线程
	pthread_create(&id3,NULL,task3,NULL); //子线程 
	
	printf("线程的ID是: %ld\n",id1);
	printf("线程的ID是: %ld\n",id2);
	printf("线程的ID是: %ld\n",id3);
	
	//主函数：代表孙悟空
	while(1)
	{
		printf("孙悟空胖揍如来佛!,全局变量是: %d\n",num);
		n++;
		if(n>=10)
			num=123; //修改全局变量，会影响三个子线程
		sleep(1);
	}
	return 0;
}