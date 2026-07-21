#include "siglecirclelist.h"

int main()
{
	int i;
	char name[20];
	int num;
	float price;
	int fd;
	//定义数组存放拼接好的字符串
	char result[100];
	//初始化头节点
	struct siglelist *myhead=list_init();
	
	//循环插入数据到链表中，顺便存放到记事本
	for(i=0; i<2; i++)
	{
		printf("请依次输入商品的名字，数量，价格\n");
		scanf("%s",name);
		scanf("%d",&num);
		scanf("%f",&price);
		list_insert_tail(name,num,price,myhead);
	}
	
	printf("插入数据完毕了\n");
	
	//打开记事本
	fd=open("./msg.txt",O_RDWR);
	if(fd==-1)
	{
		perror("打开记事本失败了\n");
		return -1;
	}
	
	//遍历链表，把商品信息按照指定格式写入到记事本
	struct siglelist *p=myhead;
	while(p->next!=myhead)
	{
		p=p->next;
		//拼接字符串，把商品名字，数量，价格写入到记事本
		bzero(result,100); //memset()
		sprintf(result,"%s@%d@%f\n",p->name,p->num,p->price);
		//写入到记事本
		write(fd,result,strlen(result));
	}
	
	printf("存放记事本完毕\n");
	//关闭文件
	close(fd);
	//销毁链表
	list_destroy(myhead);
	return 0;
}