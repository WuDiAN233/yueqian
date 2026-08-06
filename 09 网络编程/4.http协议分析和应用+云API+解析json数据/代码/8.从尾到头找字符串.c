#include "myhead.h"

int main()
{
	char buf[100]="yhhhyhjjjjjkkpkkp";
	
	//char *ret=strchr(buf,'p'); //从头到尾找
	char *ret=strrchr(buf,'p');  //从尾到头找
	printf("ret指向的字符串首地址是: %s\n",ret);
}