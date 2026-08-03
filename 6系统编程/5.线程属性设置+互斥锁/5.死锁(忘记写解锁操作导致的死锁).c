#include "myhead.h"

/*
	死锁
       1.概念
         使用互斥锁的过程中，如果线程只有上锁操作，没有对应的解锁操作
         会导致这个锁，其它线程无法上锁(一直阻塞)		 
*/

int num=0; //工厂目前的库存数量
//定义互斥锁变量
pthread_mutex_t mutex;

//生产部门
void *task1(void *arg)
{
	while(1)
	{
		//上锁
		pthread_mutex_lock(&mutex);
		num++;
		printf("生产部门生产了一件商品，目前商品总数量是: %d\n",num);
		sleep(1);
		//忘记写解锁操作
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
		printf("销售部门阻塞在上锁的位置了，等待生产部门解锁，但是生产部门忘记解锁了\n");
		pthread_mutex_lock(&mutex);
		num--;
		printf("销售部门卖出了一件商品，目前商品总数量是: %d\n",num);
		sleep(1);
		//解锁
		/*
			解锁的一瞬间存在两种可能性
			    可能性1: 刚解锁，自己无耻地把锁上上去了
				可能性2：刚解锁，生产部门抢到锁了，然后生产部门上锁
				这两种可能性取决于cpu如何调度
			程序员可以耍点小手段，我心中的答案是可能性2，我不想要可能性1
		*/
		pthread_mutex_unlock(&mutex);
		usleep(500); //500微秒
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

	//回收
	pthread_join(id1,NULL);
	pthread_join(id2,NULL);
	
	//销毁互斥锁
	pthread_mutex_destroy(&mutex);
	
	return 0;
}