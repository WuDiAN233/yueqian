#include "myhead.h"

/*
	fopen追加打开文件
*/

int main(int argc,char **argv)
{
	char buf[20]={0};
	FILE *myfile;
	//追加打开文件：打开之后，文件的偏移到了末尾位置
	myfile=fopen("./1.txt","a+");
	if(myfile==NULL)
	{
		perror("打开文件失败了!\n");
		return -1;
	}
	
	//写入内容到文件:会把yueqian写入到文件末尾
	fwrite("yueqian",7,1,myfile);
	
	//移动读写偏移
	fseek(myfile,0,SEEK_SET); //挪到起始位置
	//读取内容
	fread(buf,10,1,myfile);
	printf("读取的内容是: %s\n",buf);
	
	//关闭文件
	fclose(myfile);
	return 0;
	
}