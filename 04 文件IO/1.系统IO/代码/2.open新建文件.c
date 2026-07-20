#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

/*
	open新建文件
	   1.linux中，只要一个函数的参数需要你同时使用多个宏定义，多个宏定义都是用按位或连接起来
	   2.0777是八进制的777，转换成二进制 111 111 111
	     linux用三个二进制位表示权限
		      可读    可写   可执行
			    1      1       1
				0      0       0
		    比如：110表示可读，可写，不可执行
		 为什么要写3个7呢？  用户组--》多个用户组成的集合
		     3个7分别代表当前用户，同组用户，其它用户对于该文件的访问权限
	     
*/ 
int main()
{
	int fd;
	//新建文件：写法1：
	//fd=open("./new.txt",O_CREAT|O_EXCL|O_RDWR);  //新建的时候，不设置文件的权限,O_EXCL表示文件存在就不新建，不存在才会帮你新建
	//fd=open("./new.txt",O_CREAT|O_TRUNC|O_RDWR);   //O_TRUNC表示文件存在就清空覆盖原来的文件
	if(fd==-1)
	{
		printf("新建文件失败了!\n");
		return -1;
	}
	//新建文件：写法2：
	open("./new.txt",O_CREAT|O_EXCL|O_RDWR,0777); //新建的时候，把文件的权限设置成777
	return 0;
}

