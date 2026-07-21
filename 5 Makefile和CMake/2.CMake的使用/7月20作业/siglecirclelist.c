#include "siglecirclelist.h"  //  "" 当前路径-->环境变量
                              //  <> -->环境变量 
							  //  gcc   -I指明头文件的路径

//封装函数:初始化双向链表的头结点
struct siglelist *list_init()
{
	//头节点不存放任何有效数据
	struct siglelist *headnode=malloc(sizeof(struct siglelist));
	headnode->next=NULL;  
	headnode->prev=NULL; 
	return headnode;
}

//封装函数:实现尾插新的节点
void list_insert_tail(char *newpath,struct siglelist *head)
{
	//准备新的节点
	struct siglelist *newnode=malloc(sizeof(struct siglelist));
	strcpy(newnode->bmppath,newpath);
	newnode->next=NULL;
	newnode->prev=NULL;
	
	//p指向链表的头结点
	struct siglelist *p=head;
	//循环遍历链表
	while(p->next!=NULL)  //不要写错
		p=p->next;

	//循环退出的时候p就到了最后一个有效节点的位置
	p->next=newnode; 
	newnode->prev=p; 
}






