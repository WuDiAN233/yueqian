#include "myhead.h"

/*
	系统IO实际应用，读取触摸屏坐标
	   1.任何硬件设备，你要操作它，都是第一步打开驱动文件
	   2.触摸屏的驱动采用输入子系统模型写的，读取触摸屏的坐标，就必须了解输入子系统模型是什么
	     输入子系统模型：学习linux驱动开发，需要学习的知识点，现在读取坐标只需要了解输入子系统模型是什么，用来读取坐标即可 
	   3.强化：linux中一切都是文件这个观点
*/ 
int main()
{
	int tsfd;
	//定义输入子系统模型需要的结构体变量，这个结构体用来存放x，y坐标
	struct input_event myevent;
	
	//第一步：打开硬件设备的驱动文件
	tsfd=open("/dev/input/event0",O_RDWR);
	if(tsfd==-1)
	{
		perror("打开触摸屏驱动失败了!\n");
		return -1;
	}
	
	while(1)
	{
		//第二步：读取你点击位置的触摸屏坐标
		read(tsfd,&myevent,sizeof(myevent));
		//判断事件类型
		if(myevent.type==EV_ABS) //触摸屏事件
		{
			//进一步判断，你现在读取的是x坐标，还是y坐标
			if(myevent.code==ABS_X) //x坐标
			{
				//打印坐标值
				printf("你点击触摸屏，x坐标: %d\n",myevent.value);
			}
			if(myevent.code==ABS_Y) //y坐标
			{
				//打印坐标值
				printf("你点击触摸屏，y坐标: %d\n",myevent.value);
			}
		}
	}
	
	//第三步：关闭触摸屏
	close(tsfd);
	return 0;
}

