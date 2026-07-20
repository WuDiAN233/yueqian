#include "myhead.h"

/*
	液晶屏显示红色
*/

int main(int argc,char **argv)
{
	int lcdfd;
	int i;
	//定义变量表示红色
	//int数据总共四个字节，从高到低分别代表A  R  G  B
	int red=0x00ff0000; //红色
	
	//打开液晶屏的驱动
	lcdfd=open("/dev/fb0",O_RDWR);
	if(lcdfd==-1)
	{
		perror("打开液晶屏失败了\n");
		return -1;
	}
	
	//写入红色到液晶屏中
	//液晶屏分辨率是800*480
	for(i=0; i<800*2; i++) //把最前面两行填充成红色
		write(lcdfd,&red,4);
	
	//关闭文件
	close(lcdfd);
	return 0;
}