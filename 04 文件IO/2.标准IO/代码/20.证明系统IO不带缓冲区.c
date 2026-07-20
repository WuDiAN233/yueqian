#include "myhead.h"

/*
	证明系统IO没有缓冲区
	系统IO，你使用write会直接把内容写入到记事本
*/

int main(int argc,char **argv)
{
	int fd;
	//打开文件
	fd=open("./1.txt",O_RDWR);
	if(fd==-1)
	{
		perror("打开记事本失败\n");
		return -1;
	}
	
	while(1)
	{
		write(fd,"hello",5);
		sleep(1);
	}
}