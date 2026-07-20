#include "myhead.h"

/*
	1.三个函数长得很类似。但是功能完全不同
	     scanf    读取键盘输入的数据
		 sscanf   拆分字符串
		 fscanf   先读取文件内容，然后把读取的内容拆分成数据存放到各个变量地址中
*/

int main(int argc,char **argv)
{
	int y,m,d;
	
	FILE *myfile;
	//打开文件
	myfile=fopen("./1.txt","r+");
	if(myfile==NULL)
	{
		perror("打开文件失败了!\n");
		return -1;
	}
	
	//要求：你把记事本中年月日读取，分别存放到y,m,d三个整型变量的地址中
	/*
		%d/%d/%d 理由是记事本存放的格式就是 2026/7/17
		fscanf干了两件事情
		   第一：读取文件中的数据
		   第二：把读取的数据拆分成数据，分别存放到y,m,d三个变量的地址中
	*/
	fscanf(myfile,"%d/%d/%d",&y,&m,&d);
	printf("年，月，日分别是: %d   %d   %d\n",y,m,d);
	
	//关闭文件
	fclose(myfile);
	return 0;
	
}