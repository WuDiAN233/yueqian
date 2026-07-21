#include "myhead.h"

/*
	标准IO循环读写拷贝文件
*/

int main(int argc,char **argv)
{
	if(argc>=3) //判断参数个数
	{
		char buf[4096]; //存放读取的内容
		FILE *srcfile; 
		FILE *destfile;
		size_t ret;
		//打开源文件
		srcfile=fopen(argv[1],"r+");   //可读写，不会新建文件
		if(srcfile==NULL)
		{
			perror("打开文件失败了\n");
			return -1;
		}
		
		//新建目标文件
		destfile=fopen(argv[2],"w+");   //可读写，会新建文件
		if(destfile==NULL)
		{
			perror("打开文件失败了\n");
			return -1;
		}
		
		//循环读取源文件，写入到目标文件
		while(1)
		{
			bzero(buf,4096);
			//读取源文件:技巧我把数据块按照1个字节划分，此时fread的返回值刚好就是读取到的字节数
			ret=fread(buf,1,4096,srcfile);
			//写入到目标文件
			fwrite(buf,1,ret,destfile);
			//判断文件是否读取完毕
			if(feof(srcfile)==1) //源文件读取完毕
				break;
		}
		//关闭文件
		fclose(srcfile);
		fclose(destfile);
	}
	return 0;
}