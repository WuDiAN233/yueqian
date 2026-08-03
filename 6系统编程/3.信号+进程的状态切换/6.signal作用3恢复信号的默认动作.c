#include "myhead.h"

/*
	1.绝大部分信号都是终止进程
	2.signal函数有三个作用
	    作用1：改变信号的默认响应动作
		       例如：我要修改SIGINT的响应动作，当外界发送这个信号过来，当前进程不终止，打印helloworld
		作用2：忽略信号
		       信号发过来之后，当前进程直接丢弃这个信号，当它不存在
		作用3：恢复信号的默认动作
		       signal(i,SIG_DFL); 
			   
	    总结：SIGKILL和SIGSTOP两个信号既不能改变，也不能忽略
*/

int main(int argc,char **argv)
{
	int i;
	//调用signal忽略信号
	for(i=1; i<=64; i++)
	{
		if(i==32 || i==33) //32和33号信号不存在
			continue;
		signal(i,SIG_IGN);  //SIG_IGN表示我要忽略指定的信号
	}
	
	for(i=0; i<10; i++)
	{
		printf("主函数接着往后执行!,此时除了9和19号信号以外，其它信号都被我忽略了\n");
		sleep(1);
	}
	
	//恢复信号的默认动作
	for(i=1; i<=64; i++)
	{
		if(i==32 || i==33) //32和33号信号不存在
			continue;
		signal(i,SIG_DFL);  //SIG_DFL表示我要恢复信号的默认动作
	}
	
	for(i=0; i<10; i++)
	{
		printf("主函数,此时信号可以终止它\n");
		sleep(1);
	}
	
	
	//防止主程序退出
	while(1)
	{
		
	}
}