#include "myhead.h"

/*
	死锁
       线程来不及解锁就被取消了 
	   解决方法：把线程设置成不可以被取消
*/

int num=0; //工厂目前的库存数量
//定义互斥锁变量
pthread_mutex_t mutex;

//生产部门
void *task1(void *arg)
{
	//线程设置成不可以被取消
	pthread_setcancelstate(PTHREAD_CANCEL_DISABLE,NULL);
	while(1)
	{
		//上锁
		pthread_mutex_lock(&mutex);
		num++;
		printf("生产部门生产了一件商品，目前商品总数量是: %d\n",num);
		sleep(3);
		pthread_mutex_unlock(&mutex);
		usleep(5000); //5000微秒
	}
	
	//退出
	pthread_exit(NULL);
}
//销售部门
void *task2(void *arg)
{
	while(1)
	{
		//上锁
		//printf("销售部门阻塞在上锁的位置了，等待生产部门解锁，但是生产部门忘记解锁了\n");
		pthread_mutex_lock(&mutex);
		num--;
		printf("销售部门卖出了一件商品，目前商品总数量是: %d\n",num);
		sleep(3);
		pthread_mutex_unlock(&mutex);
		usleep(5000); //5000微秒
	}
	
	//退出
	pthread_exit(NULL);
}
int main()
{
	pthread_t id1,id2;
	
	//初始化互斥锁
	pthread_mutex_init(&mutex,NULL);

	//创建两个线程，分别代表生产和销售部门
	pthread_create(&id1,NULL,task1,NULL); //生产部门 
	//刷点手段
	sleep(1);  //保证一定是生产部门先运行，先拿到锁
	pthread_create(&id2,NULL,task2,NULL); //销售部门
	
	sleep(1);
	//取消生产部门
	pthread_cancel(id1);

	//回收
	pthread_join(id1,NULL);
	pthread_join(id2,NULL);
	
	//销毁互斥锁
	pthread_mutex_destroy(&mutex);
	
	return 0;
}