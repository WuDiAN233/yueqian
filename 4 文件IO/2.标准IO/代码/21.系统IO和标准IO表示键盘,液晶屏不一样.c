#include "myhead.h"

/*
	scanf("%d",&n1);
	   scanf会从缓冲区读取键盘输入的数据(都是当成字符串存放的)
	   scanf会依据你使用格式控制符%d，scanf会把字符串转成整数，然后存放到n1的地址中
	read这个函数，读取键盘输入，不具备类型转换功能，所有的数据都必须以字符串的形式保存才可以
*/

int main(int argc,char **argv)
{
	//平时获取键盘输入的数据
	//scanf()
	
	//int n1;
	//char n2[10];
	//float n3;
	//printf("请输入一个小数\n");
	//我要读取键盘输入的数据，存放到n1的地址中
	//read(0,&n1,sizeof(n1)); //效果等价于scanf("%d",&n1)
	//read(0,n2,5);  //等价于read(STDIN_FILENO,n2,5);
	//read(STDIN_FILENO,n2,5);
	//read(STDIN_FILENO,&n3,sizeof(n3));
	//printf("你刚才输入的数据是: %f\n",n3);
	
	
	//char n4[10]={0};
	//printf("请输入一个整数\n");
	//read(0,n4,2);
	//printf("n4 is: %s\n",n4);
	//C语言学过一个函数叫做atoi()可以把字符串转成整数
	//int atoi(const char *nptr);
	//int n5=atoi(n4); //把字符串转成整数
	//printf("n5 is: %d\n",n5);
	
	//用标准IO读取键盘输入
	char n6[20]={0};
	printf("请输入一个字符串\n");
	fread(n6,5,1,stdin); //stdin是标准IO用来表示键盘的
	printf("你刚才输入的数据是: %s\n",n6);
	
	return 0;
}