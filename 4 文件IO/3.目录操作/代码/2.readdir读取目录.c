#include "myhead.h"

/*
    readdir读取目录
	   1.目录中包含了子目录，还有其他的一些文件
	     读取目录--》循环读取才能全部读完
	   2.读取目录，d_name存放的不是绝对路径，存放的仅仅只是文件或者子目录的名字
         readdir无法递归读取目录
		    递归读取目录：读取123这个目录的时候，如果发现123包含456这个子目录，继续读取456这个子目录中的内容
*/
int main(int argc,char **argv)
{
	DIR *mydir; //结构体指针，跟fopen返回FILE*类似的道理
	struct dirent *mydirent;
	//打开某个目录
	mydir=opendir(argv[1]);
	if(mydir==NULL)
	{
		perror("打开目录失败了\n");
		return -1;
	}
	
	//循环读取目录
	while(1)
	{
		mydirent=readdir(mydir);
		if(mydirent==NULL)
			break;
		printf("目前你读取的目录中包含的内容是: %s\n",mydirent->d_name);
		//判断文件类型
		if(mydirent->d_type==DT_DIR) 
			printf("当前这个文件是个目录(文件夹)\n");
		if(mydirent->d_type==DT_REG) 
			printf("当前这个文件是个普通文件\n");
	}
	
	//关闭目录
	closedir(mydir);
	return 0;
}