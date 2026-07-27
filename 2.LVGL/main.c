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
lv_obj_t *funwin;    // 控制界面
lv_obj_t *shopwin;   // 商品界面
lv_obj_t *setwin;   // 商品界面
lv_style_t *mystyle; // 中文字体样式
lv_obj_t *pinyin_ime;

lv_obj_t *sw1; // LED1开关
lv_obj_t *sw2; // LED2开关
lv_obj_t *sw3; // LED3开关
lv_obj_t *sw4; // LED4开关
lv_obj_t *sw5; // 蜂鸣器开关

/* 先声明函数，下面才能互相调用 */
lv_style_t *create_chinese_style(char *fontpath, int fontsize);
void show_main(void);
void show_fun(void);
void show_shop(void);
void show_login(void);
void show_set(void);
void beep_ctrl(int cmd);
void led_ctrl(int led_cmd, int onoff);
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


/* 控制LED灯，onoff传 LED_ON 或 LED_OFF */
void led_ctrl(int led_cmd, int onoff)
{
    int fd = open("/dev/Led", O_RDWR);
    if(fd == -1)
    {
        perror("open /dev/Led");
        return;
    }

    ioctl(fd, led_cmd, onoff);

    close(fd);
}

/* 控制蜂鸣器：cmd=1响，cmd=0停 */
void beep_ctrl(int cmd)
{
    int fd1 = open("/dev/beep", O_RDWR);
    if(fd1 == -1)
    {
        perror("open /dev/beep");
        return;
    }

    if(cmd == 1)
    {
        ioctl(fd1, 0, 1); // 响
    }
    else
    {
        ioctl(fd1, 1, 1); // 停
    }

    close(fd1);
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

/* 开关事件：哪个开关变化了，就判断当前开关状态 */
static void sw_event_cb(lv_event_t * e)
{
    if(lv_obj_has_state(sw1, LV_STATE_CHECKED))
    {
        printf("LED1 ON\n");
        led_ctrl(LED1, LED_ON);
    }
    else
    {
        printf("LED1 OFF\n");
        led_ctrl(LED1, LED_OFF);
    }

    if(lv_obj_has_state(sw2, LV_STATE_CHECKED))
    {
        printf("LED2 ON\n");
        led_ctrl(LED2, LED_ON);
    }
    else
    {
        printf("LED2 OFF\n");
        led_ctrl(LED2, LED_OFF);
    }

    if(lv_obj_has_state(sw3, LV_STATE_CHECKED))
    {
        printf("LED3 ON\n");
        led_ctrl(LED3, LED_ON);
    }
    else
    {
        printf("LED3 OFF\n");
        led_ctrl(LED3, LED_OFF);
    }

    if(lv_obj_has_state(sw4, LV_STATE_CHECKED))
    {
        printf("LED4 ON\n");
        led_ctrl(LED4, LED_ON);
    }
    else
    {
        printf("LED4 OFF\n");
        led_ctrl(LED4, LED_OFF);
    }

    if(lv_obj_has_state(sw5, LV_STATE_CHECKED))
    {
        printf("BEEP ON\n");
        beep_ctrl(1);
    }
    else
    {
        printf("BEEP OFF\n");
        beep_ctrl(0);
    }
}

void ta1_cb(lv_event_t * e)
{
    kb1 = lv_100ask_pinyin_ime_create(setwin);
    // 把软键盘跟第一个输入框关联
    lv_obj_add_style(kb1,mystyle,0)
    lv_100ask_pinyin_ime_attach(kb1, ta1);

}

// 输入框2的事件响应函数
void ta2_cb(lv_event_t * e)
{
    // 把软键盘跟第二个输入框关联
    lv_100ask_pinyin_ime_attach(kb1, ta2);
    kb1 = lv_100ask_pinyin_ime_create(setwin);
}


/* 主界面 -> 商品界面 */
void go_shop_cb(lv_event_t * e)
{
    show_shop();
    lv_obj_del(mainwin);
}

/* 主界面 -> 控制界面 */
void go_log_set(lv_event_t * e)
{
    show_set();
    lv_obj_del(loginwin);
}

/* 主界面 -> 控制界面 */
void go_fun_cb(lv_event_t * e)
{
    show_fun();
    lv_obj_del(mainwin);
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
void cb_back_log(lv_event_t * e)
{
    show_login();
    lv_obj_del(mainwin);
}


/* 商品界面 -> 主界面 */
void shop_back_cb(lv_event_t * e)
{
    show_main();
    lv_obj_del(shopwin);
}

/* 控制界面 -> 主界面 */
void fun_back_cb(lv_event_t * e)
{
    show_main();
    lv_obj_del(funwin);
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
    lv_label_set_text(lb1, "欢迎登录粤嵌学生管理系统");

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
void show_set()
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
    
        // 创建软键盘对象(百问网做的)--》软键盘不需要设置坐标和大小

  
    //给输入框和软键盘都绑定中文样式
    lv_obj_add_style(ta1,mystyle,0);
    lv_obj_add_style(ta2,mystyle,0);
    lv_obj_add_style(kb1,mystyle,0);

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
    lv_label_set_text(lb2, "已有账号");
    lv_label_set_text(lb3, "注册");

     // 给按钮添加事件响应函数:  LV_EVENT_CLICKED枚举值表示按钮点击事件
    lv_obj_add_event_cb(bt1, set_back_log, LV_EVENT_CLICKED, NULL);
    lv_obj_add_event_cb(bt2, go_set_main, LV_EVENT_CLICKED, NULL);
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
    lv_label_set_text(title, "欢迎登录粤嵌学生系统");

    /* 商品信息按钮 */
    lv_obj_t *bt1 = lv_btn_create(mainwin);
    lv_obj_set_pos(bt1, 220, 180);
    lv_obj_set_size(bt1, 160, 70);
    lv_obj_add_event_cb(bt1, go_shop_cb, LV_EVENT_CLICKED, NULL);

    lv_obj_t *lb1 = lv_label_create(bt1);
    lv_obj_add_style(lb1, mystyle, 0);
    lv_label_set_text(lb1, "商店");
    lv_obj_center(lb1);

    /* 控制面板按钮 */
    lv_obj_t *bt2 = lv_btn_create(mainwin);
    lv_obj_set_pos(bt2, 430, 180);
    lv_obj_set_size(bt2, 160, 70);
    lv_obj_add_event_cb(bt2, go_fun_cb, LV_EVENT_CLICKED, NULL);

    lv_obj_t *lb2 = lv_label_create(bt2);
    lv_obj_add_style(lb2, mystyle, 0);
    lv_label_set_text(lb2, "控制面板");
    lv_obj_center(lb2);

    lv_obj_t * retbt = lv_btn_create(mainwin); 
    //设置坐标和大小
    lv_obj_set_pos(retbt,600,350);
    lv_obj_set_size(retbt,100,50);
    //给返回按钮添加事件响应函数
    lv_obj_add_event_cb(retbt, cb_back_log, LV_EVENT_CLICKED, NULL);
    //创建标签嵌套到返回按钮上
    lv_obj_t * lb3 = lv_label_create(retbt);
    // 标签使用中文样式
    lv_obj_add_style(lb3,mystyle,0);
    // 给标签设置文字
    lv_label_set_text(lb3,"返回");
    lv_scr_load(mainwin);
}

/* 显示商品界面：从msg.txt读取商品并显示 */
void show_shop(void)
{
    Product p[10];
    int count;
    int i;

    shopwin = lv_obj_create(NULL);
    lv_obj_set_size(shopwin, 800, 480);

    lv_obj_t *title = lv_label_create(shopwin);
    lv_obj_set_pos(title, 80, 20);
    lv_obj_set_size(title, 640, 70);
    lv_obj_add_style(title, mystyle, 0);
    lv_label_set_text(title, "欢迎使用粤嵌自助贩卖机");

    lv_obj_t * img1 = lv_img_create(shopwin);
    LV_IMG_DECLARE(image);        // 声明图片，不是.c里面数组的名字，是结构体的名字
    lv_img_set_src(img1, &image); // 指定图片数据源 不是.c里面数组的名字，是&结构体的名字

    count = read_product(p, 10);

    if(count == 0)
    {
        lv_obj_t *err = lv_label_create(shopwin);
        lv_obj_set_pos(err, 100, 180);
        lv_obj_set_size(err, 600, 80);
        lv_obj_add_style(err, mystyle, 0);
        lv_label_set_text(err, "msg.txt打开失败或者没有商品信息");
    }
    else
    {
        /* 只显示前两个商品 */
        for(i = 0; i < count && i < 2; i++)
        {
            lv_obj_t *box = lv_obj_create(shopwin);
            lv_obj_set_pos(box, 90 + i * 330, 140);
            lv_obj_set_size(box, 290, 210);
            lv_obj_set_style_border_width(box, 3, 0);
            lv_obj_set_style_bg_opa(box, LV_OPA_TRANSP, 0);

            lv_obj_t *lb = lv_label_create(box);
            lv_obj_set_pos(lb, 10, 20);
            lv_obj_set_size(lb, 260, 160);
            lv_obj_add_style(lb, mystyle, 0);
            lv_label_set_text_fmt(lb,
                                  "名称:%s\n数量:%d\n价格:￥%.1f",
                                  p[i].name,
                                  p[i].num,
                                  p[i].price);
        }
    }

    /* 返回按钮 */
    lv_obj_t *retbt = lv_btn_create(shopwin);
    lv_obj_set_pos(retbt, 330, 390);
    lv_obj_set_size(retbt, 140, 55);
    lv_obj_add_event_cb(retbt, shop_back_cb, LV_EVENT_CLICKED, NULL);

    lv_obj_t *retlb = lv_label_create(retbt);
    lv_obj_add_style(retlb, mystyle, 0);
    lv_label_set_text(retlb, "返回");
    lv_obj_center(retlb);
    lv_scr_load(shopwin);
}

/* 显示控制界面 */
void show_fun(void)
{
    funwin = lv_obj_create(NULL);
    lv_obj_set_size(funwin, 800, 480);

    lv_obj_t *title = lv_label_create(funwin);
    lv_obj_set_pos(title, 100, 20);
    lv_obj_set_size(title, 600, 60);
    lv_obj_add_style(title, mystyle, 0);
    lv_label_set_text(title, "控制面板");

    /* LED1 */
    lv_obj_t *lb1 = lv_label_create(funwin);
    lv_obj_set_pos(lb1, 180, 100);
    lv_obj_set_size(lb1, 150, 40);
    lv_obj_add_style(lb1, mystyle, 0);
    lv_label_set_text(lb1, "LED1");
    sw1 = lv_switch_create(funwin);
    lv_obj_set_pos(sw1, 330, 95);
    lv_obj_set_size(sw1, 100, 50);
    lv_obj_add_event_cb(sw1, sw_event_cb, LV_EVENT_VALUE_CHANGED, NULL);

    /* LED2 */
    lv_obj_t *lb2 = lv_label_create(funwin);
    lv_obj_set_pos(lb2, 180, 160);
    lv_obj_set_size(lb2, 150, 40);
    lv_obj_add_style(lb2, mystyle, 0);
    lv_label_set_text(lb2, "LED2");
    sw2 = lv_switch_create(funwin);
    lv_obj_set_pos(sw2, 330, 155);
    lv_obj_set_size(sw2, 100, 50);
    lv_obj_add_event_cb(sw2, sw_event_cb, LV_EVENT_VALUE_CHANGED, NULL);

    /* LED3 */
    lv_obj_t *lb3 = lv_label_create(funwin);
    lv_obj_set_pos(lb3, 180, 220);
    lv_obj_set_size(lb3, 150, 40);
    lv_obj_add_style(lb3, mystyle, 0);
    lv_label_set_text(lb3, "LED3");
    sw3 = lv_switch_create(funwin);
    lv_obj_set_pos(sw3, 330, 215);
    lv_obj_set_size(sw3, 100, 50);
    lv_obj_add_event_cb(sw3, sw_event_cb, LV_EVENT_VALUE_CHANGED, NULL);

    /* LED4 */
    lv_obj_t *lb4 = lv_label_create(funwin);
    lv_obj_set_pos(lb4, 180, 280);
    lv_obj_set_size(lb4, 150, 40);
    lv_obj_add_style(lb4, mystyle, 0);
    lv_label_set_text(lb4, "LED4");
    sw4 = lv_switch_create(funwin);
    lv_obj_set_pos(sw4, 330, 275);
    lv_obj_set_size(sw4, 100, 50);
    lv_obj_add_event_cb(sw4, sw_event_cb, LV_EVENT_VALUE_CHANGED, NULL);

    /* 蜂鸣器 */
    lv_obj_t *lb5 = lv_label_create(funwin);
    lv_obj_set_pos(lb5, 160, 340);
    lv_obj_set_size(lb5, 170, 40);
    lv_obj_add_style(lb5, mystyle, 0);
    lv_label_set_text(lb5, "蜂鸣器");
    sw5 = lv_switch_create(funwin);
    lv_obj_set_pos(sw5, 330, 335);
    lv_obj_set_size(sw5, 100, 50);
    lv_obj_add_event_cb(sw5, sw_event_cb, LV_EVENT_VALUE_CHANGED, NULL);

    /* 返回按钮 */
    lv_obj_t *retbt = lv_btn_create(funwin);
    lv_obj_set_pos(retbt, 560, 380);
    lv_obj_set_size(retbt, 140, 55);
    lv_obj_add_event_cb(retbt, fun_back_cb, LV_EVENT_CLICKED, NULL);

    lv_obj_t *retlb = lv_label_create(retbt);
    lv_obj_add_style(retlb, mystyle, 0);
    lv_label_set_text(retlb, "返回");
    lv_obj_center(retlb);
    printf("4444\n");
    lv_scr_load(funwin);
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
