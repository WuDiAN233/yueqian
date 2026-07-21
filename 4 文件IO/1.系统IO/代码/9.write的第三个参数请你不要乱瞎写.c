#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>  //open的头文件
#include <unistd.h>  //read的头文件
/*
	write的第三个参数，不可以乱瞎写
	  1.你写入的字节数，只能<=实际字节数
	  2.>实际字节数，write会找垃圾数凑够你需要的字节数
	  3.read函数读取文件，第三个参数写多了，无所谓，read读完文件就不读了
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
	write(fd,"helloworld",100);
	

	//关闭文件
	close(fd);
	return 0;
}

