#include "list.h"  

/*
	用单链表存放所有连接成功的客户端信息
*/

//封装函数实现单链表的初始化--》初始化头结点(链表的第一个节点)
struct singlelist *list_init()
{
	//定义指针申请堆空间
	struct singlelist *head=malloc(sizeof(struct singlelist));
	head->next=NULL;
	return head;
}

//尾插新的结点(新的数据)-->把newdata存放到head代表的链表的尾部
int list_insert_tail(struct singlelist *newnode,struct singlelist *head)
{
	//找到链表的尾部
	struct singlelist *p=head; //最开始指向链表的头结点
	while(p->next!=NULL)  //p的后面一个结点不是空的
		p=p->next;  //p指向p后面一个结点的位置

	//把新的节点插入到末尾位置	
	p->next=newnode;  //p的next指向新的节点(新的节点地址保存到p->next里面)
	return 0;
}

//实现删除指定的结点
int list_delete(char *delip,unsigned short delport,struct singlelist *head)
{

	return 0;
}

/*//单链表的销毁
int list_destroy(struct singlelist *head)
{
	struct singlelist *p=head->next;
	//循环每一次释放一个有效结点
	//每一次都是从头结点下一个结点开始释放
	while(p!=NULL)
	{
		printf("我目前要销毁的结点是： %d\n",p->data);
		head->next=p->next;
		p->next=NULL;
		free(p);
		p=NULL;
		p=head->next;
	}
	
	//释放头结点
	free(head);
	head=NULL;
	return 0;
} */



