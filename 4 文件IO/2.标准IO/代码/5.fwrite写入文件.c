#include "myhead.h"

/*
	fwrite写入文件
	  1.fwrite返回值：
	       成功写入的完整数据块个数
		   失败 -1
	  2.初学者容易犯错的地方
	     fwrite跟write一样恶心：如果你要写入的字节数，你数量写多了，它们两个都会找垃圾数凑够你需要的数量
*/

int main()
{
	FILE *myfile; 
	size_t ret;
	//打开文件
	myfile=fopen("./1.txt","r+");   //可读写，不会新建文件
	if(myfile==NULL)
	{
		perror("打开文件失败了\n");
		return -1;
	}
	
	//写入内容到文件中:字符串总的长度是12个字节
	//ret=fwrite("nihaoyueqian",4,3,myfile);
	//ret=fwrite("nihaoyueqian",3,4,myfile);
	ret=fwrite("nihaoyueqian",12,1,myfile);
	//ret=fwrite("nihaoyueqian",1,12,myfile);
	//ret=fwrite("nihaoyueqian",2,6,myfile);
	//ret=fwrite("nihaoyueqian",6,2,myfile);
	printf("fwrite的返回值表示写入的完整数据块个数: %ld\n",ret);
	
	//关闭文件
	fclose(myfile);
	return 0;
}