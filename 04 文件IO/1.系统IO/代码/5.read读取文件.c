#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>  //open的头文件
#include <unistd.h>  //read的头文件
/*
	read读取文件
*/ 
int main()
{
	int fd;
	//定义数组存放读取的内容
	char buf[50]={0};
	
	//打开文件
	fd=open("./1.txt",O_RDWR); 
	if(fd==-1)
	{
		printf("打开文件失败了!\n");
		return -1;
	}
	
	//读取文件中的内容：默认都是从起始位置
	read(fd,buf,7); //读取7个字节
	printf("读取的内容是: %s\n",buf);
	
	//关闭文件
	close(fd);
	return 0;
}

