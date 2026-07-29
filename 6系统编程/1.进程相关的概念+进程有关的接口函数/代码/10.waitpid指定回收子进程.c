#include "myhead.h"

/*
	waitpid指定回收子进程
*/
int main() //本身编译运行就是一个独立的进程--》叫做父进程 
{
	pid_t id1,id2;
	int status;
	pid_t ret;
	
	//创建第一个子进程
	id1=fork();
	if(id1>0) //父进程
	{
		//创建第二个子进程
		id2=fork();
		if(id2==0) //第二个子进程
		{
			int i;
			for(i=0; i<10; i++)
			{
				printf("我是第二个子进程，我负责扫描漏洞,我的ID是: %d\n",getpid());
				sleep(1);
			}
			exit(2);
		}
		else if(id2>0) //父进程
		{
			int i;
			for(i=0; i<3; i++)
			{
				printf("我是父进程，我负责安装驱动,我的ID是: %d\n",getpid());
				printf("我二儿子ID是: %d\n",id2);
				printf("我大儿子ID是: %d\n",id1);
				sleep(1);
			}
		}
	}
	else if(id1==0) //第一个子进程
	{
		int i;
		for(i=0; i<5; i++)
		{
			printf("我是第一个子进程，我负责清除垃圾,我的ID是: %d\n",getpid());
			sleep(1);
		}
		exit(1);
	}
	
	//回收子进程
	ret=waitpid(id1,&status,0); //我要先回收子进程1
	printf("我现在回收的子进程是: %d\n",ret);
	printf("我回收子进程退出值: %d\n",WEXITSTATUS(status));
	ret=waitpid(id2,&status,0); //我再回收子进程2
	printf("我现在回收的子进程是: %d\n",ret);
	printf("我回收子进程退出值: %d\n",WEXITSTATUS(status));
	
	return 0;
}