#include "myhead.h"

int main(int argc,char **argv)
{
	int lcdfd;
	int bmpfd;
	int i;
	DIR *mydir;
	struct dirent*mydirent;
	//定义数组存放读取的颜色数据
	char bmpbuf[800*480*3];
	//定义数组存放转换得到的ARGB数据
	int lcdbuf[800*480];
	char *strstr(const char *haystack, const char *needle);	
	//打开液晶屏的驱动
	lcdfd=open("/dev/fb0",O_RDWR);
	if(lcdfd==-1)
	{
		perror("打开液晶屏失败了\n");
		return -1;
	}

	mydir=opendir(".");
	if(mydir==NULL)
	{
		perror("打开目录失败了\n");
		return -1;
	}
	//readdir自动把所有的.bmp读取保存链表
	while(1)
	{
		mydirent=readdir(mydir);
		if(mydirent==NULL)
			break;
		if(strstr(mydirent->d_name,".bmp")!=NULL)
			printf("目前你读取的目录中包含的bmp图片是: %s\n",mydirent->d_name);
	}
	//打开你要显示的24位bmp图片
	bmpfd=open("/*.bmp",O_RDWR);
	if(bmpfd==-1)
	{
		perror("打开bmp图片失败了\n");
		return -1;
	}
	
	//用lseek跳过bmp图片最前面的54字节头信息
	lseek(bmpfd,54,SEEK_SET);
	
	//读取bmp图片真实的颜色数据,从55字节开始读取
	read(bmpfd,bmpbuf,800*480*3);
	for(i=0; i<800*480; i++)
		lcdbuf[i]=0x00<<24|bmpbuf[3*i+2]<<16|bmpbuf[3*i+1]<<8|bmpbuf[3*i+0];
	//把转换后的颜色写入到液晶屏
	write(lcdfd,lcdbuf,800*480*4);
	
	//关闭
	close(lcdfd);
	close(bmpfd);
	return 0;
}