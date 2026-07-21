#include "myhead.h"

/*
	access,chmod,stat综合使用
*/ 
int main(int argc,char **argv)
{
	//判断一下参数的个数
	if(argc>=2)
	{
		//判断文件是否存在
		if(access(argv[1],F_OK)==0) //文件存在
		{
			//进一步判断文件的权限是不是当前用户可读可写
			struct stat mystat;
			stat(argv[1],&mystat);
			if(((mystat.st_mode)&S_IRUSR) && ((mystat.st_mode)&S_IWUSR))
				printf("这个文件%s对于当前用户是可读可写的\n",argv[1]);
			else
			{
				chmod(argv[1],0600);
			}		
		}
		else //文件不存在
		{
			int fd;
			//新建,open的0777会受到umask的影响
			fd=open(argv[1],O_CREAT|O_TRUNC|O_RDWR,0777);
			if(fd==-1)
			{
				printf("新建文件失败!\n");
				return -1;
			}
			//确保文件的权限一定是0777
			//chmod不受到umaks的影响
			chmod(argv[1],0777);
		}
	}
	
	return 0;
}

