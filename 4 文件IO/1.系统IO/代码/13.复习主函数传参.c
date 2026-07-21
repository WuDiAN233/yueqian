#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>  //open的头文件
#include <unistd.h>  //read的头文件
/*
	复习主函数传参
*/ 
int main(int argc,char **argv)
{
	printf("参数总个数: %d\n",argc);
	return 0;
}

