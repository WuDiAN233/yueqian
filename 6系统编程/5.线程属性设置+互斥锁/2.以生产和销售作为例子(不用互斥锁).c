#include "myhead.h"

/*
	生产部门：num++
	销售部门：num--
	生产和销售共有同一个资源--》num
	   1.理想情况
	         生产一件商品，就卖出一件，不要产生数据错乱
*/

int num=0; //工厂目前的库存数量

//线程的任务函数
void *task1(void *arg)
{
	while(1)
	{
		num++;
		printf("生产部门生产了一件商品，目前商品总数量是: %d\n",num);
		sleep(1);
	}
	
	//退出
	pthread_exit(NULL);
}

void *task2(void *arg)
{
	while(1)
	{
		num--;
		printf("销售部门卖出了一件商品，目前商品总数量是: %d\n",num);
		sleep(1);
	}
	
	//退出
	pthread_exit(NULL);
}
int main()
{
	pthread_t id1,id2;

	//创建两个线程，分别代表生产和销售部门
	pthread_create(&id1,NULL,task1,NULL);
	pthread_create(&id2,NULL,task2,NULL);

	//回收
	pthread_join(id1,NULL);
	pthread_join(id2,NULL);
	
	return 0;
}