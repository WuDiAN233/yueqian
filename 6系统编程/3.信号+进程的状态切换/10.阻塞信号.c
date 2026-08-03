#include "myhead.h"

/*
	总结：信号响应的四种方式
	    第一种：默认动作
		第二种：signal修改响应动作
		第三种：signal忽略信号
		第四种：阻塞(屏蔽)信号
	1.忽略信号和阻塞(屏蔽)信号有什么区别
	    忽略信号：进程收到信号之后，直接丢弃这个信号，当它不存在
		阻塞(屏蔽)信号：进程暂时把信号挂起，不响应，如果你解除阻塞，依然可以响应
    2.阻塞(屏蔽)信号思路
         第一步：定义集合变量，存放所有你要阻塞的信号
		         集合变量: sigset_t myset;    sigset_t本质上是个数组，该数组用来存放所有你需要阻塞的信号
				  int sigemptyset(sigset_t *set); //清空集合
                  int sigfillset(sigset_t *set); //一口气把62个信号全部添加到集合中
                  int sigaddset(sigset_t *set, int signum); //把signum信号添加到集合中
				  int sigdelset(sigset_t *set, int signum); //把signum信号从集合中删除
                  int sigismember(const sigset_t *set, int signum); //判断signum是否在集合中
        第二步：调用sigprocmask这个函数设置阻塞信号
		       int sigprocmask(int how, const sigset_t * set,sigset_t * oset);
    3.第三个参数备份当前进程已经设置阻塞的那些信号
        sigset_t oldset;
	    sigemptyset(&oldset);
	    sigprocmask(SIG_BLOCK,NULL,&oldset); 此时sigprocmask会把已经设置阻塞的那些信号备份存放到oldset中	
  
				 
*/

int main(int argc,char **argv)
{
	//第一步：定义集合变量，存放所有你要阻塞的信号
	sigset_t myset;
	//清空集合
	sigemptyset(&myset);
	//往集合中添加你需要阻塞的信号
	sigaddset(&myset,1);
	sigaddset(&myset,2);
	sigaddset(&myset,3);
	
	//第二步：调用sigprocmask这个函数设置阻塞信号
	//SIG_BLOCK表示阻塞信号
	sigprocmask(SIG_BLOCK,&myset,NULL);
	
	//阻塞进程等到信号到来
	pause();
	return 0;
}