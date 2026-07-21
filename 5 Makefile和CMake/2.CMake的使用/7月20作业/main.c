#include "siglecirclelist.h" //链表的头文件
#include "bmp.h" 
#include "ts.h"

/*
	封装函数实现递归读取目录
*/

struct siglelist *myhead; //头节点

//封装函数递归读取目录
int myreaddir(char *dirpath)
{
	DIR *mydir;
	struct dirent *mydirent;
	char allpath[300]; //存放绝对路径
	//打开目录
	mydir=opendir(dirpath);
	if(mydir==NULL)
	{
		perror("打开目录失败了\n");
		return -1;
	}
	
	//循环读取目录中的内容
	while((mydirent=readdir(mydir))!=NULL)
	{
		//排除掉.和..
		if(strcmp(mydirent->d_name,".")==0 || strcmp(mydirent->d_name,"..")==0)
			continue;
		
		if(mydirent->d_type==DT_DIR) //子目录
		{
			bzero(allpath,300);
			//拼接得到子目录的绝对路径
			sprintf(allpath,"%s/%s",dirpath,mydirent->d_name);
			//递归读取子目录
			myreaddir(allpath);
		}
		if(mydirent->d_type==DT_REG) //普通文件
		{
			bzero(allpath,300);
			//判断这个普通文件是不是.bmp图片
			char *p=strstr(mydirent->d_name,".bmp");
			if(p!=NULL)  //说明.bmp在文件名字中有出现
			{
				//拼接得到普通文件的绝对路径
				sprintf(allpath,"%s/%s",dirpath,mydirent->d_name);
				//尾插到链表中
				list_insert_tail(allpath,myhead);
			}			
		}
	}
	//关闭文件夹
	closedir(mydir);
	return 0;
}

int main(int argc,char **argv)
{
	//判断主函数参数个数
	if(argc<2)
	{
		printf("对不起，您忘记传递文件夹路径名了\n");
		return -1;
	}
	//定义变量存放触摸屏坐标
	int ts_x,ts_y;
	//初始化头节点
	myhead=list_init();
	
	//打开触摸屏
	ts_open();
	
	//递归读取某个目录,把所有的bmp图片路径名存放到链表
	myreaddir(argv[1]);
	
	//最开始显示第一张图片(头节点没有存放数据，第一张就是头节点的下一个)
	//定义指针遍历链表
	struct siglelist *curp=myhead;
	curp=curp->next; //往后挪动指针
	show_bmp(curp->bmppath); //显示第一张图片
	
	//循环读取触摸屏坐标，判断用户点击是左半边还是右半边
	while(1)
	{
		ts_get_pos(&ts_x,&ts_y);
		/*
			判断坐标:
			  黑色边框：1024*600
			  蓝色边框：800*480
		*/
		if(ts_x>0 && ts_x<512) //左半边
		{
			//判断curp是否挪到头节点(我使用的是双向链表，不是双向循环链表)
			if(curp->prev==myhead)
			{
				printf("对不起，已经到了第一张图片的位置，无法上一张\n");
				show_bmp(curp->bmppath); 
			}
			else
			{
				curp=curp->prev; //往前挪动指针
				show_bmp(curp->bmppath); //显示上一张
			}		
		}
		if(ts_x>512 && ts_x<1024) //右半边，下一张
		{
			//判断curp是否挪到末尾(我使用的是双向链表，不是双向循环链表)
			if(curp->next==NULL)
			{
				printf("对不起，已经到了最后一张图片的位置，无法下一张\n");
				show_bmp(curp->bmppath); 
			}
			else
			{
				curp=curp->next; //往后挪动指针
				show_bmp(curp->bmppath); //显示下一张
			}		
		}
	}
	
	//关闭触摸屏
	ts_close();
	return 0;
}