#include "myhead.h"

/*
	标准IO打开文件
	  1.r+不可以新建文件
	    w+文件不存在，可以新建文件
	  2.系统IO，open返回文件描述符
	    标准IO，fopen返回结构体指针，存放的文件的属性信息
*/

int main()
{
	FILE *myfile; 
	//打开文件
	//myfile=fopen("./1.txt","r+"); //r+不能新建文件
	myfile=fopen("./1.txt","w+");   //w+具备新建功能
	if(myfile==NULL)
	{
		perror("打开文件失败了\n");
		return -1;
	}
	
	
	return 0;
}