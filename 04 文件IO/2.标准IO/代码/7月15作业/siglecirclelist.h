#ifndef _SIGLECIRCLELIST_H
#define _SIGLECIRCLELIST_H

#include <stdio.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>  //open的头文件
#include <unistd.h>  //read的头文件
#include <errno.h>  //perror的头文件
#include <linux/input.h>  //输入子系统模型有关的头文件
#include <sys/ioctl.h>   //ioctl的头文件

/*
	单向循环链表的头文件
*/
//定义结构体表示单向循环链表
struct siglelist
{
	//数据域:存放真实数据的那些变量,就是数据域
	char name[20]; //商品名字
	int num;     //商品数量
	float price; //商品价格
	//指针域
	struct siglelist *next;  //存放下一个数据在内存中的首地址-->方便寻找下一个
};

//函数声明
extern struct siglelist *list_init();
extern void list_insert_tail(char *newname,int newnum,float newprice,struct siglelist *head);
extern void list_destroy(struct siglelist *head);
#endif