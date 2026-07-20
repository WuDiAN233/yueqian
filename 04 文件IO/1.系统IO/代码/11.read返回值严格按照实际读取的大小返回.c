#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>  //open的头文件
#include <unistd.h>  //read的头文件
/*
	read的返回值严格按照实际读取的真实字节数
	   假设：1.txt存放的是yue
*/ 
int main()
{
	int fd;
	ssize_t ret;
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
	ret=read(fd,buf,7); //读取7个字节
	printf("读取的内容是: %s\n",buf);
	printf("read的返回值严格按照实际读取的字节数返回: %ld\n",ret);
	
	//刚才第一次，文件已经被读取完毕，再次调用read读取，read返回0
	ret=read(fd,buf,7);
	printf("read的返回值严格按照实际读取的字节数返回: %ld\n",ret);
	//关闭文件
	close(fd);
	return 0;
}

