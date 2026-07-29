#include "myhead.h"

/*
	两个子进程
	  一个显示图片
	  一个播放音乐
*/
extern int show_bmp(char *bmppath);

//定义二维数组存放图片路径名
char bmppath[5][20]={
	"/otherspace/1.bmp",
	"/otherspace/2.bmp",
	"/otherspace/3.bmp",
	"/otherspace/4.bmp",
	"/otherspace/5.bmp"
};

int main() //本身编译运行就是一个独立的进程--》叫做父进程 
{
	pid_t id1,id2;
	
	//创建第一个子进程
	id1=fork();
	if(id1>0) //父进程
	{
		//创建第二个子进程
		id2=fork();
		if(id2==0) //第二个子进程
		{
			//播放音乐
			system("madplay /otherspace/1.mp3");
		}
	}
	else if(id1==0) //第一个子进程
	{
		int n=0;
		while(1)
		{
			show_bmp(bmppath[n]);
			//由于我只有五张图片，n不可以无限制的++
			if(n<4)
				n++;
			else
				n=0;
			sleep(1);
		}
	}
	
	//阻塞父进程，等待回收子进程
	wait(NULL);
	wait(NULL);
	return 0;
}