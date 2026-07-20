#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>  //open的头文件
#include <unistd.h>  //read的头文件
/*
	stat获取文件的属性信息
	属性信息：文件大小，文件的类型(七种类型)，文件的访问权限(r w x)
*/ 
int main(int argc,char **argv)
{
	//判断一下参数的个数
	if(argc<2)
	{
		printf("对不起，您忘记传递文件路径名。 ./程序名  文件路径名\n");
		return -1;
	}
	struct stat mystat;
	int ret;
	//获取文件的属性信息
	ret=stat(argv[1],&mystat);
	if(ret==-1)
	{
		printf("获取%s文件的属性信息失败了\n",argv[1]);
		return -1;
	}
	
	//获取大小
	printf("文件的大小是: %ld\n",mystat.st_size);
	
	//获取判断文件的类型:用到linux定义好带参数的宏定义来判断
	if(S_ISREG(mystat.st_mode))
	{
		printf("该文件是个普通文件\n");
	}
	if(S_ISDIR(mystat.st_mode))
	{
		printf("该文件是个文件夹\n");
	}
	return 0;
}

