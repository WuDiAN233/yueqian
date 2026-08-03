#include "myhead.h"

/*
	线程取消类型：
	    延时取消：默认情况所有的线程都是延时取消，涉及到取消点函数概念
		立即取消：需要程序员调用函数设置才行
		          int pthread_setcanceltype(int type, int *oldtype);
				       参数：type --》宏定义
					          PTHREAD_CANCEL_DEFERRED      延时取消(默认就是这个)
		                      PTHREAD_CANCEL_ASYNCHRONOUS  立即取消
*/
//线程的任务函数
void *task1(void *arg) //所有的线程默认都是延时取消
{	
	int i,j;
	//循环的嵌套--》实现延时  sleep()取消点函数  printf取消点函数
	//我要保证两个for在执行的过程中没有使用到任何取消点函数
	for(i=0; i<100000; i++)  //for循环的嵌套执行时长是大于3秒钟
	{
		for(j=0; j<50000; j++)
		{
			//空语句
		}	
	}
	//调用某个取消点函数--》来证明延时取消
	fputc('@',stdout); //是个取消点函数
}

int main()
{
	pthread_t id1; //存放线程的ID 
	
	//创建一个线程帮我杀毒
	pthread_create(&id1,NULL,task1,NULL);
	
	sleep(3);
	//取消线程
	pthread_cancel(id1);
	
	//回收线程
	pthread_join(id1,NULL);
	return 0;
}