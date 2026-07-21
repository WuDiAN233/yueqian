#include "siglecirclelist.h"  //  "" 当前路径-->环境变量
                              //  <> -->环境变量 
							  //  gcc   -I指明头文件的路径

//封装函数:初始化单向循环链表的头结点
struct siglelist *list_init()
{
	struct siglelist *headnode=malloc(sizeof(struct siglelist));
	headnode->next=headnode;  //自己指向自己,最开始只有一个节点(头结点)
	return headnode;
}

//封装函数:实现尾插新的节点
void list_insert_tail(char *newname,int newnum,float newprice,struct siglelist *head)
{
	//准备新的节点
	struct siglelist *newnode=malloc(sizeof(struct siglelist));
	strcpy(newnode->name,newname);
	newnode->num=newnum;
	newnode->price=newprice;
	newnode->next=NULL;
	
	//p指向链表的头结点
	struct siglelist *p=head;
	//循环遍历链表
	while(p->next!=head)  //不要写错
		p=p->next;

	//循环退出的时候p就到了最有一个有效节点的位置
	p->next=newnode;
	newnode->next=head;
}


//销毁链表-->从头到尾销毁
void list_destroy(struct siglelist *head)
{
	struct siglelist *q=head;
	struct siglelist *p=head->next;
	while(q->next!=head)
	{
		p=q->next; //把下一个节点备份
		free(q);
		q=p;
	}
	
	//循环结束的时候,q指向最后一个节点
	free(q);
}




