#include "myhead.h"

/*
	子线程创建：从主线程中分割一部分资源给到子线程
	            子线程结束的时候，需要主线程回收资源
*/
struct student
{
	char name[10];
	int age;
	float score;
};
//线程的任务函数：你创建线程为了做什么事情，这个事情就依靠任务函数来实现
void *task(void *arg)
{
	//int n=*((int *)arg);
	struct student *n=(struct student *)arg;
	
	//printf("你传递给线程的参数是: %d\n",n);
	printf("姓名:  %s  年龄：%d  成绩: %f\n",n->name,n->age,n->score);
	
	for(int i=0; i<5; i++)
	{
		printf("当前这个线程的: %ld\n",pthread_self());
		sleep(1);
	}
	//线程退出的时候，程序员可以通过void *传递退出信息给主线程
	//主线程回收子线程的时候，就可以获取到子线程传递的信息
	//写法1：线程退出，但是不传递任何信息给主线程
	//pthread_exit(NULL); //NULL 子线程直接退出，不传递任何有用信息给主线程
	//写法2：线程退出，但是传递信息给主线程
	//栈空间和堆空间
	//int n1=56; //局部变量，栈空间，函数调用结束栈空间会自动释放
	//char n2[30]="我退出了";   //局部变量，栈空间，函数调用结束栈空间会自动释放
	//pthread_exit(&n1); //错误的写法，线程任务函数结束的时候，n1的地址空间已经释放了
	//pthread_exit(n2);  //错误的写法，线程任务函数结束的时候，n2的地址空间已经释放了
	
	//正确写法参考1：用静态变量
	//static char n2[30]="我退出了"; //用的数据段，不是栈空间
	//正确写法参考2：用字符串常量
	//pthread_exit("我准备退出了主程序回收我吧哈哈");
	//正确写法参考3：用堆空间
	char *n2=malloc(30);
	strcpy(n2,"I am back");
	pthread_exit(n2);
}

int main() //主线程 
{
	int n1=56;
	float n2=89.9;
	struct student n3={"张三",18,85.5};
	
	//二级指针：降级为一级指针再使用
	void *arg;
	
	pthread_t id; //存放线程的ID号，由操作系统自动分配
	//创建线程
	//pthread_create(&id,NULL,task,&n1); //子线程
	//pthread_create(&id,NULL,task,&n2);
	pthread_create(&id,NULL,task,&n3);
	
	//回收子线程
	/*
		pthread_join函数跟前面学过回收子进程的wait类似，阻塞主程序，等待子线程退出
		pthread_join的第二个参数是二级指针(用来存放一级指针的地址)
		二级指针用来自动存放pthread_exit的参数的地址
	*/
	//对应pthread_exit(NULL)
	//pthread_join(id,NULL);
	//对应pthread_exit(n2);
	pthread_join(id,&arg);
	printf("子线程退出时候传递给我的信息是: %s\n",(char *)arg);
	free(arg); //帮忙释放堆空间

	return 0;
}