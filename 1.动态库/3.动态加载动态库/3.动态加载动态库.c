#include "pid.h"

/*
	此例子给大家演示如何调用linux提供的函数动态加载动态库
	
*/

int main()
{
	void *handler;
	void *funp;
	//打开你要使用的动态库文件
	handler=dlopen("/mnt/hgfs/share/libpid.so",RTLD_LAZY);
	if(handler==NULL)
	{
		perror("打开动态库文件失败了\n");
		return -1;
	}
	
	//指定解析调用需要的函数
	funp=dlsym(handler,"fun1"); //我要查找解析libpid.so动态库中函数名叫做fun1的函数
	//写法1：简化偷懒的写法，不做类型转换
	void (* task1)()=funp;  //C语言gcc编译器不是很严格，void *可以赋值给其他类型的指针，不做强制类型转换也行
	
	//写法2：做类型转换
	//用typedef给函数指针类型取个别名，方便我书写代码
	//typedef void (* taskpoint)();
	//void (* task1)()=(taskpoint)funp; //强制转换funp为void (*)()类型
	
	//通过函数指针来调用fun1
	task1();
	
	//关闭动态库
	dlclose(handler);
	return 0;
}