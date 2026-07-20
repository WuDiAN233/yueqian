#include "myhead.h"

/*
    lseek的返回值代表当前位置距离起始位置的字节数
	假设：最开始记事本是hellogec
*/ 
int main()
{
	int fd;
	off_t ret;
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
	ret=lseek(fd,2,SEEK_CUR);
	printf("当前偏移距离起始位置字节数是: %ld\n",ret);
	
	//写入yue到文件中
	write(fd,"yue",3);
	
	//从当前位置往前偏移4个字节
	ret=lseek(fd,-4,SEEK_CUR);
	printf("当前偏移距离起始位置字节数是: %ld\n",ret);
	
	//读取内容 
	bzero(buf,10);
	read(fd,buf,3);
	printf("buf is: %s\n",buf);
	
	//关闭文件
	close(fd);
	return 0;
}

