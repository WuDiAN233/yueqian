#ifndef _BMP_H
#define _BMP_H

#include <stdio.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>  //open的头文件
#include <unistd.h>  //read的头文件
#include <errno.h>  //perror的头文件
#include <linux/input.h>  //输入子系统模型有关的头文件
#include <sys/ioctl.h>   //ioctl的头文件
#include <dirent.h>
#include <sys/mman.h>


/*
	显示bmp图片有关的头文件
*/

//函数声明
extern int show_bmp(char *bmppath);

#endif