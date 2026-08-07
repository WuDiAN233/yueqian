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
	struct singlelist *p=head;
    struct singlelist *q;
    while(p->next!=NULL)
    {
        q=p->next;
        if(strcmp(q->ip,delip)==0 && q->port==delport)
        {
            p->next=q->next;
            close(q->sock);
            q->next=NULL;
            free(q);
            return 0;
        }
        p=p->next;
    }
    return -1;
} 



