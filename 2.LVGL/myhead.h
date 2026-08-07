#ifndef MYHEAD_H
#define MYHEAD_H

#include "lvgl/lvgl.h"
#include "lvgl/demos/lv_demos.h"
#include "lv_drivers/display/fbdev.h"
#include "lv_drivers/indev/evdev.h"
#include <unistd.h>
#include <pthread.h>
#include <time.h>
#include <sys/time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <linux/input.h>
#include <sys/ioctl.h>
#include <stdbool.h>
#include <linux/input.h> 
#include <sys/mman.h> 
#include <errno.h>  
#include <dirent.h>  
#include <sys/wait.h> 
#include <pthread.h>  
#include <sys/socket.h>  
#include <netinet/in.h>
#include <arpa/inet.h>

#define DISP_BUF_SIZE (1024 * 900)

#define TEST_MAGIC 'x'


extern lv_obj_t * kb1; // 软键盘的地址
extern lv_obj_t * ta1; // 文本输入框的地址
extern lv_obj_t * ta2; // 文本输入框的地址
extern lv_obj_t *loginwin; // 登录界面的地址
extern lv_obj_t *mainwin;   // 主界面
extern lv_obj_t *setwin;   // 其他功能界面的地址
extern lv_obj_t *addwin;   // 添加功能弹框
extern lv_obj_t *morewin;  // 更多功能弹框
extern lv_obj_t *chatmsg;  // 单聊消息显示框
extern lv_obj_t *roommsg;  // 群聊消息显示框
extern lv_obj_t *feedback_ta; // 反馈输入框
extern lv_style_t *mystyle; // 中文字体样式


/* 先声明函数，下面才能互相调用 */
extern lv_style_t *create_chinese_style(char *fontpath, int fontsize);
void show_main(void);
void show_login(void);
void show_sign(void);
void show_chat(void);
void show_room(void);
void show_frd(void);
void show_set(void);
void show_add_account(void);
void show_ip_connect(void);
void show_choose_room(void);
void show_feedback(void);
void ta1_cb(lv_event_t *e);
void ta2_cb(lv_event_t *e);
void close_kb_cb(lv_event_t *e);
void set_back_main(lv_event_t *e);
void set_back_login(lv_event_t *e);
void tcp_getlist();
void tcp_chat(char *ip,unsigned short port,char *msg);
void tcp_close();
void tcp_test(char *msg);

#endif /* MYHEAD_H */
