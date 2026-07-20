#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>  //open的头文件
#include <unistd.h>  //read的头文件
/*
	read读取汉字
	   汉字读取不完整，就是乱码
	   但是ubuntu系统，默认汉字需要是utf-8编码，才能正常打印
	   1.ANSI编码的汉字，一个占2字节
	   2.utf-8编码的汉字，一个占3个字节
	   3.GB2312编码的汉字，一个占2字节
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
	
	//读取文件中的内容
	read(fd,buf,3); //读取3个字节
	printf("读取的内容是: %s\n",buf);
	
	//关闭文件
	close(fd);
	return 0;
}

