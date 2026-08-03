#include "myhead.h"

/*	 
	7月31作业
      1.放票的数量<线程的数量，会出现负数，如何解决？
        解决方法
           	if(ticketnum<=0)     只能判断一次
            while(ticketnum<=0)	 循环判断，可以判断多次
            这两句话有啥区别？
		
*/

//定义全局变量存放目前剩余票数
int ticketnum=10;
//定义互斥锁
pthread_mutex_t mymutex;
//定义条件变量
pthread_cond_t  mycond;

void *task1(void *arg)
{
	while(1)
	{
		//上锁
		pthread_mutex_lock(&mymutex);
		while(ticketnum<=0) //程序员规定的条件
		{
			//用条件变量阻塞当前窗口，暂停售票
			//条件变量：不可以单独使用，必须配合互斥锁一起使用
			/*
				为什么条件变量必须配合互斥锁一起使用？
				答案：pthread_cond_wait函数有两个作用
				      先解锁，然后阻塞当前线程
					  先解锁：为了防止出现死锁，解锁之后其它线程就可以上锁，就不会出现死锁的现象
			*/
			printf("窗口1阻塞在wait这里了\n");
			pthread_cond_wait(&mycond,&mymutex);
		}
		ticketnum--;
		printf("窗口1卖出一张票，剩余票数是: %d\n",ticketnum);
		sleep(1);
		//解锁
		pthread_mutex_unlock(&mymutex);
		usleep(5000);
	}
	
	//退出
	pthread_exit(NULL);
}

void *task2(void *arg)
{
	while(1)
	{
		//上锁
		pthread_mutex_lock(&mymutex);
		while(ticketnum<=0) //程序员规定的条件
		{
			//用条件变量阻塞当前窗口，暂停售票
			//条件变量：不可以单独使用，必须配合互斥锁一起使用
			/*
				为什么条件变量必须配合互斥锁一起使用？
				答案：下午讲解
			*/
			printf("窗口2阻塞在wait这里了\n");
			pthread_cond_wait(&mycond,&mymutex);
		}
		ticketnum--;
		printf("窗口2卖出一张票，剩余票数是: %d\n",ticketnum);
		sleep(1);
		//解锁
		pthread_mutex_unlock(&mymutex);
		usleep(5000);
	}
	//退出
	pthread_exit(NULL);
}

void *task3(void *arg)
{
	while(1)
	{
		//上锁
		pthread_mutex_lock(&mymutex);
		while(ticketnum<=0) //程序员规定的条件
		{
			//用条件变量阻塞当前窗口，暂停售票
			//条件变量：不可以单独使用，必须配合互斥锁一起使用
			/*
				为什么条件变量必须配合互斥锁一起使用？
				答案：下午讲解
			*/
			printf("窗口3阻塞在wait这里了\n");
			pthread_cond_wait(&mycond,&mymutex);
		}
		ticketnum--;
		printf("窗口3卖出一张票，剩余票数是: %d\n",ticketnum);
		sleep(1);
		//解锁
		pthread_mutex_unlock(&mymutex);
		usleep(5000);
	}
	//退出
	pthread_exit(NULL);
}

void *task4(void *arg)
{
	while(1)
	{
		//上锁
		pthread_mutex_lock(&mymutex);
		while(ticketnum<=0) //程序员规定的条件
		{
			//用条件变量阻塞当前窗口，暂停售票
			//条件变量：不可以单独使用，必须配合互斥锁一起使用
			/*
				为什么条件变量必须配合互斥锁一起使用？
				答案：下午讲解
			*/
			printf("窗口4阻塞在wait这里了\n");
			pthread_cond_wait(&mycond,&mymutex);
		}
		ticketnum--;
		printf("窗口4卖出一张票，剩余票数是: %d\n",ticketnum);
		sleep(1);
		//解锁
		pthread_mutex_unlock(&mymutex);
		usleep(5000);
	}
	//退出
	pthread_exit(NULL);
}

void *task5(void *arg)
{
	while(1)
	{
		//上锁
		pthread_mutex_lock(&mymutex);
		while(ticketnum<=0) //程序员规定的条件
		{
			//用条件变量阻塞当前窗口，暂停售票
			//条件变量：不可以单独使用，必须配合互斥锁一起使用
			/*
				为什么条件变量必须配合互斥锁一起使用？
				答案：下午讲解
			*/
			printf("窗口5阻塞在wait这里了\n");
			pthread_cond_wait(&mycond,&mymutex);
		}
		ticketnum--;
		printf("窗口5卖出一张票，剩余票数是: %d\n",ticketnum);
		sleep(1);
		//解锁
		pthread_mutex_unlock(&mymutex);
		usleep(5000);
	}
	//退出
	pthread_exit(NULL);
}
int main()
{
	pthread_t id1,id2,id3,id4,id5;

	//初始化互斥锁
	pthread_mutex_init(&mymutex,NULL);
	//初始化条件变量
	pthread_cond_init(&mycond,NULL);

	//创建五个线程模拟五个售票窗口 
	pthread_create(&id1,NULL,task1,NULL); 
	pthread_create(&id2,NULL,task2,NULL); 
	pthread_create(&id3,NULL,task3,NULL);
	pthread_create(&id4,NULL,task4,NULL);
	pthread_create(&id5,NULL,task5,NULL);
	
	//主线程代表铁道部，负责放票出来
	while(1)
	{
		if(ticketnum<=0)
		{
			printf("领导，票卖完了，请你放票出来\n");
			scanf("%d",&ticketnum);
			//唤醒所有阻塞的线程
			pthread_cond_broadcast(&mycond); 
			//唤醒一个线程
			//pthread_cond_signal(&mycond); 
		}
	}

	//回收
	pthread_join(id1,NULL);
	pthread_join(id2,NULL);
	pthread_join(id3,NULL);
	pthread_join(id4,NULL);
	pthread_join(id5,NULL);
	
	//销毁
	pthread_mutex_destroy(&mymutex);
	pthread_cond_destroy(&mycond);
	return 0;
}