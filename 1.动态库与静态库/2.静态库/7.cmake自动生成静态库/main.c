#include "wifi.h"
#include "ble.h"

int main()
{
	printf("你好粤嵌!\n");
	//打开wifi
	open_wifi();
	//打开蓝牙
	open_ble();
	return 0;
}