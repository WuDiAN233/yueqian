#include "myhead.h"

/*
	#include <sys/ioctl.h>
	int ioctl(int fd, unsigned long request, ...);
	   参数：fd --》你要控制的那个硬件设备的文件描述符
	         request --》你要给这个硬件设备发送什么命令
			             命令：跟驱动源码有关，驱动的源码中有写明发送什么命令
*/
#define TEST_MAGIC 'x'                    //定义幻数

#define LED1 _IO(TEST_MAGIC, 0)              
#define LED2 _IO(TEST_MAGIC, 1)
#define LED3 _IO(TEST_MAGIC, 2)
#define LED4 _IO(TEST_MAGIC, 3) 

#define LED_ON  	0	//灯亮
#define LED_OFF		1   //灯灭

int main()
{
	//1.在程序执行前必须确保你的驱动已经安装驱动
	//2.访问驱动设备
	int fd = open("/dev/Led",O_RDWR);
	
	//3.控制设备状态
	while(1)
	{
		ioctl(fd,LED1,LED_ON); //给第一盏灯发送开启命令
		sleep(1);
		ioctl(fd,LED1,LED_OFF);//给第一盏灯发送熄灭命令
		sleep(1);
	}
	
	//4. 关闭设备文件
	close(fd);
	return 0;
}