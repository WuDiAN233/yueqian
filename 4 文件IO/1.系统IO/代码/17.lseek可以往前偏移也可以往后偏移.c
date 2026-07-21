#include "myhead.h"

/*
	lseek第二个参数
	  1.如果是正数--》表示往后偏移
	    如果是负数--》表示往前偏移
		起始位置只能往后偏移，不能往前偏移
		末尾位置可以往前，也可以往后
	  2.如果你在windows系统上新建一个记事本，内容是hellogec，记事本大小就是hellogec的大小(八个字节)
	    如果你在linux系统上用vim新建一个记事本，内容是hellogec，记事本大小就是hellogec的大小+回车(九个字节)
	  3.SEEK_CUR表示当前位置，当前位置随着read或者write发生改变 
*/ 
int main()
{
	int fd;
	char buf[10]={0};
	//打开记事本
	fd=open("./1.txt",O_RDWR);
	if(fd==-1)
	{
		printf("文件打开失败了\n");
		return -1;
	}
	
	//有实际意义
	//lseek(fd,5,SEEK_SET); //起始位置往后偏移5个字节
	//没有实际意义，此时光标偏移依然在起始位置
	//lseek(fd,-5,SEEK_SET); //起始位置往前偏移5个字节
	
	//有实际意义：此时会在文件的末尾空出五个字节的位置，然后写入内容
	//lseek(fd,5,SEEK_END); //末尾位置往后偏移5个字节
	//有实际意义:最多不能超越起始位置
	//lseek(fd,-3,SEEK_END); //末尾位置往前偏移3个字节
	
	read(fd,buf,5);
	printf("buf is: %s\n",buf);
	//关闭文件
	close(fd);
	return 0;
}

