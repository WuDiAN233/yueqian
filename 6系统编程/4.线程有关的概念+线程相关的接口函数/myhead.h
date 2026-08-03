#ifndef MYHEAD_H
#define MYHEAD_H

/*
 * Linux C / 嵌入式开发常用公共头文件
 *
 * 用法：
 *   #include "myhead.h"
 *
 * 说明：
 *   - 只收录普通 Linux 系统和常见交叉编译环境中普遍可用的头文件。
 *   - 避免加入架构相关、板级私有、容易互相冲突或不稳定的内核内部头文件。
 *   - 如果某个工程需要更严格的编译速度或命名隔离，建议按模块继续精简。
 */

/* 标准 C：基础类型、工具函数、字符串、错误码 */
#include <assert.h>
#include <ctype.h>
#include <errno.h>
#include <float.h>
#include <inttypes.h>
#include <limits.h>
#include <locale.h>
#include <math.h>
#include <setjmp.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

/* POSIX 基础：系统调用、文件、目录、权限、环境 */
#include <dirent.h>
#include <fcntl.h>
#include <getopt.h>
#include <poll.h>
#include <pwd.h>
#include <grp.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <sys/stat.h>
#include <sys/statvfs.h>
#include <sys/types.h>
#include <sys/uio.h>
#include <sys/wait.h>

/* 进程、资源、信号、定时器 */
#include <sched.h>
#include <signal.h>
#include <sys/prctl.h>
#include <sys/resource.h>
#include <sys/select.h>
#include <sys/time.h>
#include <sys/timerfd.h>
#include <sys/utsname.h>

/* 内存映射、共享内存、消息队列、事件通知 */
#include <mqueue.h>
#include <semaphore.h>
#include <sys/eventfd.h>
#include <sys/ipc.h>
#include <sys/mman.h>
#include <sys/msg.h>
#include <sys/sem.h>
#include <sys/shm.h>

/* 线程、互斥锁、条件变量、读写锁 */
#include <pthread.h>

/* Socket / 网络：IPv4、IPv6、Unix 域 socket、地址转换 */
#include <arpa/inet.h>
#include <netdb.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <sys/socket.h>
#include <sys/un.h>

/* 终端、串口、伪终端 */
#include <pty.h>
#include <termios.h>

/* Linux 文件系统与设备接口 */
#include <linux/fb.h>
#include <linux/input.h>
#include <linux/limits.h>
#include <linux/serial.h>

/* 其他常用 Linux 机制 */
#include <dlfcn.h>
#include <elf.h>
#include <sys/epoll.h>
#include <sys/inotify.h>
#include <sys/reboot.h>
#include <sys/sendfile.h>
#include <sys/signalfd.h>
#include <sys/syscall.h>

#endif /* MYHEAD_H */
