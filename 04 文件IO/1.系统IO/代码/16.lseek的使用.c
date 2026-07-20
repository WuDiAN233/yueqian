#include "myhead.h"

/*
	lseek设置读取文件的偏移位置
	假设：文件内容是hellogec
	现在：要求你跳过hello，读取文件中间的内容，读取gec
	答案：用lseek函数设置读写偏移
	   
*/ 
int main()
{
	int fd;
	char buf[10]={0};
	//打开记事本
	fd=open("./1.txt",O_RDWR);
	if(fd==-1)
	{
		printf("文件打开失败了\n");
		return -1;
	}
	
	//不可以马上使用read，read读取文件默认都是从起始位置开始读取
	//使用lseek函数，跳过最前面的5个字节(hello)，从第六个字节开始读取
	lseek(fd,5,SEEK_SET); //SEEK_SET表示文件的起始位置
	
	//读取文件内容
	read(fd,buf,3);
	printf("读取的内容是: %s\n",buf);
	
	//关闭文件
	close(fd);
	return 0;
}

