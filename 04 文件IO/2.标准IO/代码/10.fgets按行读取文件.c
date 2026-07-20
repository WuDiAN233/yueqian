#include "myhead.h"

/*
	fread：只能按照程序员指定的大小，数据块去读取
	       不适合一行行读取文件
	fgets：专门为行读取而生的
	       fgets按行读取，会把每一行后面的回车也读取出来
*/

int main(int argc,char **argv)
{
	int ret;
	char *p;
	char buf[50]={0};
	FILE *myfile;
	//打开文件
	myfile=fopen("./1.txt","r+");
	if(myfile==NULL)
	{
		perror("打开文件失败了!\n");
		return -1;
	}
	
	//按行读取文件
	/*
		fgets第二个参数如果超过了一整行的大小，fgets不会跳到下一行读取
		fgets遇到回车换行就停止读取
	*/
	// fgets(buf,50,myfile);
	// printf("buf is: %s\n",buf);
	
	// bzero(buf,50);
	// fgets(buf,50,myfile);
	// printf("buf is: %s\n",buf);
	
	// bzero(buf,50);
	// fgets(buf,50,myfile);
	// printf("buf is: %s\n",buf);
	
	//循环读取文件
	while(1)
	{
		bzero(buf,50);
		p=fgets(buf,50,myfile); //一次只能读取一行数据
		printf("buf is: %s\n",buf);
		printf("fgets的返回值代表读取的一行数据的首地址: %s\n",p);
		//判断文件是否读取完毕、
		if(feof(myfile)==1)
			break;
	}

	//关闭文件
	fclose(myfile);
	return 0;
	
}