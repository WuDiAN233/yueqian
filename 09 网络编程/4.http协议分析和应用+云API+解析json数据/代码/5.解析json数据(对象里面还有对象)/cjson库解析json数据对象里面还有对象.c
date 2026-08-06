#include "myhead.h"
#include "cJSON.h" //cJSON库的头文件

/*
	1.对象里面还有对象
	  三句话总结json数据
	     第一：任何json数据，最外层一定是对象
		 第二：对象里面一定存放的是键值对
		 第三：键一定是字符串，值可以是任意合法类型(整数，小数，布尔，数组，对象)
*/

int main()
{
	//定义json数据，让你解析,从简单到复杂
	//原始的json数据
	char *jsondata="{\"姓名\":\"张三\",\"成绩\":{\"语文\":125,\"数学\":115,\"英语\":135,\"物理\":73,\"化学\":68}}";
	
	//第一步：把字符串格式的json数据转换成结构体类型，存放到链表中
	/*
		cJSON库源码把对象中键值对存放到一个链表中
	*/
	cJSON *myjson=cJSON_Parse(jsondata);
	
	//第二步：严格按照你肉眼观察到json数据的组成结构，一步步解析json数据
	//先解析"姓名"这个键对应的值
	cJSON *val1=cJSON_GetObjectItem(myjson,"姓名"); //我要获取姓名这个键对应的值
	printf("姓名这个键对应的值是： %s\n",val1->valuestring);
	
	//解析"成绩"这个键对应的值
	cJSON *val2=cJSON_GetObjectItem(myjson,"成绩"); //我要获取成绩这个键对应的值
	//观察发现val2依然是个对象，因此需要继续解析(一层层剥洋葱)
	//解析"语文"这个键对应的值(语文这个键属于val2)
	cJSON *val3=cJSON_GetObjectItem(val2,"语文"); //解析val2这个对象里面的键值对
	printf("语文这个键对应的值是： %d\n",val3->valueint); 
	//解析"数学"这个键对应的值(数学这个键属于val2)
	cJSON *val4=cJSON_GetObjectItem(val2,"数学"); //解析val2这个对象里面的键值对
	printf("数学这个键对应的值是： %d\n",val4->valueint); 
	
	//释放
	cJSON_Delete(myjson);
	
	return 0;
}