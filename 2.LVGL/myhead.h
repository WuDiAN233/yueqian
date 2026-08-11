#ifndef MYHEAD_H
#define MYHEAD_H

#ifndef _DEFAULT_SOURCE
#define _DEFAULT_SOURCE
#endif

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
extern lv_obj_t *chat_page; // 聊天界面
extern lv_obj_t *frd_page;  // 好友界面
extern lv_obj_t *set_page;  // 设置界面
extern lv_obj_t *frd_list;  // 好友列表
extern lv_obj_t *main_list; // 主界面聊天列表
extern lv_obj_t *chat_list; // 单聊界面好友列表
extern lv_obj_t *room_list; // 群聊好友选择列表
extern lv_obj_t *login_message; // 登录注册提示
extern lv_obj_t *search_result; // 搜索好友提示
extern lv_obj_t *search_add_btn; // 搜索好友添加按钮
extern lv_obj_t *tcp_list;  // tcp更新的列表
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
lv_obj_t *create_left_menu(lv_obj_t *parent);
void ta1_cb(lv_event_t *e);
void ta2_cb(lv_event_t *e);
void close_kb_cb(lv_event_t *e);
void set_back_main(lv_event_t *e);
void set_back_login(lv_event_t *e);
int tcp_init(void);
void tcp_update_lvgl(void);
int tcp_login(const char *username,const char *password);
int tcp_register(const char *username,const char *password);
int tcp_logout(void);
int tcp_getlist(void);
int tcp_getfriends(void);
int tcp_search(const char *username);
int tcp_addfriend(const char *username);
int tcp_delfriend(const char *username);
int tcp_chat(char *ip,unsigned short port,char *msg);
int tcp_room(const char *targets,const char *msg);
int tcp_close(void);
void tcp_test(char *msg);
void tcp_filter_friends(const char *keyword);
void chat_record_append(int room,const char *line);

extern char chatip[20];
extern unsigned short chatport;
extern char chatname[64];
extern char roomtargets[1024];
extern char roommemberbuf[20][80];
extern int roommembernum;
extern char search_account_name[64];

#endif /* MYHEAD_H */
