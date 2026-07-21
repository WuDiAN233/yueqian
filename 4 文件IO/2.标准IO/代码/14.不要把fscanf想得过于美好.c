#include "myhead.h"

/*
	fscanf把记事本中存放的一行行用户名和密码解析出来
	    %[^@]表示告诉fscanf函数遇到@字符就停止拆分
*/

int main(int argc,char **argv)
{
	char name[30]={0};
	char passwd[30]={0};
	FILE *myfile;
	//打开文件
	myfile=fopen("./1.txt","r+");
	if(myfile==NULL)
	{
		perror("打开文件失败了!\n");
		return -1;
	}
	
	fscanf(myfile,"%[^@]@%s",name,passwd);
	printf("用户名: %s   密码: %s\n",name,passwd);
	
	fscanf(myfile,"%[^@]@%s",name,passwd);
	printf("用户名: %s   密码: %s\n",name,passwd);
	
	fscanf(myfile,"%[^@]@%s",name,passwd);
	printf("用户名: %s   密码: %s\n",name,passwd);
	
	//关闭文件
	fclose(myfile);
	return 0;
	
}