#include "ts.h"

/*
	把触摸屏的代码封装成函数
*/ 
int tsfd; //触摸屏的文件描述符，定义成全局变量，任意位置都可以使用

//触摸屏打开
int ts_open()
{
	//第一步：打开硬件设备的驱动文件
	tsfd=open("/dev/input/event0",O_RDWR);
	if(tsfd==-1)
	{
		perror("打开触摸屏驱动失败了!\n");
		return -1;
	}
	return 0;
}

//读取触摸屏的坐标
int ts_get_pos(int *x,int *y)
{
	//定义输入子系统模型需要的结构体变量，这个结构体用来存放x，y坐标
	struct input_event myevent;
	//经典编程技巧：标志位
	int count=0;
	
	while(1)
	{
		//读取你点击位置的触摸屏坐标
		read(tsfd,&myevent,sizeof(myevent));
		//判断事件类型
		if(myevent.type==EV_ABS) //触摸屏事件
		{
			//进一步判断，你现在读取的是x坐标，还是y坐标
			if(myevent.code==ABS_X) //x坐标
			{
				*x=myevent.value; //保存x坐标
				count++;
			}
			if(myevent.code==ABS_Y) //y坐标
			{
				*y=myevent.value; //保存y坐标
				count++;
			}
			if(count==2)
				break; //退出死循环
		}
	}
	return 0;
}

//关闭触摸屏
int ts_close()
{
	//关闭触摸屏
	close(tsfd);
	return 0;
}



