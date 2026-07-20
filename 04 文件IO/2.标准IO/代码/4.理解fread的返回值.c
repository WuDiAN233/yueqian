#include "myhead.h"

/*
	理解fread的返回值
	   1.fread和read的不同点
	       read直接按照指定的字节数读取文件
		   read的返回值表示成功读取到的字节数
		   fread按照指定的数据块大小，数据块个数来读取文件
		   fread的返回值表示成功读取的完整数据块个数
	   2.fread和read的相同点
	       假设文件总共10个字节
	       read(fd,buf,200) //第三个参数你写多了，此时按照实际大小读取并返回实际读取的字节数
		   fread(buf,20,100,myfile); //第二，第三个参数写多了，此时按照实际大小读取
       3.假设1.txt最开始内容是hellogec
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
	
	//读取文件内容
	//ret=fread(buf,20,1,myfile); //读取1个数据块，每个数据块大小是20个字节
	ret=fread(buf,7,5,myfile);   //读取5个数据块，每个数据块大小是7个字节
	//ret=fread(buf,9,2,myfile);   //读取2个数据块，每个数据块大小是9个字节
	printf("读取的内容是: %s,返回值是： %ld\n",buf,ret);
	
	//关闭文件
	fclose(myfile);
	return 0;
}