#include "myhead.h"

/*
	fputs写入字符串到文件
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
	
	//写入字符串到文件
	fputs("中国人",myfile);
	

	//关闭文件
	fclose(myfile);
	return 0;
	
}