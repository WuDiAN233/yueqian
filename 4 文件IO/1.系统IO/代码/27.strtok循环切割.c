#include "myhead.h"

/*
   strtok切割字符串神器
      1.strtok第一次切割需要传递原始字符串的首地址
	    从第二次切割开始，不需要传递原始字符串的首地址(第一次切割strtok已经纪录了原始字符串地址)
	  2.切割标准可以写多个字符
*/ 
int main()
{
	char time[50]="2026/7/15 15:40:32 PM";
	//请你把年月日，时分秒，PM/AM全部拆分出来
	//方法1：sscanf
	//方法2：strtok
	char *p=strtok(time,"/ :"); //以 / :三个字符作为切割标准来切割time
	printf("第一次切割，结果是: %s\n",p);
	
	//循环切割
	while(1)
	{
		p=strtok(NULL,"/ :");
		if(p==NULL)
			break;
		printf("后面切割，结果是: %s\n",p);
	}
	return 0;
}

