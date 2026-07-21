#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

/*
	open函数有两个功能
	1.功能
	 功能1：打开已经存在的文件
	 功能2：新建文件
	2.文件描述符的理解
	     通俗理解：1.txt打开成功，别名就用fd表示
		 专业理解：文件描述符代表你打开的文件在结构体数组中的下标位置
	3.大家如果使用vscode写文件的IO的代码，vscode会提示#include <sys/types.h> #include <sys/stat.h> #include <fcntl.h>这些头文件找不到，原因是vscode环境变量无法识别linux系统的头文件
	4.windows跟linux路径写法的差别
	      windows系统中的路径名:  盘符:\路径名
		                          比如： C:\YUEQINA\1.txt    \叫做路径分隔符
		  linux系统的路径名：     /mnt/hgfs/GZ2605/代码/1.txt      /叫做路径分隔符
		  ubuntu输入pwd命令，可以查看文件的绝对路径
    5.ubuntu中文编码必须是utf8才可以正常显示
	6.文件描述符从0开始，0  1  2三个文件描述符被标准输入(键盘)，标准输出(液晶屏)，标准错误输出(液晶屏)占用
	                     液晶屏占用了两个文件描述符
						 拓展(做了解，超纲内容)：液晶屏为什么要占用两个文件描述符
*/ 
int main()
{
	int fd;
	int fd1;
	//打开已经存在的文件
	//文件路径名：写绝对路径(完整的路径名)，也可以写相对路径
	/*
		"C:\Users\Administrator\Desktop\GZ2605\代码\1.txt" -->属于windows系统中绝对路径，不正确
		理由：我们现在学习的open函数，属于linux操作系统提供，只能在ubuntu或者WSL(最小子系统)上使用
		正确答案：必须写linux系统中文件的路径名，不可以写windows系统的，无法识别windows系统中的文件路径名
	*/
	//fd=open("C:\Users\Administrator\Desktop\GZ2605\代码\1.txt",O_RDWR);  //错误示范
	//fd=open("/mnt/hgfs/GZ2605/代码/1.txt",O_RDWR);  //ubuntu上的绝对路径
	fd=open("./1.txt",O_RDWR);   //.叫做当前路径，我要打开当前路径下的1.txt
	//fd=open("1.txt",O_RDWR);     // 偷懒的写法，直接写文件名，默认表示当前路径下的1.txt
	if(fd==-1)
	{
		printf("打开文件失败了\n");
		return -1;
	}
	
	printf("彭老师打开的第一个文件，它的文件描述符(本质就是结构体数组的下标位置)是: %d\n",fd);
	
	//再打开一个文件
	fd1=open("2.txt",O_RDWR);
	if(fd1==-1)
	{
		printf("打开文件失败了\n");
		return -1;
	}
	
	printf("彭老师打开的第二个文件，它的文件描述符(本质就是结构体数组的下标位置)是: %d\n",fd1);
	return 0;
}

