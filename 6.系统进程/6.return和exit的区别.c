#include "myhead.h"

/*
	return和exit的区别
      1.return是关键字   exit是个函数
      2.return是退出函数，把返回值给函数的调用者
        exit是结束整个进程	  
*/

int fun()
{
	printf("fun被调用了\n");
	//return 0;  //结束当前fun这个函数，把返回值0给调用者(主函数)
	exit(0);     //结束整个进程，主函数跟着倒霉，一起结束
}

int main() //本身编译运行就是一个独立的进程--》叫做父进程 
{
	printf("主函数开始运行!\n");
	
	//调用fun函数
	fun();
	
	printf("主函数接着往后执行\n");
	return 0;  //结束当前main这个函数，把返回值0给调用者(操作系统)
	
}