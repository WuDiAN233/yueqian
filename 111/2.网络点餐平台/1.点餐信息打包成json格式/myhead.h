#ifndef _MYHEAD_H
#define _MYHEAD_H

//把C语言所有目前常用的头文件都包含进来
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <sys/types.h>  //文件IO有关的头文件
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <dirent.h>  //目录有关的头文件
#include <errno.h> //perror有关  errno有关的头文件
#include <stdbool.h>
#include <sys/mman.h>
#include <sys/wait.h> //跟wait，waitpid有关的头文件
#include <signal.h> //跟信号有关的头文件
#include <pthread.h>  //跟线程有关的头文件
#include <sys/socket.h>  //跟网络编程有关的头文件
#include <netinet/in.h>  //跟网络编程有关的头文件
#include <arpa/inet.h>   //跟网络编程有关的头文件
#include <netdb.h>  //跟gethostbyname有关的头文件

#endif