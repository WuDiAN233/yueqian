#include "myhead.h"

/*
	证明标准IO带缓冲区
	缓冲区：标准IO先把数据存放到缓冲区，然后在把缓冲区数据写入到文件中
*/

int main(int argc,char **argv)
{
	FILE *myfile;
	//打开文件
	myfile=fopen("./1.txt","r+");
	if(myfile==NULL)
	{
		perror("打开文件失败了!\n");
		return -1;
	}
	
	while(1)
	{
		//每隔1秒钟，写入hello到记事本
		fwrite("hello",5,1,myfile);
		sleep(1);
		//程序员主动刷新缓冲区(把缓冲区里面的内容提取出来写入到记事本)
		fflush(myfile);
	}
}