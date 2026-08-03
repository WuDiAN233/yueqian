#include "myhead.h"

/*
	线程属性设置的思路
	   第一步：定义属性结构体变量，并初始化
	        pthread_attr_t  myattr;
	   第二步：linux针对每个线程属性，都定义了专门的函数
	        函数统一叫做
			   pthread_attr_setxxxxx
		    以分离属性为例，其他属性的设置思路照抄本案例
            分离属性：
               可分离：线程退出的时候，不需要主线程调用pthread_join回收，想回收也回收不到
               不可分离：线程退出的时候，主线程必须调用pthread_join回收它
                         任何一个线程，你新建的时候默认的属性都是不可分离						 
*/

//线程的任务函数
void *task(void *arg)
{
	int i;
	for(i=0; i<5; i++)
	{
		printf("线程在执行\n");
		sleep(1);
	}
	
	//退出
	pthread_exit("我退出了");
}
int main()
{
	pthread_t id;
	void *arg;
	
	//第一步：定义属性结构体变量，并初始化
	pthread_attr_t  myattr;
	pthread_attr_init(&myattr);
	
	//第二步：调用linux提供专门的函数，去设置你想要的属性
	pthread_attr_setdetachstate(&myattr,PTHREAD_CREATE_DETACHED);
	
	//第三步：创建线程，使用刚才的属性
	/*
		之前第二个参数pthread_create(&id,NULL,);表示使用线程的默认属性
	*/
	pthread_create(&id,&myattr,task,NULL);
	
	sleep(10);
	
	//回收: join函数此时不阻塞(线程是分离，回收不了) 
	pthread_join(id,&arg);
	printf("我回收的线程，退出信息是： %s\n",(char *)arg);
	
	//第四步：销毁属性
	pthread_attr_destroy(&myattr);
	
	return 0;
}