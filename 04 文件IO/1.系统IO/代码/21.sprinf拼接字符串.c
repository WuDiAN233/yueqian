#include "myhead.h"

/*
   sprintf拼接字符串    
*/ 
int main()
{
	int n1=2026;
	float n2=66.6;
	char n3[50]="祝大家新年快乐";
	
	//定义数组存放拼接之后的结果
	char result[100]={0};
	
	//请你把不同类型的三个数据n1，n2，n3按照指定格式拼接成字符串
	//sprintf(result,"%s，%d，每天%f",n3,n1,n2);
	sprintf(result,"%d，每天%f,%s",n1,n2,n3);
	printf("result is：  %s\n",result);
	
	return 0;
}

