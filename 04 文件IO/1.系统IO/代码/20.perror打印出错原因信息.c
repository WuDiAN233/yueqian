#include "myhead.h"

/*
    perror打印失败的具体原因
	   1.之前printf打印，很笼统
	   2.perror可以分析函数调用具体的失败原因
	     以后调用liux中函数，失败了，都用perror打印
*/ 
int main()
{
	int fd;
	//打开记事本
	fd=open("./1.txt",O_RDWR);
	if(fd==-1)
	{
		//把系统定义的全局变量打印出来
		printf("系统定义的全局变量errno记录了错误码:  %d\n",errno);
		perror("文件打开失败，失败的原因:\n");
		return -1;
	}
	
	
	//关闭文件
	close(fd);
	return 0;
}

