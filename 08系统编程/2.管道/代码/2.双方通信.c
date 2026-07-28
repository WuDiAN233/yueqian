#include "myhead.h"
#include<stdio.h>
#include<stdlib.h>

/*
	目前：父子进程想要收发信息，收发数据，不可以
	使用无名管道在父子进程直接搭建沟通的桥梁
                 
*/
int main() //本身编译运行就是一个独立的进程--》叫做父进程 
{
	pid_t id;
	int ret1;
	int ret2;
	char buf1[100];
	char buf2[100];
	//定义数组存放无名管道读端和写端的文件描述符
	int fd1[2];
	int fd2[2];
	//第一步：创建无名管道
	ret1=pipe(fd1);
	ret2=pipe(fd2);
	if(ret1==-1&&ret2==-1)
	{
		perror("创建无名管道失败了\n");
		return -1;
	}
	
	//创建一个子进程
	id=fork(); //产生新的进程(子进程)，总共就有两个进程产生
	if(id>0)  //父进程 
	{
		while(1)
		{
			bzero(buf1,100);
			bzero(buf2,100);
			printf("请输入要发送给子进程的信息!\n");
			scanf("%s",buf1);
			//写入数据到无名管道
			write(fd1[1],buf1,strlen(buf1));
			if(strcmp(buf1,"q")==0)goto _q1;
			read(fd2[0],buf2,100);
			printf("父进程读取到的信息是: %s\n",buf2);
		}
	}
	else if(id==0) //子进程，把父进程的变量id拷贝了一份，名字跟父进程的是一样，但是相互独立
	{
		while(1)
		{
			bzero(buf1,100);
			bzero(buf2,100);
			//读取无名管道中的数据
			//read的第三个不必纠结，假设你写是100个字节，无名管道有多少数据，read就读取多少
			//如果没有进程写入数据到无名管道，read会一直阻塞
			read(fd1[0],buf1,100);
			printf("子进程读取到的信息是: %s\n",buf1);
			printf("子进程回复: \n");
			scanf("%s",buf2);
			//写入数据到无名管道
			if(strcmp(buf2,"q")==0)goto _q1;
			write(fd2[1],buf2,strlen(buf2));
		}
		exit(0);
	}
	else
	{
		perror("创建子进程失败了\n");
		return -1;
	}
_q1:
	wait(NULL); //参数NULL表示父进程只负责回收子进程，但是子进程的退出状态信息不关心
	//关闭无名管道
	close(fd1[0]);
	close(fd1[1]);
	close(fd2[0]);
	close(fd2[1]);
	return 0;
}