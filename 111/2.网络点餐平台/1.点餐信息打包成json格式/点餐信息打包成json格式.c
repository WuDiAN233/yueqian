#include "cJSON.h"
#include "myhead.h"

/*
	相关的接口函数
	  创建对象
	     cJSON * cJSON_CreateObject()
	  创建数组
	     cJSON * cJSON_CreateArray()
	  往数组里面添加元素
	     void cJSON_AddItemToArray(cJSON *array, cJSON *item)
	  往对象里面添加键值对
	      void cJSON_AddItemToObject(cJSON *object, const char *string, cJSON *item)
		           参数：object --》你要添加键值对的那个对象
		                 string --》键，只能是字符串
						 item --》键对应的值(值可以是任何类型)
						          如果你的值是字符串
								     cJSON_CreateString("小龙虾")
								  如果你的值是整数/小数
								     cJSON_CreateNumber(double num);
						          如果你的值是数组
								     cJSON_CreateArray(void);
	  void cJSON_Delete(cJSON *c)
	  cJSON_AddNumberToObject(object,name,n)	  
      cJSON_AddStringToObject(object,name,s)
	  //把json数据转成字符串格式
	      char * cJSON_Print(const cJSON *item)
*/
int main()
{
	//创建最外层的对象
	cJSON *root=cJSON_CreateObject();
	
	//对象里面添加键值对
	cJSON_AddStringToObject(root,"用户名","张三");
	//创建一个数组
	cJSON *array1=cJSON_CreateArray();
	//数组里面添加元素
	cJSON_AddItemToArray(array1,cJSON_CreateString("小龙虾"));
	cJSON_AddItemToArray(array1,cJSON_CreateString("烧鸡"));
	cJSON_AddItemToArray(array1,cJSON_CreateString("清蒸鲈鱼"));
	//对象里面添加键值对 
	cJSON_AddItemToObject(root,"点餐情况",array1);
	cJSON_AddNumberToObject(root,"总额",198);
	
	//把json数据转成字符串格式
	char *str=cJSON_Print(root);
	printf("数据是: %s\n",str);
	
	//释放
	cJSON_Delete(root);
	free(str);
	return 0;
}