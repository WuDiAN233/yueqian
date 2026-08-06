#include "myhead.h"
#include "cJSON.h" //cJSON库的头文件

/*
	1.嵌入式开发过程：经常使用第三方开源库
	  一流的企业：开源库消化吸收--》优化做二次开发
	              大疆--》无人机飞行控制系统--》开源的无人机飞控系统，开源PID算法
				  大疆厉害：请了一帮顶尖学校高手，消化吸收开源代码的解题思路
	  二流的企业：只能抄作业
	               
	2.C语言：双引号嵌套双引号，里面的双引号必须要转义
	3.cJSON类型是个结构体类型
	 {
		  char *valuestring;   //键值对，值是字符串就自动存放在这个成员变量中
		  int valueint;        //键值对，值是整数或者bool类型就自动存放在这个成员变量中
		  double valuedouble;  //键值对，值是小数类型就自动存放在这个成员变量中
	 }
	   
*/

int main()
{
	//定义json数据，让你解析,从简单到复杂
	//原始的json数据
	char *jsondata="{\"姓名\":\"张三\",\"年龄\":18,\"是否汉族\":false,\"成绩\":95.5}";
	
	//第一步：把字符串格式的json数据转换成结构体类型，存放到链表中
	/*
		cJSON库源码把对象中键值对存放到一个链表中
	*/
	cJSON *myjson=cJSON_Parse(jsondata);
	
	//第二步：严格按照你肉眼观察到json数据的组成结构，一步步解析json数据
	//先解析"姓名"这个键对应的值
	cJSON *val1=cJSON_GetObjectItem(myjson,"姓名"); //我要获取姓名这个键对应的值
	printf("姓名这个键对应的值是： %s\n",val1->valuestring);
	
	//解析"年龄"这个键对应的值
	cJSON *val2=cJSON_GetObjectItem(myjson,"年龄"); //我要获取年龄这个键对应的值
	printf("年龄这个键对应的值是： %d\n",val2->valueint);
	
	//解析"是否汉族"这个键对应的值
	cJSON *val3=cJSON_GetObjectItem(myjson,"是否汉族"); //我要获取是否汉族这个键对应的值
	printf("是否汉族这个键对应的值是： %d\n",val3->valueint);
	
	//解析"成绩"这个键对应的值
	cJSON *val4=cJSON_GetObjectItem(myjson,"成绩"); //我要获取成绩这个键对应的值
	printf("成绩这个键对应的值是： %lf\n",val4->valuedouble);
	
	//释放
	cJSON_Delete(myjson);
	
	return 0;
}