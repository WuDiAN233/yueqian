#include "myhead.h"

/*
	 创建多个线程，正确的理解
        1.多个线程究竟谁先，谁后，取决于cpu的调度		
*/

void *task1(void *arg)
{
	while(1)
	{
		printf("线程1111\n");
		sleep(1);
	}
	
	//退出
	pthread_exit(NULL);
}

void *task2(void *arg)
{
	while(1)
	{
		printf("线程2222\n");
		sleep(1);
	}
	//退出
	pthread_exit(NULL);
}

void *task3(void *arg)
{
	while(1)
	{
		printf("线程3333\n");
		sleep(1);
	}
	//退出
	pthread_exit(NULL);
}

void *task4(void *arg)
{
	while(1)
	{
		printf("线程4444\n");
		sleep(1);
	}
	//退出
	pthread_exit(NULL);
}
int main()
{
	pthread_t id1,id2,id3,id4;


	//创建线程 
	pthread_create(&id1,NULL,task1,NULL); 
	pthread_create(&id2,NULL,task2,NULL); 
	pthread_create(&id3,NULL,task3,NULL);
	pthread_create(&id4,NULL,task4,NULL);

	//回收
	pthread_join(id1,NULL);
	pthread_join(id2,NULL);
	pthread_join(id3,NULL);
	pthread_join(id4,NULL);

	
	return 0;
}