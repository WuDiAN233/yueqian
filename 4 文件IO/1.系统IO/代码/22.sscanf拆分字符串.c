#include "myhead.h"

/*
   sscanf拆分字符串 
*/ 
int main()
{
	int y,m,d;
	//char date[20]="2026-7-15";
	char date[20]="2026/7/15";
	
	//请你把年月日拆分出来
	//sscanf把date存放的字符串拆分，按照"%d-%d-%d"格式拆分
	//由于原始数据，年月日用-隔开的，因此我代码的格式也是用-隔开
	//把年拆分存放大&y  把月拆分存放&m   把日拆分存放到&d
	//sscanf(date,"%d-%d-%d",&y,&m,&d);
	sscanf(date,"%d/%d/%d",&y,&m,&d);
	printf("拆分出来的结果:  %d  %d   %d\n",y,m,d);
	return 0;
}

