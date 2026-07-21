#include "myhead.h"

/*
    memory  map(地图，映射)
	1.mmap内存映射提高显示图片的效率
	  显示图片
	      方法1:write写入数据，但是效率不高
		  方法2:mmap效率最高
		  
*/
int main()
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
	
	//打开你要显示的24位bmp图片
	/*
		液晶屏的分辨率800*480
		我们打算全屏显示bmp图片，图片也弄成800*480大小
	*/
	bmpfd=open("/1.bmp",O_RDWR);
	if(bmpfd==-1)
	{
		perror("打开bmp图片失败了\n");
		return -1;
	}
	
	//用lseek跳过bmp图片最前面的54字节头信息
	lseek(bmpfd,54,SEEK_SET);
	
	//读取bmp图片真实的颜色数据,从55字节开始读取
	read(bmpfd,bmpbuf,800*480*3);
	
	/*
		重点：研究图片像素点颜色数据排列顺序(编码特点)
		   每三个字节为一组，存放一个像素点颜色值
		   bmpbuf[0]   B
		   bmpbuf[1]   G
		   bmpbuf[2]   R
		   bmpbuf[3]   B
		   bmpbuf[4]   G
		   bmpbuf[5]   R
		   开发板是linux系统，采用小端序存放
	*/
	
	//把3个字节的BGR转换成4个字节的ARGB
	/*
		bmpbuf[0]   B
		bmpbuf[1]   G
		bmpbuf[2]   R
		透明度A(用0表示,bmp图片没有透明度，A随便设置成多少都可以)最高字节
		     0x00<<24|bmpbuf[2]<<16|bmpbuf[1]<<8|bmpbuf[0]   
	*/
	for(i=0; i<800*480; i++)
		lcdbuf[i]=0x00<<24|bmpbuf[3*i+2]<<16|bmpbuf[3*i+1]<<8|bmpbuf[3*i+0];
	
	//把转换后的颜色写入到液晶屏
	//解决图片翻转的问题
	/*
		lcdbuf[800*0]到lcdbuf[800-1]       第一行图片颜色数据
             写入到液晶屏最后一行的位置
		lcdbuf[800*1]到lcdbuf[800*2-1]     第二行图片颜色数据
		     写入到液晶屏倒数第二行的位置 
		lcdbuf[800*2]到lcdbuf[800*3-1]     第三行图片颜色数据
		     写入到液晶屏倒数第三行的位置 
	*/
	for(i=0; i<480; i++) //图片总共480行
	{
		//使用内存拷贝的函数，直接把像素点颜色值拷贝到液晶屏地址
		//不再使用write，write效率很低
		memcpy(lcdmem+800*(479-i),&lcdbuf[800*i],800*4);
	}
	
	//关闭
	close(lcdfd);
	close(bmpfd);
	
	//解除内存映射
	munmap(lcdmem,800*480*4);
	return 0;
}