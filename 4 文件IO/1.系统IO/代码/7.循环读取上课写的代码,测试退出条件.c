#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>  //open的头文件
#include <unistd.h>  //read的头文件
/*
	循环读取大文件，小的文件可以一口气全部读取，大的文件必须分段读取，循环读取
	  1.分段读取文件，必须写退出条件，你要知道文件啥时候读取完毕
	    答案：read的返回值
*/ 
int main()
{
	int fd;
	ssize_t ret;
	//定义数组存放读取的内容
	char buf[50]={0};
	//定义变量统计读取的次数
	int count=0;
	
	//打开文件
	fd=open("./1.c",O_RDWR); 
	if(fd==-1)
	{
		printf("打开文件失败了!\n");
		return -1;
	}
	
	//读取文件中的内容
	while(1)
	{	
		ret=read(fd,buf,10); //读取10个字节  683
		if(ret==0) //文件读取完毕
		{
			printf("文件读取完毕，准备退出！\n");
			break;
		}
		//printf("读取的内容是: %s,读取的字节数是: %ld\n",buf,ret);
		count++;
		printf("读取的字节数: %ld,读取的次数是: %d\n",ret,count);
	}

	//关闭文件
	close(fd);
	return 0;
}

