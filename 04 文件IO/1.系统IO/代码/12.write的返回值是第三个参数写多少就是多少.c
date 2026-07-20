#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>  //open的头文件
#include <unistd.h>  //read的头文件
/*
	write的第三个参数写多少，返回值就是多少
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
	/*
	    由于helloworld只有10个字节，但是你第三个参数写成了100
		write会找90个字节的随机数，凑够100个字节，写入到文件中
	*/
	ret=write(fd,"helloworld",1000);
	printf("write的返回值是第三个参数: %ld\n",ret);

	//关闭文件
	close(fd);
	return 0;
}

