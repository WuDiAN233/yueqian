#include "myhead.h"

/*
	
*/

int main(int argc,char **argv)
{
	//平时打印数据到液晶屏显示
	//printf
	
	//系统IO往液晶屏中写入字符串hello
	write(STDOUT_FILENO,"hello",5);
	write(1,"world",5);  //1跟STDOUT_FILENO等价的
	
	//标准IO写入字符串到液晶屏
	fwrite("yueqian",7,1,stdout);
	
	return 0;
}