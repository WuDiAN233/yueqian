#include "siglecirclelist.h"  //  "" 当前路径-->环境变量
                              //  <> -->环境变量 
							  //  gcc   -I指明头文件的路径

//封装函数:初始化双向链表的头结点
struct siglelist *list_init()
{
	//头节点不存放任何有效数据
	struct siglelist *headnode=malloc(sizeof(struct siglelist));
	headnode->next=headnode;  
	headnode->prev=headnode; 
	return headnode;
}

//封装函数:实现尾插新的节点
void list_insert_tail(char *newpath,struct siglelist *head)
{
	//准备新的节点
	struct siglelist *newnode=malloc(sizeof(struct siglelist));
	strcpy(newnode->bmppath,newpath);//修改的路径 
	struct siglelist *lastnode = head->prev;
	//四个链表的循环关系 
	// ② 新节点的 next 指向 头节点
	newnode->next = head;
   	// ① 新节点的 prev 指向 原来的尾节点
    newnode->prev = lastnode;

    // ③ 原尾节点的 next 指向 新节点
    lastnode->next = newnode;
    // ④ 头节点的 prev 指向 新节点（更新尾巴位置）
    head->prev = newnode;
}






