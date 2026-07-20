#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>  //open的头文件
#include <unistd.h>  //read的头文件
#include <strings.h>

/*
	用系统IO拷贝上课的某个录屏
*/ 
int main(int argc,char **argv)
{
	ssize_t ret;
	int srcfd; //存放源文件的文件描述符
	int destfd; //存放目标文件的文件描述符
	//定义数组存放读取到的视频数据
	char buf[4096];  //数组无法定义太大的数组，原因该数组是个局部变量，使用是栈空间
	
	//判断用户是否传递参数
	if(argc>=3)  
	{
		//打开源文件
		srcfd=open(argv[1],O_RDWR);
		if(srcfd==-1)
		{
			printf("打开源文件失败了!\n");
			return -1;
		}
		//新建目标文件
		destfd=open(argv[2],O_CREAT|O_TRUNC|O_RDWR);
		if(destfd==-1)
		{
			printf("新建目标文件失败了!\n");
			return -1;
		}
		
		//由于视频文件比较大，无法一口气全部读完
		while(1) //循环读取，循环写入   1250000字节
		{
			bzero(buf,4096);
			ret=read(srcfd,buf,4096); //读取源文件
			if(ret==0) //文件读取完毕
				break;
			//写入到目标文件
			write(destfd,buf,ret);
		}
		
		//关闭文件
		close(srcfd);
		close(destfd);
		return 0;
	}
}

