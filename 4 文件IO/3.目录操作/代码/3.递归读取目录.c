#include "myhead.h"

/*
	封装函数实现递归读取目录
*/

//封装函数递归读取目录
int myreaddir(char *dirpath)
{
	DIR *mydir;
	struct dirent *mydirent;
	char allpath[300]; //存放绝对路径
	//打开目录
	mydir=opendir(dirpath);
	if(mydir==NULL)
	{
		perror("打开目录失败了\n");
		return -1;
	}
	
	//循环读取目录中的内容
	while((mydirent=readdir(mydir))!=NULL)
	{
		//排除掉.和..
		if(strcmp(mydirent->d_name,".")==0 || strcmp(mydirent->d_name,"..")==0)
			continue;
		
		if(mydirent->d_type==DT_DIR) //子目录
		{
			bzero(allpath,300);
			//拼接得到子目录的绝对路径
			sprintf(allpath,"%s/%s",dirpath,mydirent->d_name);
			//递归读取子目录
			myreaddir(allpath);
		}
		if(mydirent->d_type==DT_REG) //普通文件
		{
			bzero(allpath,300);
			//拼接得到子目录的绝对路径
			sprintf(allpath,"%s/%s",dirpath,mydirent->d_name);
			printf("当前你读取的文件是: %s\n",allpath);
		}
	}
	//关闭文件夹
	closedir(mydir);
	return 0;
}

int main(int argc,char **argv)
{
	//递归读取某个目录
	myreaddir(argv[1]);
	return 0;
}