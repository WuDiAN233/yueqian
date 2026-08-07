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
#include "lv_100ask_pinyin_ime.h"

#define DISP_BUF_SIZE (1024 * 900)

#define TEST_MAGIC 'x'
#define LED1 _IO(TEST_MAGIC, 0)
#define LED2 _IO(TEST_MAGIC, 1)
#define LED3 _IO(TEST_MAGIC, 2)
#define LED4 _IO(TEST_MAGIC, 3)

#define LED_ON  0
#define LED_OFF 1

/* 商品结构体：一个商品有 名字、数量、价格 */
typedef struct product {
    char name[50];
    int num;
    float price;
} Product;
lv_obj_t * kb1; // 软键盘的地址
lv_obj_t * ta1; // 文本输入框的地址
lv_obj_t * ta2; // 文本输入框的地址
lv_obj_t *loginwin; // 登录界面的地址
lv_obj_t *mainwin;   // 主界面
lv_obj_t *setwin;   // 注册界面
lv_style_t *mystyle; // 中文字体样式
lv_obj_t *pinyin_ime;


/* 先声明函数，下面才能互相调用 */
lv_style_t *create_chinese_style(char *fontpath, int fontsize);
void show_main(void);
void show_login(void);
void show_set(void);
int read_product(Product p[], int max);
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

/* 读取msg.txt里面的商品信息 */
int read_product(Product p[], int max)
{
    FILE *fp;
    char line[100];
    int i = 0;

    fp = fopen("./msg.txt", "r");
    if(fp == NULL)
    {
        perror("open msg.txt");
        return 0;
    }

    while(fgets(line, sizeof(line), fp) != NULL && i < max)
    {
        /* 一行格式：王老吉@15@3.5 */
        if(sscanf(line, "%[^@]@%d@%f", p[i].name, &p[i].num, &p[i].price) == 3)
        {
            i++;
        }
    }

    fclose(fp);
    return i;
}
//这里凡是要聊天的输入框都要关联软键盘，输入框1的事件响应函数 即用来与别人聊天的输入框 兵器点击空白处需要关闭软键盘
void ta1_cb(lv_event_t * e)
{
    // 把软键盘跟第一个输入框关联
    lv_100ask_pinyin_ime_attach(kb1, ta1);
}

// 输入框2的事件响应函数
void ta2_cb(lv_event_t * e)
{
    // 把软键盘跟第二个输入框关联
    lv_100ask_pinyin_ime_attach(kb1, ta2);
}

/* 主界面 -> 注册界面 */
void go_log_set(lv_event_t * e)
{
    show_set();
    lv_obj_del(loginwin);
}

/*登入界面 ->主界面*/
void go_log_fun(lv_event_t * e) // 登录按钮
{
    show_main();
    lv_obj_del(loginwin);
}

/*登入界面 ->注册面*/
void go_set_log(lv_event_t * e) // 登录按钮
{
    // 由于现在没有做软键盘，直接跳到主界面
    // 显示主界面
    show_set();
    lv_obj_del(loginwin);
}

/*注册界面 ->主菜单*/
void go_set_main(lv_event_t * e) // 登录按钮
{
    // 由于现在没有做软键盘，直接跳到主界面
    // 显示主界面
    show_main();
    lv_obj_del(setwin);
}


/* 主界面 -> 登入界面  */
void main_back_log(lv_event_t * e)
{
    show_login();
    lv_obj_del(mainwin);
}

/* 注册界面 -> 登入登录 */
void set_back_log(lv_event_t * e)
{
    show_login();
    lv_obj_del(setwin);
}

/*创建登入界面*/
void show_login()
{
    loginwin = lv_obj_create(NULL);
    lv_obj_set_size(loginwin, 800, 480);
    lv_obj_t * lb1 = lv_label_create(loginwin);
    lv_obj_set_pos(lb1, 100, 50);
    lv_obj_set_size(lb1, 600, 80);
    // 让标签使用中文样式
    lv_obj_add_style(lb1,mystyle,0);
    // 给标签设置文字
    lv_label_set_text(lb1, "欢迎来到英语聊天室");
    // 创建两个输入框：输入用户名和密码
    lv_obj_t * ta1 = lv_textarea_create(loginwin);
    lv_obj_t * ta2 = lv_textarea_create(loginwin);
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

    // 按钮就会在开发板液晶屏上显示
    lv_obj_t * bt1 = lv_btn_create(loginwin); 
    lv_obj_t * bt2 = lv_btn_create(loginwin);

    // 修改按钮的坐标位置
    lv_obj_set_pos(bt1, 100, 340);
    lv_obj_set_pos(bt2, 650, 340);

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

     // 给按钮添加事件响应函数:  LV_EVENT_CLICKED枚举值表示按钮点击事件
    lv_obj_add_event_cb(bt1, go_log_fun, LV_EVENT_CLICKED, NULL);
    lv_obj_add_event_cb(bt2, go_log_set, LV_EVENT_CLICKED, NULL);

    //自己主动加载登录界面
    lv_scr_load(loginwin);

}

/*创建注册界面*/
void show_sign()
{
    setwin = lv_obj_create(NULL);
    lv_obj_set_size(setwin, 800, 480);
    lv_obj_t * lb1 = lv_label_create(setwin);
    lv_obj_set_pos(lb1, 100, 50);
    lv_obj_set_size(lb1, 600, 80);
    // 让标签使用中文样式
    lv_obj_add_style(lb1,mystyle,0);
    // 给标签设置文字
    lv_label_set_text(lb1, "注册界面");
   
    // 创建两个输入框：输入用户名和密码
    lv_obj_t * ta1 = lv_textarea_create(setwin);
    lv_obj_t * ta2 = lv_textarea_create(setwin);
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
    
    // 给输入框添加事件响应函数，事件类型--》光标进入事件LV_EVENT_FOCUSED
    lv_obj_add_event_cb(ta1, ta1_cb, LV_EVENT_FOCUSED, NULL);
    lv_obj_add_event_cb(ta2, ta2_cb, LV_EVENT_FOCUSED, NULL);

    // 按钮就会在开发板液晶屏上显示
    lv_obj_t * bt1 = lv_btn_create(setwin); 
    lv_obj_t * bt2 = lv_btn_create(setwin);

    // 修改按钮的坐标位置
    lv_obj_set_pos(bt1, 100, 340);
    lv_obj_set_pos(bt2, 650, 340);

    // 修改按钮大小
    lv_obj_set_size(bt1, 200, 50);
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

     // 给按钮添加事件响应函数:  LV_EVENT_CLICKED枚举值表示按钮点击事件
    lv_obj_add_event_cb(bt1, set_back_log, LV_EVENT_CLICKED, NULL);
    lv_obj_add_event_cb(bt2, go_set_main, LV_EVENT_CLICKED, NULL);
    //自己主动加载注册界面
    lv_scr_load(setwin);

}

//创建聊天界面
void show_chat()
{
    setwin = lv_obj_create(NULL);
    lv_obj_set_size(setwin, 800, 480);
    lv_obj_t * lb1 = lv_label_create(setwin);
    lv_obj_set_pos(lb1, 100, 50);
    lv_obj_set_size(lb1, 600, 80);
    // 让标签使用中文样式
    lv_obj_add_style(lb1,mystyle,0);
    // 给标签设置文字 显示好友信息 
    lv_label_set_text(lb1, "聊天界面");
   
    // 创建输入框：输入要聊天的信息 
    lv_obj_t * ta1 = lv_textarea_create(setwin);
    lv_obj_set_pos(ta1, 120, 150);
    lv_obj_set_size(ta1, 560, 80);
    lv_textarea_set_placeholder_text(ta1, "send message");
    lv_obj_add_event_cb(ta1, ta1_cb, LV_EVENT_FOCUSED, NULL);

    // 用来显示聊天界面的额外功能 里面有一个文本框 用来搜索聊天记录 有一个按钮用来清空聊天记录 有一个按钮 用来删除好友 
    lv_obj_t * bt1 = lv_btn_create(setwin); 
    lv_obj_set_pos(bt1, 100, 340);
    lv_obj_set_size(bt1, 200, 50);
    lv_obj_t * lb2 = lv_label_create(bt1);
    lv_obj_add_style(lb2,mystyle,0);
    lv_label_set_text(lb2, "...");
    lv_obj_add_event_cb(bt1, set_back_log, LV_EVENT_CLICKED, NULL);

    //自己主动加载注册界面
    lv_scr_load(setwin);

}
//创建群聊界面
void show_room()
{
    setwin = lv_obj_create(NULL);
    lv_obj_set_size(setwin, 800, 480);
    lv_obj_t * lb1 = lv_label_create(setwin);
    lv_obj_set_pos(lb1, 100, 50);
    lv_obj_set_size(lb1, 600, 80);
    // 让标签使用中文样式
    lv_obj_add_style(lb1,mystyle,0);
    // 给标签设置文字 显示好友信息 
    lv_label_set_text(lb1, "chat room");
   
    // 创建输入框：输入要聊天的信息 
    lv_obj_t * ta1 = lv_textarea_create(setwin);
    lv_obj_set_pos(ta1, 120, 150);
    lv_obj_set_size(ta1, 560, 80);
    lv_textarea_set_placeholder_text(ta1, "send message");
    lv_obj_add_event_cb(ta1, ta1_cb, LV_EVENT_FOCUSED, NULL);

    // 用来显示聊天界面的额外功能 里面有一个文本框 用来搜索聊天记录 有一个按钮用来清空聊天记录 有一个按钮 用来退出群聊 
    lv_obj_t * bt1 = lv_btn_create(setwin); 
    lv_obj_set_pos(bt1, 100, 340);
    lv_obj_set_size(bt1, 200, 50);
    lv_obj_t * lb2 = lv_label_create(bt1);
    lv_obj_add_style(lb2,mystyle,0);
    lv_label_set_text(lb2, "...");
    lv_obj_add_event_cb(bt1, set_back_log, LV_EVENT_CLICKED, NULL);

    //自己主动加载注册界面
    lv_scr_load(setwin);

}
//创建好友界面 用来展示有多少好友 ,同时点击好友可以进入聊天界面
void show_frd()
{
    setwin = lv_obj_create(NULL);
    lv_obj_set_size(setwin, 800, 480);
    lv_obj_t * lb1 = lv_label_create(setwin);
    lv_obj_set_pos(lb1, 100, 50);
    lv_obj_set_size(lb1, 600, 80);
    // 让标签使用中文样式
    lv_obj_add_style(lb1,mystyle,0);
    // 给标签设置文字 显示好友信息 
    lv_label_set_text(lb1, "friend list");
   
    // 创建输入框：输入要聊天的信息 
    lv_obj_t * ta1 = lv_textarea_create(setwin);
    lv_obj_set_pos(ta1, 120, 150);
    lv_obj_set_size(ta1, 560, 80);
    lv_textarea_set_placeholder_text(ta1, "send message");
    lv_obj_add_event_cb(ta1, ta1_cb, LV_EVENT_FOCUSED, NULL);

    // 用来显示聊天界面的额外功能 里面有一个文本框 用来搜索聊天记录 有一个按钮用来清空聊天记录 有一个按钮 用来退出群聊 
    lv_obj_t * bt1 = lv_btn_create(setwin); 
    lv_obj_set_pos(bt1, 100, 340);
    lv_obj_set_size(bt1, 200, 50);
    lv_obj_t * lb2 = lv_label_create(bt1);
    lv_obj_add_style(lb2,mystyle,0);
    lv_label_set_text(lb2, "...");
    lv_obj_add_event_cb(bt1, set_back_log, LV_EVENT_CLICKED, NULL);

    //自己主动加载注册界面
    lv_scr_load(setwin);

}
//创建设置界面 用来退出登录 这里只弹出一个弹框 里面有一个退出按钮与反馈按钮 反馈按钮即弹出一个反馈界面 然后写的内容添加到记事本中 
void show_set()
{
    setwin = lv_obj_create(NULL);
    lv_obj_set_size(setwin, 800, 480);
    lv_obj_t * lb1 = lv_label_create(setwin);
    lv_obj_set_pos(lb1, 100, 50);
    lv_obj_set_size(lb1, 600, 80);
    // 让标签使用中文样式
    lv_obj_add_style(lb1,mystyle,0);
    // 给标签设置文字 显示好友信息 
    lv_label_set_text(lb1, "settings");
   
    // 创建输入框：输入要聊天的信息 
    lv_obj_t * ta1 = lv_textarea_create(setwin);
    lv_obj_set_pos(ta1, 120, 150);
    lv_obj_set_size(ta1, 560, 80);
    lv_textarea_set_placeholder_text(ta1, "send message");
    lv_obj_add_event_cb(ta1, ta1_cb, LV_EVENT_FOCUSED, NULL);

    // 用来显示聊天界面的额外功能 里面有一个文本框 用来搜索聊天记录 有一个按钮用来清空聊天记录 有一个按钮 用来退出群聊 
    lv_obj_t * bt1 = lv_btn_create(setwin); 
    lv_obj_set_pos(bt1, 100, 340);
    lv_obj_set_size(bt1, 200, 50);
    lv_obj_t * lb2 = lv_label_create(bt1);
    lv_obj_add_style(lb2,mystyle,0);
    lv_label_set_text(lb2, "...");
    lv_obj_add_event_cb(bt1, set_back_log, LV_EVENT_CLICKED, NULL);

    //自己主动加载注册界面
    lv_scr_load(setwin);

}


/* 显示主界面 */
void show_main(void)
{
    mainwin = lv_obj_create(NULL);
    lv_obj_set_size(mainwin, 800, 480);
    lv_obj_t *title = lv_label_create(mainwin);
    lv_obj_set_pos(title, 80, 40);
    lv_obj_set_size(title, 640, 80);
    lv_obj_add_style(title, mystyle, 0);
    lv_label_set_text(title, "Welcome to the English chat room");

    //搜索界面 
    lv_obj_t * ta1 = lv_textarea_create(mainwin);
    // 设置输入框坐标，大小
    lv_obj_set_pos(ta1, 20, 50);
    lv_obj_set_size(ta1,100, 50);
    // 设置文本输入默认提示文字
    lv_textarea_set_placeholder_text(ta1, "Search");
    //点击之后弹出软键盘
    lv_obj_add_event_cb(ta1, ta1_cb, LV_EVENT_FOCUSED, NULL);
    //点击空白处关闭软键盘
    lv_obj_add_event_cb(mainwin, ta1_cb, LV_EVENT_CLICKED, NULL);
    //输入后弹出相关人员信息
    //点击可以进入聊天界面
    
    //创建添加好友按钮
    lv_obj_t *bt4 = lv_btn_create(mainwin);
    lv_obj_set_pos(bt4, 50, 50);
    lv_obj_set_size(bt4, 150, 50);
    lv_obj_t *lb4 = lv_label_create(bt4);
    lv_label_set_text(lb4, "+");
    lv_obj_center(lb4);
    //点击后弹出添加好友界面以弹框形式出现
    lv_obj_add_event_cb(bt4, add_friend_cb, LV_EVENT_CLICKED, NULL);
    //点击空白处关闭添加好友界面
    lv_obj_add_event_cb(mainwin, add_friend_cb, LV_EVENT_CLICKED, NULL);
    //添加好友界面有三个功能
    //yi.输入好友账号，点击搜索，弹出相关信息
    //er.以IP连接，点击后进入ip连接界面 有两个输入框 一个输入框输入IP地址，另一个输入框输入端口号，点击连接后进入聊天界面
    //san.发起群聊 点击后弹出好友列表 用来勾选1个或多个好友，点击确定后进入群聊界面

    //创建三个按钮 用来弹出到不同界面
    lv_obj_t *bt1 = lv_btn_create(mainwin);//聊天场所
    lv_obj_t *bt2 = lv_btn_create(mainwin);//好友列表
    lv_obj_t *bt3 = lv_btn_create(mainwin);//设置界面
    lv_obj_set_pos(bt1, 100, 0);
    lv_obj_set_pos(bt2, 200, 0);
    lv_obj_set_pos(bt3, 300, 0);
    lv_obj_set_size(bt1, 50, 70);
    lv_obj_set_size(bt2, 50, 70);
    lv_obj_set_size(bt3, 50, 70);
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
    lv_obj_add_event_cb(bt1, go_shop_cb, LV_EVENT_CLICKED, NULL);
    lv_obj_add_event_cb(bt2, go_fun_cb, LV_EVENT_CLICKED, NULL);
    lv_obj_add_event_cb(bt3, go_chat_cb, LV_EVENT_CLICKED, NULL);

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
