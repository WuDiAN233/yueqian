#include "myhead.h"

/*
	fseek用于标准IO设置读写偏移
	lseek用于系统IO设置读写偏移
	   区别：lseek返回值代表当前偏移位置距离起始位置的字节数
	         fseek返回值成功0，失败-1
*/

int main(int argc,char **argv)
{
	int ret;
	FILE *myfile;
	//追加打开文件：打开之后，文件的偏移到了末尾位置
	myfile=fopen("./1.txt","a+");
	if(myfile==NULL)
	{
		perror("打开文件失败了!\n");
		return -1;
	}
	
	//移动读写偏移
	ret=fseek(myfile,8,SEEK_SET); 
	printf("fseek成功，返回值: %d\n",ret);
	
	//为了弥补fseek返回值的不足，linux提供了ftell这个函数获取当前位置距离起始位置的字节数
	printf("当前读写偏移位置距离起始位置的字节数: %ld\n",ftell(myfile));

	//关闭文件
	fclose(myfile);
	return 0;
	
}