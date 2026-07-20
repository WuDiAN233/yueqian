#include "myhead.h"

/*
    1.假设最开始，记事本是hellogec
	2.SEEK_CUR表示当前位置，当前位置随着read或者write发生改变 
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
	
	//读取内容
	read(fd,buf,3);
	printf("buf is: %s\n",buf);
	
	//从当前位置往后移动2个字节
	lseek(fd,2,SEEK_CUR);
	
	//写入yue到文件中
	write(fd,"yue",3);
	
	//从当前位置往前偏移4个字节
	lseek(fd,-4,SEEK_CUR);
	
	//读取内容 
	bzero(buf,10);
	read(fd,buf,3);
	printf("buf is: %s\n",buf);
	
	//关闭文件
	close(fd);
	return 0;
}

