#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>  //open的头文件
#include <unistd.h>  //read的头文件
/*
	write写入文件
*/ 
int main()
{
	int fd;
	ssize_t ret;

	//打开文件
	fd=open("./1.txt",O_RDWR); 
	if(fd==-1)
	{
		printf("打开文件失败了!\n");
		return -1;
	}
	
	//写入内容到文件中
	write(fd,"helloworld",10);
	write(fd,"你好粤嵌",12);

	//关闭文件
	close(fd);
	return 0;
}

