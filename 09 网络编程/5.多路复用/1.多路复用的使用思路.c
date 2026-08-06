#include "myhead.h"

/*
	多路复用使用思路
	   1.思考清楚你的代码需要监测哪些文件描述符
	     等一会举例教你思考
		    假设我要监测fd1  fd2  fd3 
		 
	   2.定义文件描述符集合变量，存放所有你想要监测的文件描述符
	      fd_set 本质上也是数组，专门存放你要监测的文件描述符
		  fd_set myset;
		  FD_ZERO(&myset); //清空集合
		  FD_SET(fd1,&myset);
		  FD_SET(fd2,&myset);
		  FD_SET(fd3,&myset);
		  
		  void FD_CLR(int fd, fd_set *set);
		        把fd从集合set中删除
          int  FD_ISSET(int fd, fd_set *set);
		        判断fd在不在集合set中
				
          void FD_SET(int fd, fd_set *set);
		        把fd添加到集合set中
          void FD_ZERO(fd_set *set);
		        清空集合，把集合中所有存放的文件描述符删除
 
        3.调用linux系统提供多路复用的函数select()监测三个文件描述符的状态变化
		  select函数是阻塞的，如果文件描述符没有发生状态改变，select会阻塞当前进程
		  如果select监测到了某个文件描述符发生状态改变，select会自动解除阻塞，返回值>0
		  int select(int nfds, fd_set *readfds, fd_set *writefds,fd_set *exceptfds, struct timeval *timeout);
		      返回值：
			          >0  监测到文件描述符发生状态改变
					  ==0 超时
					  -1  监测失败
                参数: nfds --》最大的文件描述符+1
		                      把fd1 fd2 fd3三者最大值求出来，然后最大值+1
							select(最大+1)	
					 readfds --》监测文件描述符是否有数据可读(读就绪)
					 writefds --》监测文件描述符是否有数据可写(写就绪) 
                     exceptfds --》监测文件描述符是否发生异常(异常就绪)	
							select(最大+1,&myset,NULL,NULL); 我要监测文件描述符是否有数据可读	 
							select(最大+1,NULL,&myset,NULL); 我要监测文件描述符是否有数据可写
					 timeout --》时间结构体变量，存放超时时间的(单独讨论，暂时不理会)
		我打算举两个例子
		    例子1：tcp单向通信的例子，用多路复用来实现
			       客户端发送信息给服务器
*/ 
int main()
{
	
}