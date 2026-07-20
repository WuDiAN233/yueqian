#include "myhead.h"

/*
	标准IO打开文件
	  1.a+表示可读写追加
	  2.系统IO，open返回文件描述符
	    标准IO，fopen返回结构体指针，存放的文件的属性信息
*/

int main()
{
	FILE *myfile; 
	//打开文件
	myfile=fopen("./1.txt","a+");   //a+追加
	if(myfile==NULL)
	{
		perror("打开文件失败了\n");
		return -1;
	}
	
	//读写：从末尾开始
	return 0;
}