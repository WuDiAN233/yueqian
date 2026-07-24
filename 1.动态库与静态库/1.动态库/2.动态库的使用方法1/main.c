#include "pid.h"

int main()
{
	//虽然我无法查看pid.c源码如何写的(已经编译成动态库)
	//但是我可以调用动态库编译好的函数
	fun1();
	fun2();
	fun3();
	return 0;
}