#include "myhead.h"

/*
	fread读取文件
	  1.第二个参数表示每个数据块大小
	    第三个参数表示数据块个数
	  2.fread(buf,10,1,myfile); //读取1个数据块，每个数据块大小是10个字节
	    fread(buf,1,10,myfile);   //读取10个数据块，每个数据块大小是1个字节
	    fread(buf,2,5,myfile); 
		fread(buf,5,2,myfile);
		四种写法，最终都是读取了10个字节的数据(总数)，有啥区别
		答案：区别就是fread的返回值含义不一样
	    fread的返回值：
		    成功：返回成功读取到的完整数据块个数
			失败：-1
*/

int main()
{
	FILE *myfile; 
	size_t ret;
	char buf[20]={0};
	//打开文件
	myfile=fopen("./1.txt","r+");   //可读写，不会新建文件
	if(myfile==NULL)
	{
		perror("打开文件失败了\n");
		return -1;
	}
	
	//读取文件内容：我打算总共读取10个字节的数据
	//ret=fread(buf,10,1,myfile); //读取1个数据块，每个数据块大小是10个字节
	//ret=fread(buf,1,10,myfile);   //读取10个数据块，每个数据块大小是1个字节
	//ret=fread(buf,2,5,myfile);   //读取5个数据块，每个数据块大小是2个字节
	ret=fread(buf,5,2,myfile);   //读取2个数据块，每个数据块大小是5个字节
	printf("读取的内容是: %s,返回值是： %ld\n",buf,ret);
	
	//关闭文件
	fclose(myfile);
	return 0;
}