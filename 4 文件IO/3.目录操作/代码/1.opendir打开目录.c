#include "myhead.h"

/*
    opendir打开目录--》不能新建目录
	新建目录: mkdir函数
*/
int main(int argc,char **argv)
{
	DIR *mydir; //结构体指针，跟fopen返回FILE*类似的道理
	//打开某个目录
	mydir=opendir(argv[1]);
	if(mydir==NULL)
	{
		//进一步判断失败的具体原因
		if(errno==2) //2号错误表示没有这个目录
		{
			//新建目录
			mkdir(argv[1],0777);
		}
		else
		{
			perror("打开目录失败了\n");
			return -1;
		}
	}
	
	//关闭目录
	closedir(mydir);
	return 0;
}