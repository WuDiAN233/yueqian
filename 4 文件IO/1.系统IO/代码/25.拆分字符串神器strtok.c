#include "myhead.h"

/*
   strtok切割字符串神器
*/ 
int main()
{
	//char time[50]="2026/7/15 15:40:32 PM";
	char time[50]="2026/7/15";
	//请你把年月日，时分秒，PM/AM全部拆分出来
	//方法1：sscanf
	//方法2：strtok
	char *p=strtok(time,"/"); //以 /作为切割标准来切割time
	printf("第一次切割，结果是: %s\n",p);
	
	p=strtok(NULL,"/");
	printf("第二次切割，结果是: %s\n",p);
	
	p=strtok(NULL,"/");
	printf("第三次切割，结果是: %s\n",p);
	return 0;
}

