#include "myhead.h"

/*
   键盘输入用户名和密码，拼接存放到记事本中
*/ 
int main()
{
	int fd;
	char name[20]={0};
	char passwd[10]={0};
	char result[50]={0};
	
	printf("请输入要注册的用户名和密码!\n");
	scanf("%s",name);
	scanf("%s",passwd);
	
	//拼接字符串，存放到记事本
	sprintf(result,"%s@%s\n",name,passwd);
	
	//打开记事本
	fd=open("msg.txt",O_RDWR);
	if(fd==-1)
	{
		perror("打开记事本失败了\n");
		return -1;
	}

	//把读写偏移移动到末尾位置
	lseek(fd,0,SEEK_END);
	
	//写入到记事本:写入到末尾了
	write(fd,result,strlen(result));
	
	close(fd);
	
	return 0;
}

