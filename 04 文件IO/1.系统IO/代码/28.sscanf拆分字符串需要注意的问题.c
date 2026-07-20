#include "myhead.h"

/*
	sscanf拆分字符串需要注意的问题
	   %s 表示字符串
*/ 
int main()
{
	char msg[50]="张三@123456";
	char name[20]={0};
	char passwd[20]={0};
	
	//要求你把账号和密码切割出来
	//sscanf(msg,"%s@%s",name,passwd);
	
	//解决方法：
	//方法1：使用C语言位宽  %10s
	//不好的地方：
	//sscanf(msg,"%6s@%s",name,passwd);
	//方法2：可以特殊符号匹配切割截止符
	//%[^@]表示要求sscanf遇到@符号停止拆分
	sscanf(msg,"%[^@]@%s",name,passwd);
	
	printf("账号: %s  密码: %s\n",name,passwd);
	return 0;
}

