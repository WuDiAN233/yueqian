#include "myhead.h"
#include <stdlib.h>
/*
	创建多个子进程
	   
*/
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
			while(1)
			{
				int system("madplay /1.mp3");
			}
		}
		else if(id2>0) //父进程
		{
			while(1)
			{
				printf("我是父进程，我负责安装驱动,我的ID是: %d\n",getpid());
				sleep(1);
			}
		}
	}
	else if(id1==0) //第一个子进程
	{
		while(1)
		{
			int lcdfd;
			int bmpfd;
			int i;
			//定义数组存放读取的颜色数据
			char bmpbuf[800*480*3];
			//定义数组存放转换得到的ARGB数据
			int lcdbuf[800*480];
			//定义指针存放液晶屏的首地址
			int *lcdmem;
			
			//打开液晶屏的驱动
			lcdfd=open("/dev/fb0",O_RDWR);
			if(lcdfd==-1)
			{
				perror("打开液晶屏失败了\n");
				return -1;
			}
			
			//使用内存映射--》得到液晶屏在内存中的首地址
			lcdmem=mmap(NULL,800*480*4,PROT_READ|PROT_WRITE,MAP_SHARED,lcdfd,0);
			if(lcdmem==NULL)
			{
				perror("映射液晶屏首地址失败了\n");
				return -1;
			}

			bmpfd=open("/.bmp",O_RDWR);
			if(bmpfd==-1)
			{
				perror("打开bmp图片失败了\n");
				return -1;
			}
			
			lseek(bmpfd,54,SEEK_SET);
			
			read(bmpfd,bmpbuf,800*480*3);
			
			for(i=0; i<800*480; i++)
				lcdbuf[i]=0x00<<24|bmpbuf[3*i+2]<<16|bmpbuf[3*i+1]<<8|bmpbuf[3*i+0];

			*/
			for(i=0; i<480; i++) //图片总共480行
			{
				memcpy(lcdmem+800*(479-i),&lcdbuf[800*i],800*4);
			}
			sleep(1);
		}
	}
	
	//防止主程序退出
	while(1)
	{
		
	}
	close(lcdfd);
	close(bmpfd);
	
	//解除内存映射
	munmap(lcdmem,800*480*4);
	return 0;
}