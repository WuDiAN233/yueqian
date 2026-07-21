#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

/*
	权限&(~umask)
*/ 
int main()
{
	int fd;
	//新建文件：写法2：
	fd=open("/home/gec/new.txt",O_CREAT|O_EXCL|O_RDWR,0777); //新建的时候，把文件的权限设置成777
	if(fd==-1)
	{
		printf("新建文件失败了!\n");
		return -1;
	}
	return 0;
}

