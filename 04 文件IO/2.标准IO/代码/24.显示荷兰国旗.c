#include "myhead.h"

/*
	液晶屏显示荷兰国旗
	   1.液晶屏大家也可以把它当前成一个记事本类比理解
	     
*/

int main(int argc,char **argv)
{
	int lcdfd;
	int i;
	//定义变量表示红色
	//int数据总共四个字节，从高到低分别代表A  R  G  B
	int red=0x00ff0000;   //红色
	int white=0x00ffffff; //白色
	int blue=0x000000ff;  //蓝色
	
	//打开液晶屏的驱动
	lcdfd=open("/dev/fb0",O_RDWR);
	if(lcdfd==-1)
	{
		perror("打开液晶屏失败了\n");
		return -1;
	}
	
	//最前面160行填充红色
	//液晶屏分辨率是800*480
	for(i=0; i<800*160; i++) 
		write(lcdfd,&red,4);
	
	//中间160行填充白色
	for(i=0; i<800*160; i++) 
		write(lcdfd,&white,4);
	
	//最后160行填充蓝色
	for(i=0; i<800*160; i++) 
		write(lcdfd,&blue,4);
	
	//关闭文件
	close(lcdfd);
	return 0;
}