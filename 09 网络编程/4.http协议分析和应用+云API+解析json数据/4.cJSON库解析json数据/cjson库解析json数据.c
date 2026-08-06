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
    const char *jsondata =
        "{"
        "  \"status\": \"0\","
        "  \"msg\": \"ok\","
        "  \"result\": {"
        "    \"number\": \"465562122253261\","
        "    \"type\": \"YUNDA\","
        "    \"list\": ["
        "      {"
        "        \"time\": \"2026-08-06 07:29:46\","
        "        \"status\": \"【广州市】已离开广东广州分拨交付中心，发往广东广州从化区公司\","
        "        \"contact\": \"020-23393001\""
        "      },"
        "      {"
        "        \"time\": \"2026-08-06 07:09:31\","
        "        \"status\": \"【广州市】已到达广东广州分拨交付中心\","
        "        \"contact\": \"020-22145829\""
        "      },"
        "      {"
        "        \"time\": \"2026-08-05 19:26:51\","
        "        \"status\": \"【泉州市】已离开福建晋江分拨交付中心，发往广东广州分拨交付中心\","
        "        \"contact\": \"020-22145829\""
        "      },"
        "      {"
        "        \"time\": \"2026-08-05 19:24:14\","
        "        \"status\": \"【泉州市】已到达福建晋江分拨交付中心\","
        "        \"contact\": \"0595-28014383\""
        "      },"
        "      {"
        "        \"time\": \"2026-08-05 18:59:02\","
        "        \"status\": \"【泉州市】已离开福建石狮市公司鸿仓KH分部，发往福建晋江分拨交付中心\","
        "        \"contact\": \"0595-36124773\""
        "      },"
        "      {"
        "        \"time\": \"2026-08-05 11:14:17\","
        "        \"status\": \"【泉州市】福建石狮市公司鸿仓KH分部已揽收\","
        "        \"courier\": {"
        "          \"name\": \"柯富贵\","
        "          \"phone\": \"15880883195\""
        "        },"
        "        \"contact\": \"原始内容已截断\""
        "      }"
        "    ]"
        "  }"
        "}";
	
	//第一步：把字符串格式的json数据转换成结构体类型，存放到链表中
	/*
		cJSON库源码把对象中键值对存放到一个链表中
	*/
	cJSON *myjson=cJSON_Parse(jsondata);
	
	//第二步：严格按照你肉眼观察到json数据的组成结构，一步步解析json数据
	//先解析"status"这个键对应的值
	cJSON *val1=cJSON_GetObjectItem(myjson,"status"); //我要获取status这个键对应的值
	printf("status这个键对应的值是： %s\n",val1->valuestring);
	
	//解析"msg"这个键对应的值
	cJSON *val2=cJSON_GetObjectItem(myjson,"msg"); //我要获取msg这个键对应的值
	printf("msg这个键对应的值是： %s\n",val2->valuestring);
	
	//解析"result"这个键对应的值
	cJSON *val3=cJSON_GetObjectItem(myjson,"result"); //我要获取result这个键对应的值

	cJSON *val4=cJSON_GetObjectItem(val3,"number"); //我要获取number这个键对应的值
	printf("number这个键对应的值是： %s\n",val4->valuestring);

	cJSON *val5=cJSON_GetObjectItem(val3,"type"); //我要获取type这个键对应的值
	printf("type这个键对应的值是： %s\n",val5->valuestring);

	cJSON *val6=cJSON_GetObjectItem(val3,"list"); //我要获取list这个键对应的值
	for(int i=0; i<cJSON_GetArraySize(val6); i++)
	{
		cJSON *val7=cJSON_GetArrayItem(val6,i);
		cJSON *val8=cJSON_GetObjectItem(val7,"time"); 
		printf("time这个键对应的值是： %s\n",val8->valuestring);
		cJSON *val9=cJSON_GetObjectItem(val7,"status"); 
		printf("status这个键对应的值是： %s\n",val9->valuestring);
		cJSON *val10=cJSON_GetObjectItem(val7,"contact"); 
		printf("contact这个键对应的值是： %s\n",val10->valuestring);

	}
	//释放
	cJSON_Delete(myjson);
	
	return 0;
}