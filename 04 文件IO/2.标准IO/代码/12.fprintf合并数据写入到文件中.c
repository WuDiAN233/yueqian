#include "myhead.h"

/*
	1.三个函数长得很类似。但是功能完全不同
	     printf   打印各种数据
		 sprintf  拼接字符串
		 fprintf  把各种数据先拼接成字符串，然后把这个字符串写入到指定文件中
*/

int main(int argc,char **argv)
{
	int ret;
	int n1=2026;
	float n2=666.6;
	char n3[50]="马年吉祥";

	FILE *myfile;
	//打开文件
	myfile=fopen("./1.txt","r+");
	if(myfile==NULL)
	{
		perror("打开文件失败了!\n");
		return -1;
	}
	
	//拼接数据写入到1.txt
	fprintf(myfile,"祝大家%s，今年是%d年，每天%f",n3,n1,n2);
	

	//关闭文件
	fclose(myfile);
	return 0;
	
}