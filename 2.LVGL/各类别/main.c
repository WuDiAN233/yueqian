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

#define DISP_BUF_SIZE (1024 * 900)

#define TEST_MAGIC 'x'


lv_obj_t * kb1; // 软键盘的地址
lv_obj_t * ta1; // 文本输入框的地址
lv_obj_t * ta2; // 文本输入框的地址
lv_obj_t *loginwin; // 登录界面的地址
lv_obj_t *mainwin;   // 主界面
lv_obj_t *setwin;   // 其他功能界面的地址
lv_obj_t *addwin;   // 添加功能弹框
lv_obj_t *morewin;  // 更多功能弹框
lv_obj_t *chatmsg;  // 单聊消息显示框
lv_obj_t *roommsg;  // 群聊消息显示框
lv_obj_t *feedback_ta; // 反馈输入框
lv_style_t *mystyle; // 中文字体样式

/* 先声明函数，下面才能互相调用 */
lv_style_t *create_chinese_style(char *fontpath, int fontsize);
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
uint32_t custom_tick_get(void);
/* 使用freetype显示中文 */
lv_style_t *create_chinese_style(char *fontpath, int fontsize)
{
    static lv_ft_info_t info;
    info.name = fontpath;
    info.weight = fontsize;
    info.style = FT_FONT_STYLE_NORMAL;
    info.mem = NULL;

    if(!lv_ft_font_init(&info)) {
        LV_LOG_ERROR("create failed.");
    }

    static lv_style_t style;
    lv_style_init(&style);
    lv_style_set_text_font(&style, info.font);
    lv_style_set_text_align(&style, LV_TEXT_ALIGN_CENTER);

    return &style;
}

//这里凡是要聊天的输入框都要关联软键盘
void ta1_cb(lv_event_t * e)
{
    ta1 = lv_event_get_target(e);
    // 把软键盘跟第一个输入框关联
    lv_keyboard_set_textarea(kb1, ta1);
    lv_obj_clear_flag(kb1, LV_OBJ_FLAG_HIDDEN);
    lv_obj_move_foreground(kb1);
}

// 输入框2的事件响应函数
void ta2_cb(lv_event_t * e)
{
    ta2 = lv_event_get_target(e);
    // 把软键盘跟第二个输入框关联
    lv_keyboard_set_textarea(kb1, ta2);
    lv_obj_clear_flag(kb1, LV_OBJ_FLAG_HIDDEN);
    lv_obj_move_foreground(kb1);
}

// 点击空白处关闭软键盘
void close_kb_cb(lv_event_t * e)
{
    lv_obj_t *target = lv_event_get_target(e);
    lv_obj_t *current = lv_event_get_current_target(e);

    if(target == current && kb1 != NULL)
    {
        lv_keyboard_set_textarea(kb1, NULL);
        lv_obj_add_flag(kb1, LV_OBJ_FLAG_HIDDEN);
    }
}

//登录界面 -> 注册界面 
void log_go_sig(lv_event_t * e)
{
    show_sign();
    lv_obj_del(loginwin);
}

//登入界面 ->主界面
void log_go_main(lv_event_t * e) // 登录按钮
{
    //stp接入
    show_main();
    lv_obj_del(loginwin);
}

//注册界面 ->主菜单
void set_go_main(lv_event_t * e) // 注册按钮
{
    //stp接入
    show_main();
    lv_obj_del(setwin);
}

// 主界面 -> 登入界面  
void main_back_log(lv_event_t * e)
{
    show_login();
    lv_obj_del(mainwin);
}

// 注册界面 -> 登录界面
void set_back_log(lv_event_t * e)
{
    show_login();
    lv_obj_del(setwin);
}

// 其他功能界面 -> 主界面
void set_back_main(lv_event_t * e)
{
    show_main();
    lv_obj_del(setwin);
}

// 主界面 -> 群聊界面
void main_go_chat(lv_event_t * e)
{
    show_room();
    lv_obj_del(mainwin);
}

// 主界面 -> 好友界面
void main_go_frd(lv_event_t * e)
{
    show_frd();
    lv_obj_del(mainwin);
}

// 主界面 -> 设置界面
void main_go_set(lv_event_t * e)
{
    show_set();
    lv_obj_del(mainwin);
}

//好友界面 -> 聊天界面
void fri_chat_cb(lv_event_t * e)
{
    lv_obj_t *oldwin = setwin;
    //stp接入
    show_chat();
    lv_obj_del(oldwin);
}

//聊天发送按钮
void send_chat_cb(lv_event_t * e)
{
    const char *msg = lv_textarea_get_text(ta1);
    if(strlen(msg) == 0)
        return;

    //stp接入
}

//搜索聊天记录
void search_chat_record_cb(lv_event_t * e)
{
    //stp接入
}

//群聊发送按钮
void send_room_cb(lv_event_t * e)
{
    const char *msg = lv_textarea_get_text(ta1);
    if(strlen(msg) == 0)
        return;

    //stp接入
}

//清空聊天记录
void clear_chat_record_cb(lv_event_t * e)
{
    if(chatmsg != NULL)
        lv_textarea_set_text(chatmsg, "");
    if(roommsg != NULL)
        lv_textarea_set_text(roommsg, "");
    //stp接入
}

//搜索好友
void search_main_cb(lv_event_t * e)
{
    const char *msg = lv_textarea_get_text(ta1);
    if(strlen(msg) == 0)
        return;

    //stp接入
}

//删除好友
void delete_friend_cb(lv_event_t * e)
{
    lv_obj_t *oldwin = setwin;
    //stp接入
    show_frd();
    lv_obj_del(oldwin);
}
//关闭添加功能弹框
void close_add_cb(lv_event_t * e)
{
    lv_obj_t *target = lv_event_get_target(e);
    lv_obj_t *current = lv_event_get_current_target(e);

    if(target != current)
        return;

    if(addwin != NULL)
    {
        lv_obj_del(addwin);
        addwin = NULL;
    }
}

//关闭更多功能弹框
void close_more_cb(lv_event_t * e)
{
    lv_obj_t *target = lv_event_get_target(e);
    lv_obj_t *current = lv_event_get_current_target(e);

    if(target != current)
        return;

    if(morewin != NULL)
    {
        lv_keyboard_set_textarea(kb1, NULL);
        lv_obj_add_flag(kb1, LV_OBJ_FLAG_HIDDEN);
        lv_obj_del(morewin);
        morewin = NULL;
    }
}


//+功能弹框 -> 输入账号添加好友界面
void add_account_cb(lv_event_t * e)
{
    addwin = NULL;
    show_add_account();
    lv_obj_del(mainwin);
}

//+功能弹框 -> IP连接界面
void add_ip_cb(lv_event_t * e)
{
    addwin = NULL;
    show_ip_connect();
    lv_obj_del(mainwin);
}

//+功能弹框 -> 发起群聊界面
void add_room_cb(lv_event_t * e)
{
    addwin = NULL;
    show_choose_room();
    lv_obj_del(mainwin);
}

//设置界面 -> 登录界面
void set_back_login(lv_event_t * e)
{
    //stp接入
    show_login();
    lv_obj_del(setwin);
}

//设置界面 -> 反馈界面
void set_back_feedback_cb(lv_event_t * e)
{
    lv_obj_t *oldwin = setwin;
    show_feedback();
    lv_obj_del(oldwin);
}

//反馈界面 -> 设置界面 
void feedback_back_set_cb(lv_event_t * e)
{
    lv_obj_t *oldwin = setwin;
    show_set();
    lv_obj_del(oldwin);
}

//保存反馈内容到记事本按钮
void save_feedback_cb(lv_event_t * e)
{
    const char *msg = lv_textarea_get_text(feedback_ta);
    if(strlen(msg) == 0)
        return;

    FILE *fp = fopen("./feedback.txt", "a+");
    if(fp == NULL)
    {
        perror("open feedback.txt");
        return;
    }

    fprintf(fp, "%s\n", msg);
    fclose(fp);
    lv_textarea_set_text(feedback_ta, "");
}


//创建反馈界面
void show_feedback()
{
    if(kb1 != NULL)
    {
        lv_keyboard_set_textarea(kb1, NULL);
        lv_obj_add_flag(kb1, LV_OBJ_FLAG_HIDDEN);
    }
    setwin = lv_obj_create(NULL);
    lv_obj_set_size(setwin, 800, 480);
    lv_obj_add_event_cb(setwin, close_kb_cb, LV_EVENT_CLICKED, NULL);

    lv_obj_t * lb1 = lv_label_create(setwin);
    lv_obj_set_pos(lb1, 100, 20);
    lv_obj_set_size(lb1, 600, 70);
    lv_obj_add_style(lb1,mystyle,0);
    lv_label_set_text(lb1, "反馈");

    feedback_ta = lv_textarea_create(setwin);
    ta1 = feedback_ta;
    lv_obj_set_pos(feedback_ta, 80, 110);
    lv_obj_set_size(feedback_ta, 640, 230);
    lv_textarea_set_placeholder_text(feedback_ta, "请输入反馈内容");
    lv_obj_add_event_cb(feedback_ta, ta1_cb, LV_EVENT_FOCUSED, NULL);

    lv_obj_t * bt1 = lv_btn_create(setwin); 
    lv_obj_set_pos(bt1, 600, 370);
    lv_obj_set_size(bt1, 120, 60);
    lv_obj_t * lb2 = lv_label_create(bt1);
    lv_obj_add_style(lb2,mystyle,0);
    lv_label_set_text(lb2, "提交");
    lv_obj_center(lb2);
    lv_obj_add_event_cb(bt1, save_feedback_cb, LV_EVENT_CLICKED, NULL);

    lv_obj_t * bt2 = lv_btn_create(setwin); 
    lv_obj_set_pos(bt2, 80, 370);
    lv_obj_set_size(bt2, 120, 60);
    lv_obj_t * lb3 = lv_label_create(bt2);
    lv_obj_add_style(lb3,mystyle,0);
    lv_label_set_text(lb3, "返回");
    lv_obj_center(lb3);
    lv_obj_add_event_cb(bt2, feedback_back_set_cb, LV_EVENT_CLICKED, NULL);

    lv_scr_load(setwin);
}


//聊天界面更多功能按钮
void chat_more_cb(lv_event_t * e)
{
    if(morewin != NULL)
        return;

    morewin = lv_obj_create(setwin);
    lv_obj_set_pos(morewin, 420, 60);
    lv_obj_set_size(morewin, 400, 400);
    lv_obj_move_foreground(morewin);

    ta2 = lv_textarea_create(morewin);
    lv_obj_set_pos(ta2, 20, 20);
    lv_obj_set_size(ta2, 310, 60);
    lv_textarea_set_placeholder_text(ta2, "search records");
    lv_obj_add_event_cb(ta2, ta2_cb, LV_EVENT_FOCUSED, NULL);

    lv_obj_t *bt1 = lv_btn_create(morewin);
    lv_obj_t *bt2 = lv_btn_create(morewin);
    lv_obj_t *bt3 = lv_btn_create(morewin);
    lv_obj_t *bt4 = lv_btn_create(morewin);

    lv_obj_set_pos(bt1, 10, 95);
    lv_obj_set_pos(bt2, 10, 155);
    lv_obj_set_pos(bt3, 10, 215);
    lv_obj_set_pos(bt4, 10, 275);

    lv_obj_set_size(bt1, 250, 45);
    lv_obj_set_size(bt2, 250, 45);
    lv_obj_set_size(bt3, 250, 45);
    lv_obj_set_size(bt4, 250, 45);

    lv_obj_t *lb1 = lv_label_create(bt1);
    lv_obj_t *lb2 = lv_label_create(bt2);
    lv_obj_t *lb3 = lv_label_create(bt3);
    lv_obj_t *lb4 = lv_label_create(bt4);

    lv_obj_add_style(lb1,mystyle,0);
    lv_obj_add_style(lb2,mystyle,0);
    lv_obj_add_style(lb3,mystyle,0);
    lv_obj_add_style(lb4,mystyle,0);

    lv_label_set_text(lb1, "搜索");
    lv_label_set_text(lb2, "清空聊天记录");
    lv_label_set_text(lb3, "删除好友");
    lv_label_set_text(lb4, "关闭");

    lv_obj_center(lb1);
    lv_obj_center(lb2);
    lv_obj_center(lb3);
    lv_obj_center(lb4);

    lv_obj_add_event_cb(bt1, search_chat_record_cb, LV_EVENT_CLICKED, NULL);
    lv_obj_add_event_cb(bt2, clear_chat_record_cb, LV_EVENT_CLICKED, NULL);
    lv_obj_add_event_cb(bt3, delete_friend_cb, LV_EVENT_CLICKED, NULL);
    lv_obj_add_event_cb(bt4, close_more_cb, LV_EVENT_CLICKED, NULL);
}

//群聊更多功能按钮
void room_more_cb(lv_event_t * e)
{
    if(morewin != NULL)
        return;

    morewin = lv_obj_create(setwin);
    lv_obj_set_pos(morewin, 420, 60);
    lv_obj_set_size(morewin, 400, 400);
    lv_obj_move_foreground(morewin);

    ta2 = lv_textarea_create(morewin);
    lv_obj_set_pos(ta2, 20, 20);
    lv_obj_set_size(ta2, 310, 60);
    lv_textarea_set_placeholder_text(ta2, "search records");
    lv_obj_add_event_cb(ta2, ta2_cb, LV_EVENT_FOCUSED, NULL);

    lv_obj_t *bt1 = lv_btn_create(morewin);
    lv_obj_t *bt2 = lv_btn_create(morewin);
    lv_obj_t *bt3 = lv_btn_create(morewin);
    lv_obj_t *bt4 = lv_btn_create(morewin);

    lv_obj_set_pos(bt1, 20, 95);
    lv_obj_set_pos(bt2, 20, 155);
    lv_obj_set_pos(bt3, 20, 215);
    lv_obj_set_pos(bt4, 20, 275);

    lv_obj_set_size(bt1, 310, 45);
    lv_obj_set_size(bt2, 310, 45);
    lv_obj_set_size(bt3, 310, 45);
    lv_obj_set_size(bt4, 310, 45);

    lv_obj_t *lb1 = lv_label_create(bt1);
    lv_obj_t *lb2 = lv_label_create(bt2);
    lv_obj_t *lb3 = lv_label_create(bt3);
    lv_obj_t *lb4 = lv_label_create(bt4);

    lv_obj_add_style(lb1,mystyle,0);
    lv_obj_add_style(lb2,mystyle,0);
    lv_obj_add_style(lb3,mystyle,0);
    lv_obj_add_style(lb4,mystyle,0);

    lv_label_set_text(lb1, "搜索");
    lv_label_set_text(lb2, "清空聊天记录");
    lv_label_set_text(lb3, "退出群聊");
    lv_label_set_text(lb4, "关闭");

    lv_obj_center(lb1);
    lv_obj_center(lb2);
    lv_obj_center(lb3);
    lv_obj_center(lb4);

    lv_obj_add_event_cb(bt1, search_chat_record_cb, LV_EVENT_CLICKED, NULL);
    lv_obj_add_event_cb(bt2, clear_chat_record_cb, LV_EVENT_CLICKED, NULL);
    lv_obj_add_event_cb(bt3, set_back_main, LV_EVENT_CLICKED, NULL);
    lv_obj_add_event_cb(bt4, close_more_cb, LV_EVENT_CLICKED, NULL);
}

// 发起群聊确认按钮
void choose_room_ok_cb(lv_event_t * e)
{
    lv_obj_t *oldwin = setwin;
    //stp接入
    show_room();
    lv_obj_del(oldwin);
}


//点击加号弹出添加功能 
void add_friend_cb(lv_event_t * e)
{
    if(addwin != NULL)
        return;

    addwin = lv_obj_create(mainwin);
    lv_obj_set_pos(addwin, 470, 70);
    lv_obj_set_size(addwin, 300, 300);
    lv_obj_move_foreground(addwin);

    lv_obj_t *bt1 = lv_btn_create(addwin);
    lv_obj_t *bt2 = lv_btn_create(addwin);
    lv_obj_t *bt3 = lv_btn_create(addwin);
    lv_obj_t *bt4 = lv_btn_create(addwin);

    lv_obj_set_pos(bt1, 10, 20);
    lv_obj_set_pos(bt2, 10, 80);
    lv_obj_set_pos(bt3, 10, 140);
    lv_obj_set_pos(bt4, 10, 200);

    lv_obj_set_size(bt1, 240, 45);
    lv_obj_set_size(bt2, 240, 45);
    lv_obj_set_size(bt3, 240, 45);
    lv_obj_set_size(bt4, 240, 45);

    lv_obj_t *lb1 = lv_label_create(bt1);
    lv_obj_t *lb2 = lv_label_create(bt2);
    lv_obj_t *lb3 = lv_label_create(bt3);
    lv_obj_t *lb4 = lv_label_create(bt4);

    lv_obj_add_style(lb1,mystyle,0);
    lv_obj_add_style(lb2,mystyle,0);
    lv_obj_add_style(lb3,mystyle,0);
    lv_obj_add_style(lb4,mystyle,0);

    lv_label_set_text(lb1, "添加好友");
    lv_label_set_text(lb2, "IP连接");
    lv_label_set_text(lb3, "发起群聊");
    lv_label_set_text(lb4, "关闭");

    lv_obj_center(lb1);
    lv_obj_center(lb2);
    lv_obj_center(lb3);
    lv_obj_center(lb4);

    lv_obj_add_event_cb(bt1, add_account_cb, LV_EVENT_CLICKED, NULL);
    lv_obj_add_event_cb(bt2, add_ip_cb, LV_EVENT_CLICKED, NULL);
    lv_obj_add_event_cb(bt3, add_room_cb, LV_EVENT_CLICKED, NULL);
    lv_obj_add_event_cb(bt4, close_add_cb, LV_EVENT_CLICKED, NULL);
}

//使用IP连接按钮
void connect_ip_cb(lv_event_t * e)
{
    lv_obj_t *oldwin = setwin;
    const char *ip = lv_textarea_get_text(ta1);
    const char *port = lv_textarea_get_text(ta2);
    if(strlen(ip) == 0 || strlen(port) == 0)
        return;

    //stp接入
    show_chat();
    lv_obj_del(oldwin);
}

//搜索已经注册的账号添加好友按钮
void search_account_cb(lv_event_t * e)
{
    const char *msg = lv_textarea_get_text(ta1);
    if(strlen(msg) == 0)
        return;

    //stp接入
}

//通过ip连接的界面
void show_ip_connect()
{
    if(kb1 != NULL)
    {
        lv_keyboard_set_textarea(kb1, NULL);
        lv_obj_add_flag(kb1, LV_OBJ_FLAG_HIDDEN);
    }
    setwin = lv_obj_create(NULL);
    lv_obj_set_size(setwin, 800, 480);
    lv_obj_add_event_cb(setwin, close_kb_cb, LV_EVENT_CLICKED, NULL);

    lv_obj_t * lb1 = lv_label_create(setwin);
    lv_obj_set_pos(lb1, 100, 20);
    lv_obj_set_size(lb1, 600, 70);
    lv_obj_add_style(lb1,mystyle,0);
    lv_label_set_text(lb1, "IP连接");

    ta1 = lv_textarea_create(setwin);
    ta2 = lv_textarea_create(setwin);
    lv_obj_set_pos(ta1, 120, 120);
    lv_obj_set_pos(ta2, 120, 220);
    lv_obj_set_size(ta1, 560, 70);
    lv_obj_set_size(ta2, 560, 70);
    lv_textarea_set_placeholder_text(ta1, "请输入IP地址");
    lv_textarea_set_placeholder_text(ta2, "请输入端口号");
    lv_textarea_set_one_line(ta1, true);
    lv_textarea_set_one_line(ta2, true);
    lv_obj_add_event_cb(ta1, ta1_cb, LV_EVENT_FOCUSED, NULL);
    lv_obj_add_event_cb(ta2, ta2_cb, LV_EVENT_FOCUSED, NULL);

    lv_obj_t * bt1 = lv_btn_create(setwin); 
    lv_obj_set_pos(bt1, 550, 330);
    lv_obj_set_size(bt1, 120, 60);
    lv_obj_t * lb2 = lv_label_create(bt1);
    lv_obj_add_style(lb2,mystyle,0);
    lv_label_set_text(lb2, "连接");
    lv_obj_center(lb2);
    lv_obj_add_event_cb(bt1, connect_ip_cb, LV_EVENT_CLICKED, NULL);

    lv_obj_t * bt2 = lv_btn_create(setwin); 
    lv_obj_set_pos(bt2, 30, 30);
    lv_obj_set_size(bt2, 100, 50);
    lv_obj_t * lb3 = lv_label_create(bt2);
    lv_obj_add_style(lb3,mystyle,0);
    lv_label_set_text(lb3, "返回");
    lv_obj_center(lb3);
    lv_obj_add_event_cb(bt2, set_back_main, LV_EVENT_CLICKED, NULL);

    lv_scr_load(setwin);
}

//创建添加好友界面
void show_add_account()
{
    if(kb1 != NULL)
    {
        lv_keyboard_set_textarea(kb1, NULL);
        lv_obj_add_flag(kb1, LV_OBJ_FLAG_HIDDEN);
    }
    setwin = lv_obj_create(NULL);
    lv_obj_set_size(setwin, 800, 480);
    lv_obj_add_event_cb(setwin, close_kb_cb, LV_EVENT_CLICKED, NULL);

    lv_obj_t * lb1 = lv_label_create(setwin);
    lv_obj_set_pos(lb1, 100, 30);
    lv_obj_set_size(lb1, 600, 70);
    lv_obj_add_style(lb1,mystyle,0);
    lv_label_set_text(lb1, "添加好友");

    ta1 = lv_textarea_create(setwin);
    lv_obj_set_pos(ta1, 120, 150);
    lv_obj_set_size(ta1, 560, 70);
    lv_textarea_set_placeholder_text(ta1, "请输入好友账号");
    lv_textarea_set_one_line(ta1, true);
    lv_obj_add_event_cb(ta1, ta1_cb, LV_EVENT_FOCUSED, NULL);

    lv_obj_t * bt1 = lv_btn_create(setwin); 
    lv_obj_set_pos(bt1, 550, 260);
    lv_obj_set_size(bt1, 120, 60);
    lv_obj_t * lb2 = lv_label_create(bt1);
    lv_obj_add_style(lb2,mystyle,0);
    lv_label_set_text(lb2, "搜索");
    lv_obj_center(lb2);
    lv_obj_add_event_cb(bt1, search_account_cb, LV_EVENT_CLICKED, NULL);

    // 搜索结果显示区域
    lv_obj_t * result = lv_obj_create(setwin);
    lv_obj_set_pos(result, 120, 340);
    lv_obj_set_size(result, 560, 80);
    //stp接入

    lv_obj_t * bt2 = lv_btn_create(setwin); 
    lv_obj_set_pos(bt2, 30, 30);
    lv_obj_set_size(bt2, 100, 50);
    lv_obj_t * lb3 = lv_label_create(bt2);
    lv_obj_add_style(lb3,mystyle,0);
    lv_label_set_text(lb3, "返回");
    lv_obj_center(lb3);
    lv_obj_add_event_cb(bt2, set_back_main, LV_EVENT_CLICKED, NULL);

    lv_scr_load(setwin);
}

//创建发起群聊选择好友界面
void show_choose_room()
{
    if(kb1 != NULL)
    {
        lv_keyboard_set_textarea(kb1, NULL);
        lv_obj_add_flag(kb1, LV_OBJ_FLAG_HIDDEN);
    }
    setwin = lv_obj_create(NULL);
    lv_obj_set_size(setwin, 800, 480);

    lv_obj_t * lb1 = lv_label_create(setwin);
    lv_obj_set_pos(lb1, 100, 20);
    lv_obj_set_size(lb1, 600, 70);
    lv_obj_add_style(lb1,mystyle,0);
    lv_label_set_text(lb1, "选择好友发起群聊");

    // 好友选择区域
    lv_obj_t * list = lv_obj_create(setwin);
    lv_obj_set_pos(list, 50, 100);
    lv_obj_set_size(list, 700, 280);
    //stp接入
    // 获取好友之后在这里创建复选框

    lv_obj_t * bt1 = lv_btn_create(setwin); 
    lv_obj_set_pos(bt1, 600, 400);
    lv_obj_set_size(bt1, 120, 55);
    lv_obj_t * lb2 = lv_label_create(bt1);
    lv_obj_add_style(lb2,mystyle,0);
    lv_label_set_text(lb2, "确定");
    lv_obj_center(lb2);
    lv_obj_add_event_cb(bt1, choose_room_ok_cb, LV_EVENT_CLICKED, NULL);

    lv_obj_t * bt2 = lv_btn_create(setwin); 
    lv_obj_set_pos(bt2, 50, 400);
    lv_obj_set_size(bt2, 120, 55);
    lv_obj_t * lb3 = lv_label_create(bt2);
    lv_obj_add_style(lb3,mystyle,0);
    lv_label_set_text(lb3, "返回");
    lv_obj_center(lb3);
    lv_obj_add_event_cb(bt2, set_back_main, LV_EVENT_CLICKED, NULL);

    lv_scr_load(setwin);
}
//创建登入界面
void show_login()
{
    if(kb1 != NULL)
    {
        lv_keyboard_set_textarea(kb1, NULL);
        lv_obj_add_flag(kb1, LV_OBJ_FLAG_HIDDEN);
    }
    loginwin = lv_obj_create(NULL);
    lv_obj_set_size(loginwin, 800, 480);
    lv_obj_add_event_cb(loginwin, close_kb_cb, LV_EVENT_CLICKED, NULL);
    lv_obj_t * lb1 = lv_label_create(loginwin);
    lv_obj_set_pos(lb1, 100, 50);
    lv_obj_set_size(lb1, 600, 80);
    // 让标签使用中文样式
    lv_obj_add_style(lb1,mystyle,0);
    // 给标签设置文字
    lv_label_set_text(lb1, "欢迎来到英语聊天室");
    // 创建两个输入框：输入用户名和密码
    ta1 = lv_textarea_create(loginwin);
    ta2 = lv_textarea_create(loginwin);
    // 设置输入框坐标，大小
    lv_obj_set_pos(ta1, 120, 150);
    lv_obj_set_pos(ta2, 120, 250);
    lv_obj_set_size(ta1, 560, 80);
    lv_obj_set_size(ta2, 560, 80);
    // 让输入框使用中文样式
    lv_obj_add_style(ta1,mystyle,0);
    lv_obj_add_style(ta2,mystyle,0);
    // 设置文本输入默认提示文字
    lv_textarea_set_placeholder_text(ta1, "请输入用户名");
    lv_textarea_set_placeholder_text(ta2, "请输入密码");
    lv_textarea_set_password_mode(ta2, true);
    lv_textarea_set_one_line(ta1, true);
    lv_textarea_set_one_line(ta2, true);
    lv_obj_add_event_cb(ta1, ta1_cb, LV_EVENT_FOCUSED, NULL);
    lv_obj_add_event_cb(ta2, ta2_cb, LV_EVENT_FOCUSED, NULL);

    // 按钮就会在开发板液晶屏上显示
    lv_obj_t * bt1 = lv_btn_create(loginwin); 
    lv_obj_t * bt2 = lv_btn_create(loginwin);

    // 修改按钮的坐标位置
    lv_obj_set_pos(bt1, 100, 340);
    lv_obj_set_pos(bt2, 600, 340);

    // 修改按钮大小
    lv_obj_set_size(bt1, 100, 50);
    lv_obj_set_size(bt2, 100, 50);

    // 新建标签，把按钮作为父窗口
    lv_obj_t * lb2 = lv_label_create(bt1);
    lv_obj_t * lb3 = lv_label_create(bt2);

    // 标签设置中文样式
    lv_obj_add_style(lb2,mystyle,0);
    lv_obj_add_style(lb3,mystyle,0);

    // 给标签设置文字
    lv_label_set_text(lb2, "登录");
    lv_label_set_text(lb3, "注册");
    lv_obj_center(lb2);
    lv_obj_center(lb3);
     // 给按钮添加事件响应函数:  LV_EVENT_CLICKED枚举值表示按钮点击事件
    lv_obj_add_event_cb(bt1, log_go_main, LV_EVENT_CLICKED, NULL);
    lv_obj_add_event_cb(bt2, log_go_sig, LV_EVENT_CLICKED, NULL);

    //自己主动加载登录界面
    lv_scr_load(loginwin);

}

//创建注册界面
void show_sign()
{
    if(kb1 != NULL)
    {
        lv_keyboard_set_textarea(kb1, NULL);
        lv_obj_add_flag(kb1, LV_OBJ_FLAG_HIDDEN);
    }
    setwin = lv_obj_create(NULL);
    lv_obj_set_size(setwin, 800, 480);
    lv_obj_add_event_cb(setwin, close_kb_cb, LV_EVENT_CLICKED, NULL);
    lv_obj_t * lb1 = lv_label_create(setwin);
    lv_obj_set_pos(lb1, 100, 50);
    lv_obj_set_size(lb1, 600, 80);
    // 让标签使用中文样式
    lv_obj_add_style(lb1,mystyle,0);
    // 给标签设置文字
    lv_label_set_text(lb1, "注册界面");
   
    // 创建两个输入框：输入用户名和密码
    ta1 = lv_textarea_create(setwin);
    ta2 = lv_textarea_create(setwin);
    // 设置输入框坐标，大小
    lv_obj_set_pos(ta1, 120, 150);
    lv_obj_set_pos(ta2, 120, 250);
    lv_obj_set_size(ta1, 560, 80);
    lv_obj_set_size(ta2, 560, 80);
    // 让输入框使用中文样式
    lv_obj_add_style(ta1,mystyle,0);
    lv_obj_add_style(ta2,mystyle,0);
    // 设置文本输入默认提示文字
    lv_textarea_set_placeholder_text(ta1, "请输入用户名");
    lv_textarea_set_placeholder_text(ta2, "请输入密码");
    lv_textarea_set_password_mode(ta2, true);
    lv_textarea_set_one_line(ta1, true);
    lv_textarea_set_one_line(ta2, true);
    // 给输入框添加事件响应函数，事件类型--》光标进入事件LV_EVENT_FOCUSED
    lv_obj_add_event_cb(ta1, ta1_cb, LV_EVENT_FOCUSED, NULL);
    lv_obj_add_event_cb(ta2, ta2_cb, LV_EVENT_FOCUSED, NULL);

    // 按钮就会在开发板液晶屏上显示
    lv_obj_t * bt1 = lv_btn_create(setwin); 
    lv_obj_t * bt2 = lv_btn_create(setwin);

    // 修改按钮的坐标位置
    lv_obj_set_pos(bt1, 100, 340);
    lv_obj_set_pos(bt2, 600, 340);

    // 修改按钮大小
    lv_obj_set_size(bt1, 100, 50);
    lv_obj_set_size(bt2, 100, 50);

    // 新建标签，把按钮作为父窗口
    lv_obj_t * lb2 = lv_label_create(bt1);
    lv_obj_t * lb3 = lv_label_create(bt2);

    // 标签设置中文样式
    lv_obj_add_style(lb2,mystyle,0);
    lv_obj_add_style(lb3,mystyle,0);

    // 给标签设置文字
    lv_label_set_text(lb2, "返回");
    lv_label_set_text(lb3, "注册");
    lv_obj_center(lb2);
    lv_obj_center(lb3);

     // 给按钮添加事件响应函数:  LV_EVENT_CLICKED枚举值表示按钮点击事件
    lv_obj_add_event_cb(bt1, set_back_log, LV_EVENT_CLICKED, NULL);
    lv_obj_add_event_cb(bt2, set_go_main, LV_EVENT_CLICKED, NULL);
    //自己主动加载注册界面
    lv_scr_load(setwin);

}

//创建聊天界面
void show_chat()
{
    if(kb1 != NULL)
    {
        lv_keyboard_set_textarea(kb1, NULL);
        lv_obj_add_flag(kb1, LV_OBJ_FLAG_HIDDEN);
    }
    morewin = NULL;
    roommsg = NULL;
    setwin = lv_obj_create(NULL);
    lv_obj_set_size(setwin, 800, 480);
    lv_obj_add_event_cb(setwin, close_kb_cb, LV_EVENT_CLICKED, NULL);

    lv_obj_t * lb1 = lv_label_create(setwin);
    lv_obj_set_pos(lb1, 100, 10);
    lv_obj_set_size(lb1, 600, 50);
    // 让标签使用中文样式
    lv_obj_add_style(lb1,mystyle,0);
    // 给标签设置文字 显示好友信息
    lv_label_set_text(lb1, "chat with friend");
    //stp接入

    // 创建消息显示框
    chatmsg = lv_textarea_create(setwin);
    lv_obj_set_pos(chatmsg, 30, 70);
    lv_obj_set_size(chatmsg, 740, 260);
    lv_textarea_set_text(chatmsg, "");
    lv_textarea_set_cursor_click_pos(chatmsg, false);
    //stp接入
   
    // 创建输入框：输入要聊天的信息 
    ta1 = lv_textarea_create(setwin);
    lv_obj_set_pos(ta1, 120, 350);
    lv_obj_set_size(ta1, 480, 70);
    lv_textarea_set_placeholder_text(ta1, "send message");
    lv_obj_add_event_cb(ta1, ta1_cb, LV_EVENT_FOCUSED, NULL);

    // 发送按钮
    lv_obj_t * sendbt = lv_btn_create(setwin);
    lv_obj_set_pos(sendbt, 620, 350);
    lv_obj_set_size(sendbt, 100, 70);
    lv_obj_t * sendlb = lv_label_create(sendbt);
    lv_obj_add_style(sendlb,mystyle,0);
    lv_label_set_text(sendlb, "send");
    lv_obj_center(sendlb);
    lv_obj_add_event_cb(sendbt, send_chat_cb, LV_EVENT_CLICKED, NULL);

    // 返回按钮
    lv_obj_t * backbt = lv_btn_create(setwin);
    lv_obj_set_pos(backbt, 20, 10);
    lv_obj_set_size(backbt, 70, 45);
    lv_obj_t * backlb = lv_label_create(backbt);
    lv_obj_add_style(backlb,mystyle,0);
    lv_label_set_text(backlb, "return");
    lv_obj_center(backlb);
    lv_obj_add_event_cb(backbt, set_back_main, LV_EVENT_CLICKED, NULL);

    // 用来显示聊天界面的额外功能 里面有一个文本框 用来搜索聊天记录 有一个按钮用来清空聊天记录 有一个按钮 用来删除好友 
    lv_obj_t * bt1 = lv_btn_create(setwin); 
    lv_obj_set_pos(bt1, 710, 10);
    lv_obj_set_size(bt1, 60, 45);
    lv_obj_t * lb2 = lv_label_create(bt1);
    lv_obj_add_style(lb2,mystyle,0);
    lv_label_set_text(lb2, "...");
    lv_obj_center(lb2);
    lv_obj_add_event_cb(bt1, chat_more_cb, LV_EVENT_CLICKED, NULL);

    //自己主动加载聊天界面
    lv_scr_load(setwin);
}

//创建群聊界面
void show_room()
{
    if(kb1 != NULL)
    {
        lv_keyboard_set_textarea(kb1, NULL);
        lv_obj_add_flag(kb1, LV_OBJ_FLAG_HIDDEN);
    }
    morewin = NULL;
    chatmsg = NULL;
    setwin = lv_obj_create(NULL);
    lv_obj_set_size(setwin, 800, 480);
    lv_obj_add_event_cb(setwin, close_kb_cb, LV_EVENT_CLICKED, NULL);

    lv_obj_t * lb1 = lv_label_create(setwin);
    lv_obj_set_pos(lb1, 100, 10);
    lv_obj_set_size(lb1, 600, 50);
    // 让标签使用中文样式
    lv_obj_add_style(lb1,mystyle,0);
    // 给标签设置文字 显示群聊信息 
    lv_label_set_text(lb1, "chat room");
    //stp接入

    // 创建群聊消息显示框
    roommsg = lv_textarea_create(setwin);
    lv_obj_set_pos(roommsg, 30, 70);
    lv_obj_set_size(roommsg, 740, 260);
    lv_textarea_set_text(roommsg, "");
    lv_textarea_set_cursor_click_pos(roommsg, false);
    //stp接入
   
    // 创建输入框：输入要聊天的信息 
    ta1 = lv_textarea_create(setwin);
    lv_obj_set_pos(ta1, 120, 350);
    lv_obj_set_size(ta1, 480, 70);
    lv_textarea_set_placeholder_text(ta1, "send message");
    lv_obj_add_event_cb(ta1, ta1_cb, LV_EVENT_FOCUSED, NULL);

    // 发送按钮
    lv_obj_t * sendbt = lv_btn_create(setwin);
    lv_obj_set_pos(sendbt, 620, 350);
    lv_obj_set_size(sendbt, 100, 70);
    lv_obj_t * sendlb = lv_label_create(sendbt);
    lv_obj_add_style(sendlb,mystyle,0);
    lv_label_set_text(sendlb, "发送");
    lv_obj_center(sendlb);
    lv_obj_add_event_cb(sendbt, send_room_cb, LV_EVENT_CLICKED, NULL);

    // 返回按钮
    lv_obj_t * backbt = lv_btn_create(setwin);
    lv_obj_set_pos(backbt, 20, 10);
    lv_obj_set_size(backbt, 70, 45);
    lv_obj_t * backlb = lv_label_create(backbt);
    lv_obj_add_style(backlb,mystyle,0);
    lv_label_set_text(backlb, "返回");
    lv_obj_center(backlb);
    lv_obj_add_event_cb(backbt, set_back_main, LV_EVENT_CLICKED, NULL);

    // 用来显示聊天界面的额外功能 里面有一个文本框 用来搜索聊天记录 有一个按钮用来清空聊天记录 有一个按钮 用来退出群聊 
    lv_obj_t * bt1 = lv_btn_create(setwin); 
    lv_obj_set_pos(bt1, 710, 10);
    lv_obj_set_size(bt1, 60, 45);
    lv_obj_t * lb2 = lv_label_create(bt1);
    lv_obj_add_style(lb2,mystyle,0);
    lv_label_set_text(lb2, "...");
    lv_obj_center(lb2);
    lv_obj_add_event_cb(bt1, room_more_cb, LV_EVENT_CLICKED, NULL);

    //自己主动加载群聊界面
    lv_scr_load(setwin);
}

//创建好友界面 用来展示有多少好友 ,同时点击好友可以进入聊天界面
void show_frd()
{
    if(kb1 != NULL)
    {
        lv_keyboard_set_textarea(kb1, NULL);
        lv_obj_add_flag(kb1, LV_OBJ_FLAG_HIDDEN);
    }
    setwin = lv_obj_create(NULL);
    lv_obj_set_size(setwin, 800, 480);

    lv_obj_t * lb1 = lv_label_create(setwin);
    lv_obj_set_pos(lb1, 100, 20);
    lv_obj_set_size(lb1, 600, 60);
    // 让标签使用中文样式
    lv_obj_add_style(lb1,mystyle,0);
    // 给标签设置文字 显示好友信息 
    lv_label_set_text(lb1, "friend list");

    // 显示好友数量
    lv_obj_t * numlb = lv_label_create(setwin);
    lv_obj_set_pos(numlb, 30, 90);
    lv_obj_set_size(numlb, 740, 50);
    lv_obj_add_style(numlb,mystyle,0);
    lv_label_set_text(numlb, "好友数量：");
    //stp接入

    // 创建好友列表区域
    lv_obj_t * list = lv_list_create(setwin);
    lv_obj_set_pos(list, 30, 145);
    lv_obj_set_size(list, 740, 260);
    //stp接入
    // 获取好友之后使用lv_list_add_btn创建好友按钮
    // 好友按钮点击事件使用friend_chat_cb

    // 返回按钮
    lv_obj_t * bt1 = lv_btn_create(setwin); 
    lv_obj_set_pos(bt1, 30, 415);
    lv_obj_set_size(bt1, 100, 50);
    lv_obj_t * lb2 = lv_label_create(bt1);
    lv_obj_add_style(lb2,mystyle,0);
    lv_label_set_text(lb2, "返回");
    lv_obj_center(lb2);
    lv_obj_add_event_cb(bt1, set_back_main, LV_EVENT_CLICKED, NULL);

    //自己主动加载好友界面
    lv_scr_load(setwin);
}

//创建设置界面 
void show_set()
{
    if(kb1 != NULL)
    {
        lv_keyboard_set_textarea(kb1, NULL);
        lv_obj_add_flag(kb1, LV_OBJ_FLAG_HIDDEN);
    }
    setwin = lv_obj_create(NULL);
    lv_obj_set_size(setwin, 800, 480);

    lv_obj_t * lb1 = lv_label_create(setwin);
    lv_obj_set_pos(lb1, 100, 30);
    lv_obj_set_size(lb1, 600, 70);
    // 让标签使用中文样式
    lv_obj_add_style(lb1,mystyle,0);
    // 给标签设置文字 显示好友信息 
    lv_label_set_text(lb1, "settings");

    // 退出登录按钮
    lv_obj_t * bt1 = lv_btn_create(setwin); 
    lv_obj_set_pos(bt1, 250, 150);
    lv_obj_set_size(bt1, 300, 70);
    lv_obj_t * lb2 = lv_label_create(bt1);
    lv_obj_add_style(lb2,mystyle,0);
    lv_label_set_text(lb2, "退出登录");
    lv_obj_center(lb2);
    lv_obj_add_event_cb(bt1, set_back_login, LV_EVENT_CLICKED, NULL);

    // 反馈按钮
    lv_obj_t * bt2 = lv_btn_create(setwin); 
    lv_obj_set_pos(bt2, 250, 250);
    lv_obj_set_size(bt2, 300, 70);
    lv_obj_t * lb3 = lv_label_create(bt2);
    lv_obj_add_style(lb3,mystyle,0);
    lv_label_set_text(lb3, "反馈");
    lv_obj_center(lb3);
    lv_obj_add_event_cb(bt2, set_back_feedback_cb, LV_EVENT_CLICKED, NULL);

    // 返回按钮
    lv_obj_t * bt3 = lv_btn_create(setwin); 
    lv_obj_set_pos(bt3, 30, 400);
    lv_obj_set_size(bt3, 100, 50);
    lv_obj_t * lb4 = lv_label_create(bt3);
    lv_obj_add_style(lb4,mystyle,0);
    lv_label_set_text(lb4, "返回");
    lv_obj_center(lb4);
    lv_obj_add_event_cb(bt3, set_back_main, LV_EVENT_CLICKED, NULL);

    //自己主动加载设置界面
    lv_scr_load(setwin);
}

//显示主界面 
void show_main(void)
{
    if(kb1 != NULL)
    {
        lv_keyboard_set_textarea(kb1, NULL);
        lv_obj_add_flag(kb1, LV_OBJ_FLAG_HIDDEN);
    }
    addwin = NULL;
    morewin = NULL;
    chatmsg = NULL;
    roommsg = NULL;
    mainwin = lv_obj_create(NULL);
    lv_obj_set_size(mainwin, 800, 480);
    lv_obj_t *title = lv_label_create(mainwin);
    lv_obj_set_pos(title, 80, 40);
    lv_obj_set_size(title, 640, 80);
    lv_obj_add_style(title, mystyle, 0);
    lv_label_set_text(title, "Welcome chat room");

    //搜索界面 
    ta1 = lv_textarea_create(mainwin);
    // 设置输入框坐标，大小
    lv_obj_set_pos(ta1, 70, 50);
    lv_obj_set_size(ta1,150, 50);
    // 设置文本输入默认提示文字
    lv_textarea_set_placeholder_text(ta1, "Search");
    //点击之后弹出软键盘
    lv_obj_add_event_cb(ta1, ta1_cb, LV_EVENT_FOCUSED, NULL);
    //点击空白处关闭软键盘
    lv_obj_add_event_cb(mainwin, close_kb_cb, LV_EVENT_CLICKED, NULL);
    //输入后弹出相关人员信息
    //点击可以进入聊天界面
    
    //创建添加好友按钮
    lv_obj_t *bt4 = lv_btn_create(mainwin);
    lv_obj_set_pos(bt4, 230, 50);
    lv_obj_set_size(bt4, 50, 50);
    lv_obj_t *lb4 = lv_label_create(bt4);
    lv_label_set_text(lb4, "+");
    lv_obj_center(lb4);
    //点击后弹出添加好友界面以弹框形式出现
    lv_obj_add_event_cb(bt4, add_friend_cb, LV_EVENT_CLICKED, NULL);
    //点击空白处关闭添加好友界面
    lv_obj_add_event_cb(mainwin, close_add_cb, LV_EVENT_CLICKED, NULL);
    //添加好友界面有三个功能
    //yi.输入好友账号，点击搜索，弹出相关信息
    //er.以IP连接，点击后进入ip连接界面 有两个输入框 一个输入框输入IP地址，另一个输入框输入端口号，点击连接后进入聊天界面
    //san.发起群聊 点击后弹出好友列表 用来勾选1个或多个好友，点击确定后进入群聊界面

    //创建三个按钮 用来弹出到不同界面
    lv_obj_t *bt1 = lv_btn_create(mainwin);//聊天场所
    lv_obj_t *bt2 = lv_btn_create(mainwin);//好友列表
    lv_obj_t *bt3 = lv_btn_create(mainwin);//设置界面
    lv_obj_set_pos(bt1, 0, 100);
    lv_obj_set_pos(bt2, 0, 200);
    lv_obj_set_pos(bt3, 0, 300);
    lv_obj_set_size(bt1, 70, 70);
    lv_obj_set_size(bt2, 70, 70);
    lv_obj_set_size(bt3, 70, 70);
    //增加标签
    lv_obj_t *lb1 = lv_label_create(bt1);
    lv_obj_t *lb2 = lv_label_create(bt2);
    lv_obj_t *lb3 = lv_label_create(bt3);
    lv_label_set_text(lb1, "chatroom");
    lv_label_set_text(lb2, "personal");
    lv_label_set_text(lb3, "settings");
    lv_obj_center(lb1);
    lv_obj_center(lb2);
    lv_obj_center(lb3);
    //增加事件
    lv_obj_add_event_cb(bt1, main_go_chat, LV_EVENT_CLICKED, NULL);
    lv_obj_add_event_cb(bt2, main_go_frd , LV_EVENT_CLICKED, NULL);
    lv_obj_add_event_cb(bt3, main_go_set, LV_EVENT_CLICKED, NULL);

    lv_scr_load(mainwin);
}

int main(void)
{
    lv_init();

    fbdev_init();

    static lv_color_t buf[DISP_BUF_SIZE];

    static lv_disp_draw_buf_t disp_buf;
    lv_disp_draw_buf_init(&disp_buf, buf, NULL, DISP_BUF_SIZE);

    static lv_disp_drv_t disp_drv;
    lv_disp_drv_init(&disp_drv);
    disp_drv.draw_buf = &disp_buf;
    disp_drv.flush_cb = fbdev_flush;
    disp_drv.hor_res = 800;
    disp_drv.ver_res = 480;
    lv_disp_drv_register(&disp_drv);

    evdev_init();

    static lv_indev_drv_t indev_drv_1;
    lv_indev_drv_init(&indev_drv_1);
    indev_drv_1.type = LV_INDEV_TYPE_POINTER;
    indev_drv_1.read_cb = evdev_read;
    lv_indev_drv_register(&indev_drv_1);

    mystyle = create_chinese_style("/font/simkai.ttf", 34);

    kb1 = lv_keyboard_create(lv_layer_top());
    lv_obj_set_size(kb1, 800, 200);
    lv_obj_align(kb1, LV_ALIGN_BOTTOM_MID, 0, 0);
    lv_keyboard_set_mode(kb1, LV_KEYBOARD_MODE_TEXT_LOWER);
    lv_obj_add_flag(kb1, LV_OBJ_FLAG_HIDDEN);
    lv_obj_add_event_cb(kb1, close_kb_cb, LV_EVENT_CANCEL, NULL);
    lv_obj_add_event_cb(kb1, close_kb_cb, LV_EVENT_READY, NULL);

    show_login();

    while(1)
    {
        lv_timer_handler();
        usleep(5000);
    }

    return 0;
}

/* Set in lv_conf.h as `LV_TICK_CUSTOM_SYS_TIME_EXPR` */
uint32_t custom_tick_get(void)
{
    static uint64_t start_ms = 0;
    if(start_ms == 0)
    {
        struct timeval tv_start;
        gettimeofday(&tv_start, NULL);
        start_ms = (tv_start.tv_sec * 1000000 + tv_start.tv_usec) / 1000;
    }

    struct timeval tv_now;
    gettimeofday(&tv_now, NULL);
    uint64_t now_ms;
    now_ms = (tv_now.tv_sec * 1000000 + tv_now.tv_usec) / 1000;

    uint32_t time_ms = now_ms - start_ms;
    return time_ms;
}
