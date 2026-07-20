#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

/*
	测试文件描述符的最大值
	   1.拓展：文件描述符默认是0----1023
	   2.通过ulimit命令，修改文件描述符的最大值
	         ulimit  -n               查看当前系统最大值
			 ulimit  -n   3000        修改文件描述符最大值为3000
	   
*/ 
int main()
{
	int i;
	int fd;
	//循环打开同一个文件，多次
	for(i=0; i<2000; i++)
	{
		fd=open("./1.txt",O_RDWR);
		if(fd==-1)
		{
			printf("打开记事本失败了!\n");
			return -1;
		}
		printf("目前你打开的文件描述符是: %d\n",fd);
	}
	return 0;
}

